#include <cstdlib>
#include <iostream>
#include <tclap/CmdLine.h>
#include <execinfo.h>
#include <sys/stat.h>
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
#include "InotifyFileWatcher.hpp"
#include "FileManager.hpp"

using namespace std;
namespace fs = boost::filesystem;

#undef LOG
#define LOG(x) cerr

extern Rboolean R_Visible;

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details);
static void rc2_log_callback(int severity, const char *msg);

struct RC2::RSession::Impl {
	event_base*						eventBase;
	struct bufferevent*				eventBuffer;
	struct evbuffer*				outBuffer;
	InputBufferManager				inputBuffer;
	RInside*						R;
	FileManager						fileManager;
//	InotifyFileWatcher				fileWatcher;
	unique_ptr<TemporaryDirectory>	tmpDir;
	shared_ptr<string>				consoleOutBuffer;
	int								wspaceId;
	int								socket;
	bool							open;
	bool							ignoreOutput;
	bool							sourceInProgress;
	bool							watchVariables;

	Impl();
};


RC2::RSession::Impl::Impl()
	: consoleOutBuffer(new string), open(false), ignoreOutput(false), 
		sourceInProgress(false), watchVariables(false), outBuffer(nullptr)
{
	FLAGS_log_dir = "/tmp";
	google::InitGoogleLogging("rsession");
}

RC2::RSession::RSession(RSessionCallbacks *callbacks)
		: _impl(new Impl())
{
	LOG(INFO) << "RSession created" << endl;
	_callbacks = callbacks;
	setenv("R_HOME", "/usr/local/lib/R", 0); //will not overwrite
	_impl->R = new RInside(0, NULL, false, true, false);
	auto clambda = [&](const string &text, bool is_error) -> void {
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
	};
	_callbacks->_writeLambda = clambda;
	_impl->R->set_callbacks(_callbacks);
}

RC2::RSession::~RSession()
{
	if (_impl->R) {
		_impl->R->parseEvalQNT("save.image()");
		delete _impl->R;
		_impl->R = nullptr;
	}
	if (nullptr != _impl->outBuffer)
		evbuffer_free(_impl->outBuffer);
	LOG(INFO) << "RSession destroyed" << endl;
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
//	_impl->fileWatcher.setEventBase(_impl->eventBase);
	_impl->fileManager.setEventBase(_impl->eventBase);
}

void
RC2::RSession::startEventLoop()
{
	event_base_loop(_impl->eventBase, 0);
}

void 
RC2::RSession::handleJsonStatic(struct bufferevent *bev, void *ctx)
{
	LOG(INFO) << "got bufferevent\n";
	RC2::RSession *me = static_cast<RC2::RSession*>(ctx);
	me->_impl->inputBuffer.appendData(bufferevent_get_input(bev));
	if (me->_impl->inputBuffer.hasCompleteMessage()) {
		me->handleJsonCommand(me->_impl->inputBuffer.popCurrentMessage());
	}
}

void
RC2::RSession::handleJsonCommand(string json)
{
	LOG(INFO) << "handleJsonCommand\n";
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
			handleOpenCommand(arg);
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
				SEXP ans;
				RInside::ParseEvalResult result = _impl->R->parseEvalR(arg, ans);
				flushOutputBuffer();
				if (result == RInside::ParseEvalResult::PE_SUCCESS) {
					sendJsonToClientSource(acknowledgeExecComplete(startTimeStr));
					bool sendDelta = _impl->watchVariables || ((json::Boolean)doc["watchVariables"]).Value();
					if (sendDelta)
						jsonStr = handleListVariablesCommand(true, doc["clientData"]);
				} else if (result == RInside::ParseEvalResult::PE_INCOMPLETE) {
					sendTextToClient("Incomplete R statement\n", true);
				}
			} else if (msg == "execFile") {
				jsonStr = executeFile(arg, startTimeStr, doc["clientData"]);
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
	_impl->fileManager.initFileManager("postgresql://rc2@127.0.0.1:9000/rc2?application_name=rsession",
		_impl->wspaceId);
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
		json.addString("msg", "results");
		json.addString("startTime", startTime);
		json.addString("helpTopic", arg);
		string jsonStr = json.addStringArray("helpPath", paths);
		sendJsonToClientSource(jsonStr);
	} catch (std::runtime_error &err) {
		LOG(WARNING) << "help got error:" << err.what() << endl;
	}
	_impl->ignoreOutput = false;
}

