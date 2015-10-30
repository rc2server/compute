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
#include "InputBufferManager.hpp"
//#include "FormattedException.hpp"
#include "common/RC2Utils.hpp"
#include "common/ZeroInitializedStruct.hpp"
#include "InotifyFileWatcher.hpp"
#include "FileManager.hpp"
#include "RC2Logging.h"

using namespace std;
namespace fs = boost::filesystem;

extern Rboolean R_Visible;

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details);
static void rc2_log_callback(int severity, const char *msg);

inline double currentFractionalSeconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (round(tv.tv_usec/1000.0)/1000.0);
}

struct ExecCompleteArgs {
	RC2::RSession	*session;
	string 		stime;
	json::UnknownElement *clientExtras;
	int outputFileId;
	ExecCompleteArgs(RC2::RSession *inSession, string inTime, json::UnknownElement *inExtras) 
		: session(inSession), stime(inTime), clientExtras(new json::UnknownElement(inExtras))
	{}
	~ExecCompleteArgs() {
		if (clientExtras) delete clientExtras;
	}
};

struct RC2::RSession::Impl : public ZeroInitializedStruct {
	event_base*						eventBase;
	struct bufferevent*				eventBuffer;
	struct evbuffer*				outBuffer;
	InputBufferManager				inputBuffer;
	RInside*						R;
	FileManager						fileManager;
//	InotifyFileWatcher				fileWatcher;
	unique_ptr<TemporaryDirectory>	tmpDir;
	shared_ptr<string>				consoleOutBuffer;
	double							consoleLastWrite;
	int								wspaceId;
	int								sessionRecId;
	int								socket;
	bool							open;
	bool							ignoreOutput;
	bool							sourceInProgress;
	bool							watchVariables;

			Impl();
	void	addFileChangesToJson(JsonDictionary& json);
	string	acknowledgeExecComplete(string stime, json::UnknownElement *clientExtras);

	static void handleExecComplete(int fd, short event_type, void *ctx) 
	{
		LOG(INFO) << "handleExecComplete" << endl;
		ExecCompleteArgs *args = reinterpret_cast<ExecCompleteArgs*>(ctx);
		string s = args->session->_impl->acknowledgeExecComplete(args->stime, args->clientExtras);
		LOG(INFO) << "handleExecComplete got json:" << s << endl;
		args->session->sendJsonToClientSource(s);
		if (args->outputFileId > 0) {
			RC2::JsonDictionary json;
			json.addString("msg", "showoutput");
			json.addLong("fileId", args->outputFileId);
			args->session->sendJsonToClientSource(json);
			LOG(INFO) << "sending showoutput:" << (string)json << endl;
		}
		delete args;
	}
};


RC2::RSession::Impl::Impl()
	: consoleOutBuffer(new string)
{
	FLAGS_log_dir = "/tmp";
	google::InitGoogleLogging("rsession");
	cerr << "is tty:" << isatty(fileno(stdin)) << endl;
}

void
RC2::RSession::Impl::addFileChangesToJson(JsonDictionary& json)
{
	std::vector<string> add, mod, del;
	std::vector<long> imageIds;
	long batchId;
	fileManager.checkWatch(imageIds, batchId);
//	_impl->fileWatcher.stopWatch(add, mod, del);
	mod.insert(mod.end(), add.begin(), add.end()); //merge add/modified
	if (mod.size() > 0)
		json.addStringArray("filesModified", mod);
	if (del.size() > 0)
		json.addStringArray("filesDeleted", del);
	if (imageIds.size() > 0)
		json.addLongArray("images", imageIds);
	if (batchId > 0)
		json.addLong("imgBatch", batchId);
}

string
RC2::RSession::Impl::acknowledgeExecComplete(string stime, json::UnknownElement *clientExtras) 
{
	RC2::JsonDictionary json;
	json.addString("msg", "execComplete");
	json.addString("startTime", stime);
	if (clientExtras)
		json.addObject("clientData", *clientExtras);
	addFileChangesToJson(json);
	return json;
}



void
RC2::RSession::scheduleExecCompleteAcknowledgmenet(string stime, 
	json::UnknownElement *clientExtras, int outputFileId)
{
	ExecCompleteArgs *args = new ExecCompleteArgs(this, stime, clientExtras);
	args->outputFileId = outputFileId;
	struct timeval delay = {0, 100000};
	struct event *ev = event_new(_impl->eventBase, -1, EV_TIMEOUT, RC2::RSession::Impl::handleExecComplete, args);
	event_add(ev, &delay);
}

RC2::RSession::RSession(RSessionCallbacks *callbacks)
		: _impl(new Impl())
{
	FLAGS_stderrthreshold = 0;
	LOG(INFO) << "RSession created" << endl;
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
		_impl->R->parseEvalQNT("save.image()");
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
	_impl->outBuffer = evbuffer_new();
	_impl->fileManager.setEventBase(_impl->eventBase);
}

