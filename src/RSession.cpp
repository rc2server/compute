#include <cstdlib>
#include <iostream>
#include <tclap/CmdLine.h>
#include <execinfo.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/log/utility/setup/file.hpp>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <glog/logging.h>
#include <RInside.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "common/RC2Utils.hpp"
#include "RSession.hpp"
#include "RSessionCallbacks.hpp"
#include "json.hpp"
#include "InputBufferManager.hpp"
//#include "FormattedException.hpp"
#include "JsonCommand.hpp"
#include "common/RC2Utils.hpp"
#include "common/ZeroInitializedStruct.hpp"
#include "InotifyFileWatcher.hpp"
#include "FileManager.hpp"
#include "RC2Logging.h"

using namespace std;
namespace fs = boost::filesystem;
using json2 = nlohmann::json;

extern Rboolean R_Visible;

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details, int queryId=0);
static void rc2_log_callback(int severity, const char *msg);

inline double currentFractionalSeconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (round(tv.tv_usec/1000.0)/1000.0);
}

struct ExecCompleteArgs {
	RC2::RSession *session;
	RC2::JsonCommand command;
	int outputFileId;
	string outputFileName;
	int queryId;
	ExecCompleteArgs(RC2::RSession *inSession, RC2::JsonCommand inCommand, int inQueryId, int outputId=0, string outputName="") 
		: session(inSession), queryId(inQueryId), command(inCommand), outputFileId(outputId), outputFileName(outputName)
	{}
};


struct RC2::RSession::Impl : public ZeroInitializedStruct {
	struct event_base*				eventBase;
	struct bufferevent*				eventBuffer;
	struct evbuffer*				outBuffer;
	InputBufferManager				inputBuffer;
	RInside*						R;
	FileManager						fileManager;
	unique_ptr<TemporaryDirectory>	tmpDir;
	shared_ptr<string>				consoleOutBuffer;
	double							consoleLastWrite;
	int								wspaceId;
	int								sessionRecId;
	int								socket;
	int								currentQueryId;
	bool							open;
	bool							ignoreOutput;
	bool							sourceInProgress;
	bool							watchVariables;

			Impl();
			Impl(const Impl &copy) = delete;
			Impl& operator=(const Impl&) = delete;
			void	addFileChangesToJson(json2& json);
	string	acknowledgeExecComplete(JsonCommand &command, int queryId);

	static void handleExecComplete(int fd, short event_type, void *ctx) 
	{
		ExecCompleteArgs *args = reinterpret_cast<ExecCompleteArgs*>(ctx);
		string s = args->session->_impl->acknowledgeExecComplete(args->command, args->queryId);
		LOG(INFO) << "handleExecComplete got json:" << s << endl;
		args->session->sendJsonToClientSource(s);
		if (args->outputFileId > 0) {
			json2 results = { {"msg", "showoutput"}, {"fileId", args->outputFileId}, {"fileName", args->outputFileName} };
			args->session->sendJsonToClientSource(results.dump());
			LOG(INFO) << "sending showoutput:" << results << endl;
		}
		delete args;
	}

	class NotifySuspender {
		Impl& _impl;
	public:
		NotifySuspender(Impl &impl)
		:_impl(impl)
		{
			_impl.ignoreOutput = true;
//			_impl.fileManager.suspendNotifyEvents();
		}
		
		~NotifySuspender()
		{
			_impl.ignoreOutput = false;
//			_impl.fileManager.resumeNotifyEvents();
		}
		
	};
	
};


RC2::RSession::Impl::Impl()
	: consoleOutBuffer(new string)
{
	FLAGS_log_dir = "/tmp";
	google::InitGoogleLogging("rsession");
	cerr << "is tty:" << isatty(fileno(stdin)) << endl;
}

void
RC2::RSession::Impl::addFileChangesToJson(json2& json)
{
	std::vector<string> add, mod, del;
	std::vector<long> imageIds;
	long batchId;
	fileManager.checkWatch(imageIds, batchId);
	mod.insert(mod.end(), add.begin(), add.end()); //merge add/modified
	if (mod.size() > 0)
		json["filesModified"] = mod;
	if (del.size() > 0)
		json["filesDeleted"] = del;
	if (imageIds.size() > 0)
		json["images"] = imageIds;
	if (batchId > 0)
		json["imgBatch"] = batchId;
}