string
RC2::RSession::executeFile(string arg, string startTime, json::UnknownElement clientExtras)
{
	string result;
	fs::path p(arg);
	clearFileChanges();
//	_impl->fileWatcher.startWatch();
	if (p.extension() == ".Rmd") {
		result = executeRMarkdown(arg, startTime, &clientExtras);
	} else if (p.extension() == ".Rnw") {
		result = executeSweave(arg, startTime, &clientExtras);
	} else if (p.extension() == ".R") {
		string cmd = "source(file=\"" + escape_quotes(arg) + "\", echo=TRUE)";
		_impl->sourceInProgress = true;
		_impl->R->parseEvalQNT(cmd);	
		flushOutputBuffer();
		_impl->sourceInProgress = false;
		result = acknowledgeExecComplete(startTime, &clientExtras);
	} else {
		result = formatErrorAsJson(kError_Execfile_InvalidInput, "unsupported file type");
	}
	return result;
}

string
RC2::RSession::executeRMarkdown(string arg, string startTime, json::UnknownElement *clientExtras)
{
	fs::path p(arg);
	string mdname = p.stem().native() + ".md";
	string htmlName = p.stem().native() + ".html";
	string cmd = "knit(\"" + escape_quotes(arg) + "\"); markdownToHTML(\"" 
		+ escape_quotes(mdname) + "\", \"" + escape_quotes(htmlName) + "\");";
	_impl->ignoreOutput = true;
	_impl->R->parseEvalQNT(cmd);
	_impl->ignoreOutput = false;
	flushOutputBuffer();
	
	//delete generated .md file
	fs::path mdfile = _impl->tmpDir->getPath();
	mdfile /= mdname;
	fs::remove(mdfile);
	//delete figure directory
	fs::path figureDir = _impl->tmpDir->getPath();
	figureDir /= "figure";
	if (fs::exists(figureDir))
		fs::remove_all(figureDir);
	//find html file
	fs::path htmlfile = _impl->tmpDir->getPath();
	htmlfile /= htmlName;
	if (fs::exists(htmlfile)) {
		return acknowledgeExecComplete(startTime, clientExtras);
	}
	return formatErrorAsJson(kError_ExecFile_MarkdownFailed, "failed to generate html");
}

string
RC2::RSession::executeSweave(string arg, string startTime, json::UnknownElement *clientExtras)
{
	fs::path srcPath(_impl->tmpDir->getPath());
	srcPath /= arg;
	string baseName = srcPath.stem().native();
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
		return acknowledgeExecComplete(startTime, clientExtras);
	}
	fs::path texPath(scratchPath);
	texPath /= baseName + ".tex";
	if (!fs::exists(texPath)) {
		//there was an error
		LOG(WARNING) << "sweave failed" << endl;
	} else {
		cmd = "texi2pdf('";
		cmd += escape_quotes(texPath.string());
		cmd += "')";
//		_impl->ignoreOutput = true;
		_impl->R->parseEvalNT(cmd);
		fs::path genPdfPath(scratchPath);
		genPdfPath /= baseName + ".pdf";
		fs::path destPdfPath(_impl->tmpDir->getPath());
		destPdfPath /= baseName + ".pdf";
		if (fs::exists(genPdfPath)) {
			fs::copy_file(genPdfPath, destPdfPath, fs::copy_option::overwrite_if_exists);
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
	return acknowledgeExecComplete(startTime, clientExtras);
}

void
RC2::RSession::addFileChangesToJson(JsonDictionary& json)
{
	std::vector<string> add, mod, del;
//	_impl->fileWatcher.stopWatch(add, mod, del);
	mod.insert(mod.end(), add.begin(), add.end()); //merge add/modified
	if (mod.size() > 0)
		json.addStringArray("filesModified", mod);
	if (del.size() > 0)
		json.addStringArray("filesDeleted", del);
}

void
RC2::RSession::clearFileChanges()
{
	RC2::JsonDictionary json;
	addFileChangesToJson(json);
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
		LOG(INFO) << "sending:" << json << endl;
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

string
RC2::RSession::acknowledgeExecComplete(string stime, json::UnknownElement *clientExtras) 
{
	RC2::JsonDictionary json;
	json.addString("msg", "execComplete");
	json.addString("startTime", stime);
	if (clientExtras)
		json.addObject("clientData", *clientExtras);
	addFileChangesToJson(json);
	return json;
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