void
RC2::RSession::startEventLoop()
{
LOG(INFO) << "starting event loop" << endl;
//	sleep(20);
//LOG(INFO) << "sleep over" << endl;
	event_base_loop(_impl->eventBase, 0);
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
		json::Object doc;
		try {
			std::istringstream ist(json);
			json::Reader::Read(doc, ist);
		} catch (json::Reader::ParseException &pe) {
			LOG(ERROR) << "parse exception:" << pe.what() << endl;
			return;
		} catch (std::exception &ex) {
			LOG(ERROR) << "unknown exception parsing:" << ex.what() << endl;
		}
		string msg(((json::String)doc[string("msg")]).Value());
		string arg(((json::String)doc["argument"]).Value());
		_impl->wspaceId = ((json::Number)doc["wspaceId"]).Value();
		if (msg == "open") {
			if (_impl->open) {
				LOG(ERROR) << "duplicate open message received" << endl;
				return;
			}
			_impl->sessionRecId = ((json::Number)doc["sessionRecId"]).Value();
			string dbhost(((json::String)doc[string("dbhost")]).Value());
			string dbuser(((json::String)doc[string("dbuser")]).Value());
			string dbname(((json::String)doc[string("dbname")]).Value());
			ostringstream dbarg;
			dbarg << "postgresql://" << "/" << dbuser << "@" << dbhost << "/" 
				<< dbname < "?application_name=rsession";
			handleOpenCommand(dbarg.str());
		} else {
			if (!_impl->open) {
				LOG(ERROR) << "R not open" << endl;
				return;
			}
			string startTimeStr = ((json::String)doc["startTime"]).Value();
			string jsonStr;
			if (msg == "close") {
				_impl->R->parseEvalQNT("save.image()");
				event_base_loopbreak(_impl->eventBase);
			} else if (msg == "clearFileChanges") {
//				_impl->fileWatcher.startWatch(); //clears cache
			} else if (msg == "execScript") {
				LOG(INFO) << "exec:" << arg << endl;
//				_impl->fileWatcher.startWatch();
				SEXP ans=NULL;
				RInside::ParseEvalResult result = _impl->R->parseEvalR(arg, ans);
				LOG(INFO) << "parseEvalR returned " << (ans != NULL) << endl;
				flushOutputBuffer();
				if (result == RInside::ParseEvalResult::PE_SUCCESS) {
					scheduleExecCompleteAcknowledgmenet(startTimeStr);
					bool sendDelta = _impl->watchVariables || ((json::Boolean)doc["watchVariables"]).Value();
					if (sendDelta)
						jsonStr = handleListVariablesCommand(true, doc["clientData"]);
				} else if (result == RInside::ParseEvalResult::PE_INCOMPLETE) {
					sendTextToClient("Incomplete R statement\n", true);
				}
			} else if (msg == "execFile") {
				jsonStr = executeFile(atol(arg.c_str()), startTimeStr, doc["clientData"]);
			} else if (msg == "help") {
				handleHelpCommand(arg, startTimeStr);
			} else if (msg == "listVariables") {
				bool startWatch = ((json::Boolean)doc["watch"]).Value();
				_impl->watchVariables = startWatch;
				jsonStr = handleListVariablesCommand(startWatch, doc["clientData"]);
			} else if (msg == "getVariable") {
				jsonStr = handleGetVariableCommand(arg, startTimeStr);
			} else if (msg == "toggleVariableWatch") {
				_impl->watchVariables = ((json::Boolean)doc["watch"]).Value();
				if (_impl->watchVariables)
					jsonStr = handleListVariablesCommand(false, doc["clientData"]);
			}
			if (jsonStr.length() > 0)
				sendJsonToClientSource(jsonStr);
		}
	} catch (std::runtime_error error) {
		LOG(WARNING) << "handleJsonCommand error: " << error.what() << endl;
		sendJsonToClientSource(error.what());
	}
}