string
RC2::RSession::Impl::acknowledgeExecComplete(JsonCommand& command, int queryId) 
{
	LOG(INFO) << "exec complete posting" << endl;
	fileManager.cleanupImageWatch();
	json2 results;
	results["msg"] = "execComplete";
	results["startTime"] = command.startTimeStr();
	if (queryId > 0)
		results["queryId"] = queryId;
	if (!command.clientData().is_null())
		results["clientData"] = command.clientData();
	addFileChangesToJson(results);
	return results.dump();
}



void
RC2::RSession::scheduleExecCompleteAcknowledgmenet(JsonCommand& command, int queryId,
	int outputFileId, string outputFileName)
{
	ExecCompleteArgs *args = new ExecCompleteArgs(this, command, queryId, outputFileId, outputFileName);
	struct timeval delay = {0, 1};
	struct event *ev = event_new(_impl->eventBase, -1, 0, 	RC2::RSession::Impl::handleExecComplete, args);
	event_priority_set(ev, 0);
 	event_add(ev, &delay);
}

RC2::RSession::RSession(RSessionCallbacks *callbacks)
		: _impl(new Impl())
{
	FLAGS_stderrthreshold = 0;
	_callbacks = callbacks;
	setenv("R_HOME", "/usr/local/lib/R", 0); //will not overwrite
	_impl->R = new RInside(0, NULL, false, true, false);
	auto clambda = [&](const string &text, bool is_error) -> void {
		consoleCallback(text, is_error);
	};
	_callbacks->_writeLambda = clambda;
	_impl->R->set_callbacks(_callbacks);
}

RC2::RSession::~RSession()
{
	if (_impl->R) {
		_impl->fileManager.saveRData();
		delete _impl->R;
		_impl->R = nullptr;
	}
	if (nullptr != _impl->outBuffer)
		evbuffer_free(_impl->outBuffer);
	LOG(INFO) << "RSession destroyed" << endl;
}

void
RC2::RSession::consoleCallback(const string &text, bool is_error)
{
//	LOG(INFO) << "write cb: " << text << "(ignore=" << _impl->ignoreOutput 
//		<< ",vis=" << R_Visible << ",sip=" << _impl->sourceInProgress << ")" << endl;
	if (_impl->ignoreOutput)
		return;
	if (is_error) {
		string errstr = formatStringAsJson(text, true);
		sendJsonToClientSource(errstr);
	} else {
		if (R_Visible || _impl->sourceInProgress) {
			_impl->consoleOutBuffer->append(text);
		}
	}
}

bool
RC2::RSession::parseArguments(int argc, char *argv[])
{
	try {
		TCLAP::CmdLine cmdLine("Handle a remote R connection", ' ', "0.1");
		
		TCLAP::ValueArg<int> portArg("s", "socket", "socket to listen on", 
			true, -1, "socketnum", cmdLine);
		
		TCLAP::SwitchArg switchArg("v", "verbose", "enable logging", cmdLine);
			
		cmdLine.parse(argc, argv);
		_impl->socket = portArg.getValue();
		bool verbose = switchArg.getValue();

//		logging::core::get()->set_filter
//		(
//			logging::trivial::severity >= (verbose ? logging::trivial::info : logging::trivial::warning)
//		);
		
		LOG(INFO) << "arguments parsed\n";
		
	} catch (TCLAP::ArgException &e) {
		LOG(ERROR) << "error:" << e.error() << endl;
	}
	return true;
}

void
RC2::RSession::installExitHandler(void(*handler)(short flags))
{
	//TODO: implement
}