void
RC2::RSession::handleOpenCommand(string arg)
{
	bool outDir = arg.length() < 1;
	if (outDir)
		arg = "/tmp/" + RC2::GenerateUUID();
	LOG(INFO) << "got open message: " << _impl->wspaceId << endl;
	struct stat sb;
	if (stat(arg.c_str(), &sb) == 0) {
		if (!S_ISDIR(sb.st_mode)) {
			//exists and not a directory
			throw (FormatErrorAsJson(kError_Open_InvalidDir, "invalid working directory"));
		}
	} else {
		//does not exist. try creating a tmp one
		arg = "/tmp/" + GenerateUUID();
		int rc = RC2::MakeDirectoryPath(arg, 0755);
		if (rc != 0) {
			throw (FormatErrorAsJson(kError_Open_CreateDirFailed, 
				"failed to create working directory"));
		}
	}
	_impl->tmpDir = std::move(std::unique_ptr<TemporaryDirectory>(new TemporaryDirectory(arg, outDir)));
	LOG(INFO) << "wd=" << _impl->tmpDir->getPath() << endl;
	_impl->fileManager.setWorkingDir(_impl->tmpDir->getPath());
	_impl->fileManager.initFileManager("postgresql://rc2@127.0.0.1/rc2?application_name=rsession",
		_impl->wspaceId, _impl->sessionRecId);
	_impl->fileManager.loadRData();
	setenv("TMPDIR", arg.c_str(), 1);
	setenv("TEMP", arg.c_str(), 1);
	setenv("R_DEFAULT_DEVICE", "png", 1);
	LOG(INFO) << "setting wd" << endl;
	_impl->R->parseEvalQNT("setwd(\"" + escape_quotes(arg) + "\")");
	_impl->ignoreOutput = true;
	_impl->R->parseEvalQNT("library(rc2);");
	_impl->R->parseEvalQNT("library(knitr)");
	_impl->R->parseEvalQNT("library(markdown)");
	_impl->R->parseEvalQNT("library(tools)");
	_impl->R->parseEvalQNT("rm(argv)"); //RInside creates this even though we passed NULL
	_impl->R->parseEvalQNT("options(device = \"rc2.pngdev\", bitmapType = \"cairo\")");
	_impl->R->parseEvalQNT("load(\".RData\")");
	_impl->ignoreOutput = false;
	sendJsonToClientSource("{\"msg\":\"opensuccess\"}");
	_impl->open = true;
	LOG(INFO) << "open done" << endl;
//	_impl->fileWatcher.initializeWatcher(_impl->tmpDir->getPath());
}

string
RC2::RSession::handleListVariablesCommand(bool delta, json::UnknownElement clientExtras)
{
	string rArg;
	if (delta)
		rArg = "delta=TRUE";
	string cmd("rc2.listVariables(" + rArg + ")");
	JsonDictionary json;
	_impl->ignoreOutput = true;
	try {
		Rcpp::CharacterVector results = _impl->R->parseEval(cmd);
		string jsonStr(results[0]);
		LOG(INFO) << "executed list vars" << endl;

		json::Object varDict;
		std::istringstream ist(jsonStr);
		json::Reader::Read(varDict, ist);

		json.addString("msg", "variableupdate");
		json.addObject("variables", varDict);
		json.addBool("delta", delta);
		try {
			//the following will raise exception if not a dict
			json.addObject("clientData", varDict["clientData"]);
		} catch (json::Exception &je) {
		}
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "listVariables got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
	return json;
}

string
RC2::RSession::handleGetVariableCommand(string varName, string startTime)
{
	string cmd("rc2.sublistValue(\"" + escape_quotes(varName) + "\")");
	LOG(INFO) << "get variable:" << varName << endl;
	_impl->ignoreOutput = true;
	JsonDictionary json;
	try {
		Rcpp::CharacterVector results = _impl->R->parseEval(cmd);
		string jsonStr(results[0]);

		json::Object varDict;
		std::istringstream ist(jsonStr);
		json::Reader::Read(varDict, ist);

		json.addString("msg", "variablevalue");
		json.addString("startTime", startTime);
		json.addObject("value", varDict);
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "getVariable got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
	return json;
}

void
RC2::RSession::handleHelpCommand(string arg, string startTime)
{
	string helpCmd("help(\"" + escape_quotes(arg) + "\")");
	std::cerr << "help:" << helpCmd << endl;
	_impl->ignoreOutput = true;
	try {
		Rcpp::List list = _impl->R->parseEval(helpCmd);
		Rcpp::CharacterVector helpType = list[0];
		Rcpp::CharacterVector helpPaths = list[1];
		std::vector<string> paths;
		for (Rcpp::CharacterVector::iterator itr=helpPaths.begin(); itr != helpPaths.end(); ++itr)
			paths.push_back((char*)*itr);
		JsonDictionary json;
		json.addString("msg", "help");
		json.addString("topic", arg);
		string jsonStr = json.addStringArray("paths", paths);
		sendJsonToClientSource(jsonStr);
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "help got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
}

string
RC2::RSession::executeFile(long fileId, string startTime, json::UnknownElement clientExtras)
{
	string result;
	string fpath = _impl->fileManager.filePathForId(fileId);
	fs::path p(fpath);
	clearFileChanges();
	_impl->fileManager.resetWatch();
//	_impl->fileWatcher.startWatch();
	if (p.extension() == ".Rmd") {
		result = executeRMarkdown(fpath, fileId, startTime, &clientExtras);
	} else if (p.extension() == ".Rnw") {
		result = executeSweave(fpath, fileId, startTime, &clientExtras);
	} else if (p.extension() == ".R") {
		string cmd = "source(file=\"" + escape_quotes(fpath) + "\", echo=TRUE)";
		LOG(INFO) << "executing:" << cmd << endl;
		_impl->sourceInProgress = true;
		_impl->R->parseEvalQNT(cmd);	
		flushOutputBuffer();
		_impl->sourceInProgress = false;
		scheduleExecCompleteAcknowledgmenet(startTime, &clientExtras);
	} else {
		result = formatErrorAsJson(kError_Execfile_InvalidInput, "unsupported file type");
	}
	return result;
}

string
RC2::RSession::executeRMarkdown(string arg, long fileId, string startTime, json::UnknownElement *clientExtras)
{
	fs::path scratchPath(_impl->tmpDir->getPath());
	scratchPath /= ".rc2md";
	create_directories(scratchPath);
	string cmd = "setwd('" + escape_quotes(scratchPath.string()) + "')";
	_impl->R->parseEvalNT(cmd);

	fs::path origFileName(arg);
	string mdname = origFileName.stem().native() + ".md";
	string htmlName = origFileName.stem().native() + ".html";
	cmd = "knit(\"../" + escape_quotes(arg) + "\"); markdownToHTML(\"" 
		+ escape_quotes(mdname) + "\", \"" + escape_quotes(htmlName) + "\");";
	_impl->ignoreOutput = true;
	_impl->R->parseEvalQNT(cmd);
	_impl->ignoreOutput = false;
	flushOutputBuffer();

	//switch working dir back
	cmd = "setwd('" + escape_quotes(scratchPath.parent_path().string()) + "')";
	_impl->R->parseEvalNT(cmd);
	
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
	if (fs::exists(htmlPath) && fs::file_size(htmlPath, ec) > 0) {
		int fileId = _impl->fileManager.findOrAddFile(htmlName);
		scheduleExecCompleteAcknowledgmenet(startTime, clientExtras, fileId);
		return "";
	}
	return formatErrorAsJson(kError_ExecFile_MarkdownFailed, "failed to generate html");
}

string
RC2::RSession::executeSweave(string arg, long fileId, string startTime, json::UnknownElement *clientExtras)
{
	int fileOutputId=0;
	fs::path srcPath(_impl->tmpDir->getPath());
	srcPath /= arg;
	string baseName = srcPath.stem().native();
	string basePdfName = baseName + ".pdf";
	fs::path scratchPath(_impl->tmpDir->getPath());
	scratchPath /= ".rc2sw";
	create_directories(scratchPath);
	string cmd = "setwd('" + escape_quotes(scratchPath.string()) + "')";
	_impl->R->parseEvalNT(cmd);
	cmd = "Sweave('../";
	cmd += escape_quotes(arg);
	cmd += "', encoding='UTF-8')";
	try {
		_impl->R->parseEval(cmd);
	} catch (std::runtime_error &e) {
		sendOutputBufferToClient(true);
		scheduleExecCompleteAcknowledgmenet(startTime, clientExtras);
		return "";
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
//			LOG(INFO) << "closed stdin" << endl;
			char exe[32];
			strncpy(exe, "/usr/bin/texi2pdf", 32);
			char path[1024];
			strncpy(path, texPath.string().c_str(), 1024);
			char batchFlag[4];
			strncpy(batchFlag, "-b", 4);
			char *args[] = {exe, batchFlag, path, NULL};
//			LOG(INFO) << "calling execve" << endl;
			fprintf(parentErr, "calling execve\n");
			execve("/usr/bin/texi2pdf", args, environ);
			fprintf(parentErr, "IMPOSSIBLE\n");
//			LOG(INFO) << "IMPOSSIBLE" << endl;
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
	cmd = "setwd('" + escape_quotes(scratchPath.parent_path().string()) + "')";
	_impl->R->parseEvalNT(cmd);
	_impl->ignoreOutput = false;
	//TODO: delete scratchPath
	flushOutputBuffer();
	scheduleExecCompleteAcknowledgmenet(startTime, clientExtras, fileOutputId);
	return "";
}

void
RC2::RSession::clearFileChanges()
{
	RC2::JsonDictionary json;
	_impl->addFileChangesToJson(json);
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
	
	RC2::JsonDictionary json;
	json.addString("msg", "results");
	json.addBool(is_error ? "stderr" : "stdout", true);
	json.addString("string", output);
	return json;
}

string
RC2::RSession::getWorkingDirectory() const 
{
	return _impl->tmpDir->getPath(); 
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
formatErrorAsJson(int errorCode, string details)
{
	RC2::JsonDictionary json;
	json.addString("msg", "error");
	json.addInt("errorCode", errorCode);
	json.addString("errorDetails", details);
	return json;
}

void 
rc2_log_callback(int severity, const char *msg)
{
	cerr << "logging " << severity << ":" << msg << endl;
}