void
RC2::RSession::prepareForRunLoop()
{
//	event_set_log_callback(rc2_log_callback);
	struct event_config *config = event_config_new();
	event_config_require_features(config, EV_FEATURE_FDS);
	_impl->eventBase = event_base_new_with_config(config);
	event_config_free(config);
	//0=high, 1=normal, 2=low, 3=lowlow. default is num/2, so we need 4 to make default=1
	event_base_priority_init(_impl->eventBase, 4); 
	//only listen on socket if we have a valid network socket (won't for unit testing)
	if (_impl->socket > 0) {
		evutil_make_socket_nonblocking(_impl->socket);
		_impl->eventBuffer = bufferevent_socket_new(_impl->eventBase, _impl->socket, 0);
		if (_impl->eventBuffer == nullptr) {
			LOG(ERROR) << "failed to create bufferevent socket" << endl;
			return;
		}
		bufferevent_setcb(_impl->eventBuffer,
			RSession::handleJsonStatic,
			nullptr,
			nullptr,
			this);
		bufferevent_enable(_impl->eventBuffer, EV_READ|EV_WRITE|BEV_OPT_DEFER_CALLBACKS);
	}
	_impl->outBuffer = evbuffer_new();
	_impl->fileManager.setEventBase(_impl->eventBase);
}

void
RC2::RSession::startEventLoop()
{
	//commented out lines for when trying to attach via gdb
//LOG(INFO) << "starting event loop" << endl;
//	sleep(20);
//LOG(INFO) << "sleep over" << endl;
	event_base_loop(_impl->eventBase, 0);
}

void 
RC2::RSession::stopEventLoop()
{
	if (_impl->eventBuffer != NULL) {
		bufferevent_free(_impl->eventBuffer);
	}
	event_base_loopbreak(_impl->eventBase);
}

void 
RC2::RSession::handleJsonStatic(struct bufferevent *bev, void *ctx)
{
LOG(INFO) << "got input" << endl;
	try {
		RC2::RSession *me = static_cast<RC2::RSession*>(ctx);
		me->_impl->inputBuffer.appendData(bufferevent_get_input(bev));
		if (me->_impl->inputBuffer.hasCompleteMessage()) {
			me->handleJsonCommand(me->_impl->inputBuffer.popCurrentMessage());
		}
	} catch (exception const& e) {
		LOG(ERROR) << "exception in handleJsonStatic:" << e.what() << endl;
	}
}

void
RC2::RSession::handleJsonCommand(string json)
{
	try {
		if (json.length() < 1)
			return;
		LOG(INFO) << "json=" << json << endl;
		json2 doc;
		try {
			std::istringstream istr(json);
			istr >> doc;
		} catch (std::invalid_argument &iae) {
			LOG(ERROR) << "parse exception:" << iae.what() << endl;
			return;
		} catch (std::exception &ex) {
			LOG(ERROR) << "unknown exception parsing:" << ex.what() << endl;
		}
		JsonCommand command(doc);
		if (command.type() == CommandType::Open) {
			if (_impl->open) {
				LOG(ERROR) << "duplicate open message received" << endl;
				return;
			}
			handleOpenCommand(command);
		} else {
			if (!_impl->open) {
				LOG(ERROR) << "R not open" << endl;
				return;
			}
			_impl->currentQueryId = command.raw().value("queryId", 0);

			switch(command.type()) {
				case CommandType::Close:
				event_base_loopbreak(_impl->eventBase);
				break;
				case CommandType::ClearFileChanges:
				_impl->fileManager.resetWatch(); //clears cache
				break;
				case CommandType::ExecScript:
				handleExecuteScript(command);
				break;
				case CommandType::ExecFile:
				executeFile(command);
				break;
				case CommandType::Help:
				handleHelpCommand(command);
				break;
				case CommandType::ListVariables:
				_impl->watchVariables = command.raw().value("watch", false);
				handleListVariablesCommand(true, command);
				break;
				case CommandType::GetVariable:
				handleGetVariableCommand(command);
				break;
				case CommandType::ToggleWatch:
				_impl->watchVariables = command.raw().value("toggleVariableWatch", false);
				if (_impl->watchVariables)
					handleListVariablesCommand(false, command);
				break;
			}
		}
	} catch (std::runtime_error error) {
		LOG(WARNING) << "handleJsonCommand error: " << error.what() << endl;
		sendJsonToClientSource(error.what());
	}
	_impl->currentQueryId = 0;
}

void
RC2::RSession::handleOpenCommand(JsonCommand &cmd)
{
	if (_impl->open) {
		LOG(ERROR) << "duplicate open message received" << endl;
		return;
	}
	_impl->wspaceId = cmd.raw()["wspaceId"];
	try {
		_impl->sessionRecId = cmd.raw()["sessionRecId"];
		string dbhost(cmd.valueForKey("dbhost"));
		string dbuser(cmd.valueForKey("dbuser"));
		string dbname(cmd.valueForKey("dbname"));
		ostringstream connectString;
		connectString << "postgresql://" << dbuser << "@" << dbhost << "/" 
			<< dbname << "?application_name=rsession&sslmode=disable";
		string dbpass(cmd.valueForKey("dbpass"));
		if (dbpass.length() > 0)
			connectString << "&password=" << dbpass;
		LOG(INFO) << connectString.str() << endl;
		
		if (NULL ==  _impl->eventBase) {
			LOG(ERROR) << "handleOpenCommand called before prepareForRunLoop()" << endl;
			abort();
		}
		string workDir = "/tmp/" + GenerateUUID();
		int rc = RC2::MakeDirectoryPath(workDir, 0755);
		if (rc != 0) {
			throw std::runtime_error("failed to create working directory");
		}
		_impl->tmpDir = std::move(std::unique_ptr<TemporaryDirectory>(new TemporaryDirectory(workDir, false)));
		LOG(INFO) << "wd=" << _impl->tmpDir->getPath() << endl;
		_impl->fileManager.setWorkingDir(_impl->tmpDir->getPath());
		_impl->fileManager.initFileManager(connectString.str().c_str(), _impl->wspaceId, _impl->sessionRecId);
		bool haveRData = _impl->fileManager.loadRData();
		setenv("TMPDIR", workDir.c_str(), 1);
		setenv("TEMP", workDir.c_str(), 1);
		setenv("R_DEFAULT_DEVICE", "png", 1);
		_impl->R->parseEvalQNT("setwd(\"" + escape_quotes(workDir) + "\")");
		_impl->ignoreOutput = true;
		_impl->R->parseEvalQNT("library(rc2);");
		_impl->R->parseEvalQNT("library(knitr)");
		_impl->R->parseEvalQNT("library(markdown)");
		_impl->R->parseEvalQNT("library(tools)");
		_impl->R->parseEvalQNT("rm(argv)"); //RInside creates this even though we passed NULL
		_impl->R->parseEvalQNT("options(device = \"rc2.pngdev\", bitmapType = \"cairo\")");
		if (haveRData)
			_impl->R->parseEvalQNT("load(\".RData\")");
		_impl->ignoreOutput = false;
		json2 response =  { {"msg", "openresponse"}, {"success", true} };
		sendJsonToClientSource(response.dump());
		_impl->open = true;
	} catch (std::runtime_error &err) {
		json2 error = { {"msg", "openresponse"}, {"success", false}, {"errorMessage", err.what()} };
		sendJsonToClientSource(error.dump());
	}
}

void
RC2::RSession::handleExecuteScript(JsonCommand& command) {
	LOG(INFO) << "exec:" << command.argument() << endl;
	_impl->fileManager.resetWatch();
	SEXP ans=NULL;
	RInside::ParseEvalResult result = _impl->R->parseEvalR(command.argument(), ans);
	LOG(INFO) << "parseEvalR returned " << (ans != NULL) << endl;
	flushOutputBuffer();
	if (result == RInside::ParseEvalResult::PE_SUCCESS) {
		scheduleExecCompleteAcknowledgmenet(command, _impl->currentQueryId);
		bool sendDelta = _impl->watchVariables || command.watchVariables();
		if (sendDelta)
			handleListVariablesCommand(true, command);
	} else if (result == RInside::ParseEvalResult::PE_INCOMPLETE) {
		sendTextToClient("Incomplete R statement\n", true);
	}
}

void
RC2::RSession::handleListVariablesCommand(bool delta, JsonCommand& command)
{
	string rArg;
	if (delta)
		rArg = "delta=TRUE";
	string cmd("rc2.listVariables(" + rArg + ")");
	_impl->ignoreOutput = true;
	try {
		Rcpp::CharacterVector rResults = _impl->R->parseEval(cmd);
		string jsonStr(rResults[0]);
		LOG(INFO) << "executed list vars" << endl;

		auto varDict = json2::parse(jsonStr);
		json2 results = {
			{"variables", varDict},
			{"msg", "variableupdate"},
			{"delta", delta}
		};
		if (!command.clientData().is_null())
			results["clientData"] = command.clientData();
		sendJsonToClientSource(results.dump());
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "listVariables got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
}

void
RC2::RSession::handleGetVariableCommand(JsonCommand &command)
{
	string rcmd("rc2.sublistValue(\"" + escape_quotes(command.argument()) + "\")");
	LOG(INFO) << "get variable:" <<command.argument() << endl;
	_impl->ignoreOutput = true;
	try {
		Rcpp::CharacterVector rResults = _impl->R->parseEval(rcmd);
		string jsonStr(rResults[0]);
		json2 varDict = json2::parse(jsonStr);
		json2 results = {
			{"msg", "variablevalue"},
			{"value", varDict},
			{"startTime", command.startTimeStr()}
		};
		sendJsonToClientSource(results.dump());
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "getVariable got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
}

void
RC2::RSession::handleHelpCommand(JsonCommand& command)
{
	string helpCmd("help(\"" + escape_quotes(command.argument()) + "\")");
	std::cerr << "help:" << helpCmd << endl;
	_impl->ignoreOutput = true;
	try {
		Rcpp::List list = _impl->R->parseEval(helpCmd);
		Rcpp::CharacterVector helpType = list[0];
		Rcpp::CharacterVector helpPaths = list[1];
		std::vector<string> paths;
		for (Rcpp::CharacterVector::iterator itr=helpPaths.begin(); itr != helpPaths.end(); ++itr)
			paths.push_back((char*)*itr);
		json2 results = {
			{"msg", "help"},
			{"topic", command.argument()},
			{"paths", paths}
		};
		sendJsonToClientSource(results.dump());
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "help got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
}

void
RC2::RSession::executeFile(JsonCommand& command) {
	long fileId = atol(command.argument().c_str());
	string fpath = _impl->fileManager.filePathForId(fileId);
	fs::path p(fpath);
	clearFileChanges();
	_impl->fileManager.resetWatch();
	if (p.extension() == ".Rmd") {
		executeRMarkdown(fpath, fileId, command);
	} else if (p.extension() == ".Rnw") {
		executeSweave(fpath, fileId, command);
	} else if (p.extension() == ".R") {
		string rcmd = "source(file=\"" + escape_quotes(fpath) + "\", echo=TRUE)";
		LOG(INFO) << "executing:" << rcmd << endl;
		_impl->sourceInProgress = true;
		_impl->R->parseEvalQNT(rcmd);	
		flushOutputBuffer();
		_impl->sourceInProgress = false;
		scheduleExecCompleteAcknowledgmenet(command, _impl->currentQueryId);
	} else {
		sendJsonToClientSource(formatErrorAsJson(kError_Execfile_InvalidInput, "unsupported file type", _impl->currentQueryId));
	}
}

void
RC2::RSession::executeRMarkdown(string filePath, long fileId, JsonCommand& command)
{
	fs::path scratchPath(_impl->tmpDir->getPath());
	scratchPath /= ".rc2md";
	create_directories(scratchPath);
	string rcmd = "setwd('" + escape_quotes(scratchPath.string()) + "')";
	_impl->R->parseEvalNT(rcmd);
	bool fileGenerated = false;
	fs::path origFileName(filePath);
	string mdname = origFileName.stem().native() + ".md";
	string htmlName = origFileName.stem().native() + ".html";
	rcmd = "knit(\"../" + escape_quotes(filePath) + "\"); markdownToHTML(\"" 
		+ escape_quotes(mdname) + "\", \"" + escape_quotes(htmlName) + "\");";

	{
		Impl::NotifySuspender suspender(*_impl);
		_impl->R->parseEvalQNT(rcmd);
		flushOutputBuffer();
	}
	//switch working dir back
		rcmd = "setwd('" + escape_quotes(scratchPath.parent_path().string()) + "')";
		_impl->R->parseEvalNT(rcmd);
		
		//move generated html to tmpDir
		fs::path tmpHtml = scratchPath;
		tmpHtml /= htmlName;
		fs::path htmlPath = _impl->tmpDir->getPath();
		htmlPath /= htmlName;
		fs::copy_file(tmpHtml, htmlPath, fs::copy_option::overwrite_if_exists);
		//delete scratch dir
		fs::remove_all(scratchPath);
		LOG(INFO) << "should be html at " << htmlPath << endl;
		boost::system::error_code ec;
		fileGenerated = fs::exists(htmlPath) && fs::file_size(htmlPath, ec) > 0;
//	}
	
	if (fileGenerated) {
		Impl::NotifySuspender suspender(*_impl);
		int fileId = _impl->fileManager.findOrAddFile(htmlName);
		scheduleExecCompleteAcknowledgmenet(command, _impl->currentQueryId, fileId, htmlName);
	} else {
		formatErrorAsJson(kError_ExecFile_MarkdownFailed, "failed to generate html", _impl->currentQueryId);
	}
}

void
RC2::RSession::executeSweave(string filePath, long fileId, JsonCommand& command)
{
	int fileOutputId=0;
	fs::path srcPath(_impl->tmpDir->getPath());
	srcPath /= filePath;
	string baseName = srcPath.stem().native();
	string basePdfName = baseName + ".pdf";
	fs::path scratchPath(_impl->tmpDir->getPath());
	scratchPath /= ".rc2sw";
	create_directories(scratchPath);
	string rcmd = "setwd('" + escape_quotes(scratchPath.string()) + "')";
	_impl->R->parseEvalNT(rcmd);
	rcmd = "Sweave('../";
	rcmd += escape_quotes(filePath);
	rcmd += "', encoding='UTF-8')";
	try {
		_impl->R->parseEval(rcmd);
	} catch (std::runtime_error &e) {
		sendOutputBufferToClient(true);
		scheduleExecCompleteAcknowledgmenet(command, _impl->currentQueryId);
		return;
	}
	fs::path texPath(scratchPath);
	texPath /= baseName + ".tex";
	if (!fs::exists(texPath)) {
		//there was an error
		LOG(WARNING) << "sweave failed" << endl;
	} else {
		_impl->ignoreOutput = true;
		pid_t parent = getpid();
		pid_t pid = fork();
		if (pid == -1) {
			LOG(ERROR) << "fork() failed for texi2dvi" << endl;
		} else if (pid > 0) {
			int status=0;
			LOG(INFO) << "calling waitpid:" << pid << endl;
			waitpid(pid, &status, 0);
			LOG(INFO) << "finished waitpid:" << status << endl;
		} else {
			int oldStdErr = dup(2);
			FILE *parentErr = fdopen(oldStdErr, "w");
			int devnull = open("/dev/null", O_WRONLY);
			dup2(devnull, 1);
			dup2(devnull, 2);
			close(0);
			fprintf(parentErr, "closed stdin\n");
			char exe[32];
			strncpy(exe, "/usr/bin/texi2pdf", 32);
			char path[1024];
			strncpy(path, texPath.string().c_str(), 1024);
			char batchFlag[4];
			strncpy(batchFlag, "-b", 4);
			char *args[] = {exe, batchFlag, path, NULL};
			fprintf(parentErr, "calling execve\n");
			execve("/usr/bin/texi2pdf", args, environ);
			fprintf(parentErr, "IMPOSSIBLE\n");
		}
		_impl->ignoreOutput = false;
		fs::path genPdfPath(scratchPath);
		genPdfPath /= basePdfName;
		fs::path destPdfPath(_impl->tmpDir->getPath());
		destPdfPath /= basePdfName;
		boost::system::error_code ec;
		if (fs::exists(genPdfPath) && fs::file_size(genPdfPath, ec) > 0) {
			fs::copy_file(genPdfPath, destPdfPath, fs::copy_option::overwrite_if_exists);
			fileOutputId = _impl->fileManager.findOrAddFile(basePdfName);
		} else {
			LOG(INFO) << "genPdfPath empty:" << genPdfPath << endl;
			fs::path errorPath(scratchPath);
			errorPath /= baseName + ".log";
			if (fs::exists(errorPath)) {
				fs::path logPath(srcPath.parent_path());
				logPath /= baseName = ".log";
				fs::copy_file(errorPath, logPath, fs::copy_option::overwrite_if_exists);
				//TODO: send message saying to look at the log file
			} else {
				//report error with no details
			}
		}
	}
	rcmd = "setwd('" + escape_quotes(scratchPath.parent_path().string()) + "')";
	_impl->R->parseEvalNT(rcmd);
	_impl->ignoreOutput = false;
	//TODO: delete scratchPath
	flushOutputBuffer();
	scheduleExecCompleteAcknowledgmenet(command, _impl->currentQueryId, fileOutputId, basePdfName);
}

void
RC2::RSession::clearFileChanges()
{
	json2 ignoredJson;
	_impl->addFileChangesToJson(ignoredJson);
}

//causes R to save any images generated and then sends the output buffer to the client
void
RC2::RSession::flushOutputBuffer()
{
	_impl->ignoreOutput = true;
	_impl->R->parseEvalQNT("rc2.pngoff()");		
	_impl->ignoreOutput = false;
	sendOutputBufferToClient(false);
}

//if the consoleOutputBuffer has any text in it, sends that to client and clears the buffer
void
RC2::RSession::sendOutputBufferToClient(bool is_error)
{
	if (_impl->consoleOutBuffer->length() > 0) {
		sendTextToClient(*_impl->consoleOutBuffer.get(), is_error);
		_impl->consoleOutBuffer.get()->clear();
	}
}

//sends raw text (such has R console output or hard-coded string) packaged as json
void
RC2::RSession::sendTextToClient(string text, bool is_error)
{
	string msg = formatStringAsJson(text, is_error);
	sendJsonToClientSource(msg);
}

//packages json as bytes and sends over socket
// subclasses can override (i.e. testing) to avoid socket use
void
RC2::RSession::sendJsonToClientSource(string json)
{
	if (json.length() < 1)
		return;
	if (_impl->socket > 0) { //only send if we have a valid socket
		LOG(INFO) << "sending:" << json << "(" << json.length() << " bytes)" << endl;
		int32_t header[2];
		header[0] = kRSessionMagicNumber;
		header[1] = json.length();
		header[0] = htonl(header[0]);
		header[1] = htonl(header[1]);
		evbuffer_add(_impl->outBuffer, &header, sizeof(header));
		evbuffer_add(_impl->outBuffer, json.c_str(), json.length());
		
		bufferevent_write_buffer(_impl->eventBuffer, _impl->outBuffer);
	} else {
		LOG(WARNING) << "output w/o client:" << json << endl;
	}
}

string
RC2::RSession::formatStringAsJson(const string &input, bool is_error)
{
	if (input.length() < 1)
		return "";
	string output = input;
	
	if (_impl->sourceInProgress) {
		static const boost::regex reg("\n\\$value\n.*$");
		output = boost::regex_replace(input, reg, "\n");
	}
	json2 response = {
		{"msg", "results"},
		{"string", output},
		{is_error ? "stderr" : "stdout", true}
	};
	if (_impl->currentQueryId > 0)
		response["queryId"] = _impl->currentQueryId;
	return response.dump();
}

string
RC2::RSession::getWorkingDirectory() const 
{
	return _impl->tmpDir->getPath(); 
}

struct event_base* RC2::RSession::getEventBase() const
{
	return _impl->eventBase;
}


static string 
escape_quotes(const string before)
{
	string after;
	after.reserve(before.length() + 4);
	for (string::size_type i=0; i < before.length(); ++i) {
		switch (before[i]) {
			case '"':
			case '\\':
				after += '\\';
			
			default:
				after += before[i];
		}
	}
	return after;
}

static string
formatErrorAsJson(int errorCode, string details, int queryId)
{
	json2 response = { 
		{"msg", "error"}, 
		{"errorCode", errorCode}, 
		{"errorDetails", details} 
	};
	if (queryId > 0)
		response["queryId"] = queryId;
	return response.dump();
}

void 
rc2_log_callback(int severity, const char *msg)
{
	cerr << "logging " << severity << ":" << msg << endl;
}

