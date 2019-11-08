#include <cstdlib>
#include <iostream>
#include <tclap/CmdLine.h>
#include <execinfo.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cassert>
#include <boost/log/utility/setup/file.hpp>
#include <boost/algorithm/string.hpp>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <nlohmann/json.hpp>
#include "json-schema.hpp"
#include "RC2Logging.h"
#include <Rcpp.h>
#include <RInside.h>
#include <event2/buffer.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include "common/RC2Utils.hpp"
#include "common/PGDBConnection.hpp"
#include "RSession.hpp"
#include "RSessionCallbacks.hpp"
#include "InputBufferManager.hpp"
//#include "FormattedException.hpp"
#include "JsonCommand.hpp"
#include "common/RC2Utils.hpp"
#include "common/ZeroInitializedStruct.hpp"
#include "common/FormattedException.hpp"
#include "FileManager.hpp"
#include "EnvironmentWatcher.hpp"

using namespace std;
namespace fs = boost::filesystem;
using json2 = nlohmann::json;
using Rcpp::Environment;

namespace RC2 {
    typedef map<long, unique_ptr<EnvironmentWatcher>> EnvironmentMap;
}

extern Rboolean R_Visible;

enum KnitExceptionCode { none, forkError=100, execvError, pathError, pdfError, errorInLogFile, unknownError };
enum PdfStatus { successPdfStatus, killedPdfStatus, otherPdfStatus };

int clientErrorForKnitExceptionCode(KnitExceptionCode code) {
	switch (code) {
		case forkError:
		case execvError:
		case pathError:
		case pdfError:
			return kError_SweaveError;			
		case errorInLogFile:
			return kError_SweaveErrorInLogFile;
		case unknownError:
		default:
			return kError_Unknown;
	}
}

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details, int queryId=0);
static void rc2_log_callback(int severity, const char *msg);

inline bool stringHasPrefix(string str, const char* prefix) {
	return strncmp(str.c_str(), prefix, strlen(prefix)) == 0;
}

inline double currentFractionalSeconds() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + (round(tv.tv_usec/1000.0)/1000.0);
}

namespace RC2 {

struct ExecCompleteArgs {
	RSession *session;
	JsonCommand command;
	FileInfo finfo;
	int queryId;
	//object ptr points to does not have to exist past this call. just to allow null value
	ExecCompleteArgs(RSession *inSession, JsonCommand inCommand, int inQueryId, FileInfo *info) 
		: session(inSession), queryId(inQueryId), command(inCommand), finfo(info)
	{}
};

struct DelayCommandArgs {
	RSession *session;
	string json;
	DelayCommandArgs(RC2::RSession *inSession, string inJson)
		: session(inSession), json(inJson)
	{}
};

struct BoolResetter {
	BoolResetter(bool *ptr, bool valueToSet) { _ptr = ptr; _valToSet = valueToSet; *_ptr = valueToSet; }
	~BoolResetter() { *_ptr = !_valToSet; }
private:
	bool *_ptr;
	bool _valToSet;
};

struct SweavePdfData {
	JsonCommand command;
	RSession *session;
	pid_t pid;
	fs::path srcPath;
	fs::path scratchPath;
	string baseName;
	int queryId;
	PdfStatus pstatus = successPdfStatus;
	SweavePdfData (JsonCommand cmd, RSession *s, pid_t p, fs::path spath, fs::path tpath, string name, int qid = 0) 
	: command(cmd), session(s), pid(p), srcPath(spath), scratchPath(tpath), baseName(name), queryId(qid)
	{};
};

} // namespace

struct RC2::RSession::Impl : public ZeroInitializedStruct {
	struct event_base*				eventBase;
	struct bufferevent*				eventBuffer;
	struct evbuffer*				outBuffer;
	InputBufferManager				inputBuffer;
	RInside*						R;
	unique_ptr<FileManager>			fileManager;
	unique_ptr<TemporaryDirectory>	tmpDir;
	string							rc2home; // home directory. specified by RC2_HOME env var. Defaults to dir containing executable
//	unique_ptr<EnvironmentWatcher>	envWatcher;
    EnvironmentMap                  environments;
	int								environmentCounter;
	shared_ptr<string>				consoleOutBuffer;
	string							stdOutCapture;
	string							currentImageName; //watching to track title of it
	json							incomingJsonSchema;
	double							consoleLastWrite;
	int								apiVersion;
	int								wspaceId;
	int								sessionRecId;
	int								socket;
	int								currentQueryId;
	int								startDelay;
	bool							isOpen;
	bool							ignoreOutput;
	bool							captureStdOut;
	bool							sourceInProgress;
	bool							watchVariables;
	bool 							properlyClosed;

				Impl();
				Impl(const Impl &copy) = delete;
		Impl&	operator=(const Impl&) = delete;
		void	addImagesToJson(json2& json);
		string	acknowledgeExecComplete(JsonCommand &command, int queryId, bool expectShowOutput);
		EnvironmentWatcher*	env(long id);
		ExecuteCallback getExecuteCallback();
		/**
			 * @brief validates all received json against the configured json schema 
			 * @throws exceptions */
		void	validateIncomingJson(json jsonObj);


	static void handleExecComplete(int fd, short event_type, void *ctx) 
	{
		ExecCompleteArgs *args = reinterpret_cast<ExecCompleteArgs*>(ctx);
		bool gotFileInfo = args->finfo.id > 0;
		LOG(INFO) << "got ack with file " << gotFileInfo;
		string s = args->session->_impl->acknowledgeExecComplete(args->command, args->queryId, gotFileInfo);
		LOG(INFO) << "handleExecComplete got json:" << s;
		args->session->sendJsonToClientSource(s);
		if (gotFileInfo) {
			args->session->_impl->fileManager->fileInfoForId(args->finfo.id, args->finfo);
			json2 results = { 
				{"msg", "showoutput"}, 
				{"fileId", args->finfo.id}, 
				{"fileName", args->finfo.name}, 
				{"fileVersion", args->finfo.version},
				{"queryId", args->queryId} };
			args->session->sendJsonToClientSource(results.dump());
		}
		delete args;
	}
	
	static void handleDelayedCommand(int fd, short event_type, void *ctx) 
	{
		DelayCommandArgs *args = reinterpret_cast<DelayCommandArgs*>(ctx);
		args->session->handleJsonCommand(args->json);
		delete args;
	}
	
	
	class NotifySuspender {
		Impl& _impl;
	public:
		NotifySuspender(Impl &impl)
		:_impl(impl)
		{
			_impl.ignoreOutput = true;
		}
		
		~NotifySuspender()
		{
			_impl.ignoreOutput = false;
		}
		
	};
	
};


RC2::RSession::Impl::Impl()
	: consoleOutBuffer(new string)
{
	const char *envHome = getenv("RC2_HOME");
	if (envHome) {
		rc2home = envHome;
	} else {
		string installLoc = RC2::GetPathForExecutable(getpid());
		string::size_type parentDirIndex = installLoc.rfind('/');
		rc2home = installLoc.substr(0, parentDirIndex);
	}
}

RC2::ExecuteCallback RC2::RSession::Impl::getExecuteCallback()
{
	return [this](const string& cmd, RObject& result) {
		SEXP sr;
		bool ok = false;
		{
			BooleanWatcher bwatch(&ignoreOutput);
			ok = R->parseEval(cmd, sr) == 0;
		}
		if (sr)
			result = sr;
		return ok;
	};
}

RC2::EnvironmentWatcher * 
RC2::RSession::Impl::env(long id)
{
	if (environments.count(id) == 0) {
		// need to create one
		environments[id] = unique_ptr<EnvironmentWatcher>(new EnvironmentWatcher(Environment::global_env(), getExecuteCallback()));
	}
	return environments[id].get();
}

void 
RC2::RSession::Impl::validateIncomingJson(json jsonObj)
{
	if (incomingJsonSchema.is_null()) return;
 	assert(jsonObj.is_null() == false);
	nlohmann::json_schema::json_validator validator;
	try {
//		LOG(INFO) << "validating " << jsonObj.dump(4);
		validator.set_root_schema(incomingJsonSchema);
		validator.validate(jsonObj);
//		LOG(INFO) << "incoming json validated" << endl;
	} catch (const std::exception &e) {
		LOG(WARNING) << "error validating json: " << e.what();
// TODO: fix validation error barfing on queryId
//		string errStr = "error validationg incoming json: ";
//		errStr += e.what();
//		throw GenericException(errStr);
	}
}

void
RC2::RSession::Impl::addImagesToJson(json2& json)
{
	std::vector<long> imageIds;
	long batchId;
	fileManager->checkWatch(imageIds, batchId);
	if (imageIds.size() > 0)
		json["images"] = imageIds;
	if (batchId > 0)
		json["imgBatch"] = batchId;
}

pid_t
RC2::RSession::Impl::knit(string texPath)
{
	char exe[32];
	strncpy(exe, "/usr/bin/texi2pdf", 32);
	char path[2048];
	if (texPath.length() > sizeof(path)) 
		throw GenericException("texPath too long", pathError);
	strncpy(path, texPath.c_str(), sizeof(path));
	char batchFlag[4];
	strncpy(batchFlag, "-b", 4);
	char *args[] = {exe, batchFlag, path, NULL};
	pid_t pid = fork();
	if (pid == -1) {
		LOG(WARNING) << "fork() failed for texi2dvi";
		throw FormattedException("fork() failed: %i", errno).code(forkError);
	} else if (pid > 0) {
		return pid;
	} else {
		// setup our atdin, stdout, stderr
		int oldStdErr = dup(2);
		FILE *parentErr = fdopen(oldStdErr, "w");
		int devnull = open("/dev/null", O_WRONLY);
		dup2(devnull, 1);
		dup2(devnull, 2);
		close(0);
		int rc = execve("/usr/bin/texi2pdf", args, environ);
		dprintf(oldStdErr, "sweave execve returned %d", rc);
		throw FormattedException("sweave execve returned %d", rc).code(execvError);
	}
}

void 
RC2::RSession::Impl::handleSweaveOutput(SweavePdfData *data)
{
	switch (data->pstatus) {
		case otherPdfStatus:
		data->session->sendJsonToClientSource(formatErrorAsJson(kError_SweaveError, "unknown error generating pdf", data->queryId));
			return;
		case killedPdfStatus:
			data->session->sendJsonToClientSource(formatErrorAsJson(kError_SweaveError, "error generating pdf", data->queryId));
			return;
		case successPdfStatus:
			break;
	}
	FileInfo finfo;
	string basePdfName = data->baseName + ".pdf";
	fs::path genPdfPath(data->scratchPath);
	genPdfPath /= basePdfName;
	fs::path destPdfPath(tmpDir->getPath());
	destPdfPath /= basePdfName;
	boost::system::error_code ec;
	if (fs::exists(genPdfPath) && fs::file_size(genPdfPath, ec) > 0) {
		fs::copy_file(genPdfPath, destPdfPath, fs::copy_option::overwrite_if_exists);
		fileManager->findOrAddFile(basePdfName, finfo);
	} else {
		LOG(INFO) << "genPdfPath empty:" << genPdfPath;
		// look for error file
		string logName = data->baseName + ".log";
		fs::path errorPath(data->scratchPath);
		errorPath /= logName;
		if (fs::exists(errorPath)) {
			fs::path logPath(data->srcPath.parent_path());
			logPath /= logName;
			fs::copy_file(errorPath, logPath, fs::copy_option::overwrite_if_exists);
			throw GenericException(logName, errorInLogFile);
		}
		//report error with no details
		data->session->sendJsonToClientSource(formatErrorAsJson(kError_SweaveError, "unknown error generating pdf", data->queryId));
		return;
	}
	{
		BoolResetter reset(&ignoreOutput, true);
		string rcmd = "setwd('" + escape_quotes(data->scratchPath.parent_path().string()) + "')";
		R->parseEvalNT(rcmd);
	}
	data->session->flushOutputBuffer();
	data->session->scheduleExecCompleteAcknowledgment(data->command, data->queryId, &finfo);

}


string
RC2::RSession::Impl::acknowledgeExecComplete(JsonCommand& command, int queryId, bool expectShowOutput) 
{
	LOG(INFO) << "exec complete posting";
	fileManager->cleanupImageWatch();
	json2 results;
	results["msg"] = "execComplete";
	results["startTime"] = command.startTimeStr();
	if (queryId > 0)
		results["queryId"] = queryId;
	if (!command.clientData().is_null())
		results["clientData"] = command.clientData();
	results["expectShowOutput"] = expectShowOutput;
	addImagesToJson(results);
	return results.dump();
}


void
RC2::RSession::scheduleExecCompleteAcknowledgment(JsonCommand& command, int queryId,
	FileInfo* info)
{
	ExecCompleteArgs *args = new ExecCompleteArgs(this, command, queryId, info);
	struct timeval delay = {0, 1};
	struct event *ev = event_new(_impl->eventBase, -1, 0, 	RC2::RSession::Impl::handleExecComplete, args);
	event_priority_set(ev, 0);
 	event_add(ev, &delay);
}

void
RC2::RSession::scheduleDelayedCommand(string json)
{
	DelayCommandArgs *args = new DelayCommandArgs(this, json);
	struct timeval delay = {0, 1};
	struct event *ev = event_new(_impl->eventBase, -1, 0, 	RC2::RSession::Impl::handleDelayedCommand, args);
	event_priority_set(ev, 0);
	event_add(ev, &delay);
}

RC2::RSession::RSession(RSessionCallbacks *callbacks)
		: _impl(new Impl())
{
	_callbacks = callbacks;
	setenv("R_HOME", "/usr/local/lib/R", 0); //will not overwrite
	_impl->R = new RInside(0, NULL, false, true, false);
	auto clambda = [&](const string &text, bool is_error) -> void {
		consoleCallback(text, is_error);
	};
	_callbacks->_writeLambda = clambda;
	_impl->R->set_callbacks(_callbacks);
	if (!_impl->fileManager) { // might have been set by subclass
		unique_ptr<FileManager> fm(new FileManager());
		_impl->fileManager = std::move(fm);
	}
	// initialize json schema
	string incomingPath = incomingJsonSchemaPath();
	string incomingSchemaString = SlurpFile(incomingPath.c_str());
	auto schema = json::parse(incomingSchemaString, nullptr, false);
	_impl->incomingJsonSchema = schema;
}

RC2::RSession::~RSession()
{
	if (_impl->R) {
		if (!_impl->properlyClosed) {
			LOG(WARNING) << "session not properly closed" << std::endl;
			handleCloseCommand();
		}
		delete _impl->R;
		_impl->R = nullptr;
	}
	if (nullptr != _impl->outBuffer)
		evbuffer_free(_impl->outBuffer);
	LOG(INFO) << "RSession destroyed";
}

RInside*
RC2::RSession::getRInside() const
{
	return _impl->R;
}

void
RC2::RSession::consoleCallback(const string &text, bool is_error)
{
//	LOG(INFO) << "callback:" << text;
	if (!_impl->isOpen && !is_error) return;
	if (stringHasPrefix(text, "rc2.imgstart=")) {
		auto imgname = text.substr(13);
		boost::algorithm::trim(imgname);
		_impl->currentImageName = imgname;
		return;
	} else if (stringHasPrefix(text, "rc2.imgtitle=")) {
		auto imgtitle = text.substr(13);
		boost::algorithm::trim(imgtitle);
		_impl->fileManager->setTitle(imgtitle, _impl->currentImageName);
		_impl->currentImageName = "";
		return;
	}
	if (getenv("RC2_LOG_RSTDOUT")) {
        LOG(INFO) << "write cb: " << text <<  "(ignore=" << _impl->ignoreOutput << ",vis=" << R_Visible << ",sip=" << _impl->sourceInProgress << ",err=" << is_error << ")";
    }
    if (_impl->captureStdOut) {
		_impl->stdOutCapture += text;
		return;
	}
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
		
		TCLAP::ValueArg<int> delayArg("d", "delay", "seconds to delay start of event loop",
									  false, 0, "integer", cmdLine);
		
		TCLAP::SwitchArg switchArg("v", "verbose", "enable logging", cmdLine);
			
		cmdLine.parse(argc, argv);
		_impl->socket = portArg.getValue();
		_impl->startDelay = delayArg.getValue();
		bool verbose = switchArg.getValue();
		if (verbose) {
			setenv("GLOG_minloglevel", "1", 1);
		}
		
		LOG(INFO) << "arguments parsed\n";
		
	} catch (TCLAP::ArgException &e) {
		LOG(FATAL) << "error:" << e.error();
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
			LOG(WARNING) << "failed to create bufferevent socket";
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
	_impl->fileManager->setEventBase(_impl->eventBase);
}

void
RC2::RSession::startEventLoop()
{
	if (_impl->startDelay > 0) {
		sleep(_impl->startDelay);
		LOG(INFO) << "sleep over";
	}
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
	try {
		RC2::RSession *me = static_cast<RC2::RSession*>(ctx);
		me->_impl->inputBuffer.appendData(bufferevent_get_input(bev));
		if (me->_impl->inputBuffer.hasCompleteMessage()) {
			me->handleJsonCommand(me->_impl->inputBuffer.popCurrentMessage());
		}
	} catch (exception const& e) {
		LOG(WARNING) << "exception in handleJsonStatic:" << e.what();
	}
}

void
RC2::RSession::handleCommand(JsonCommand& command)
{
	if (!_impl->isOpen ) {
		LOG(WARNING) << "R not open";
		return;
	}
	_impl->currentQueryId = command.raw().value("queryId", 0);
	switch(command.type()) {
		case CommandType::Close:
			handleCloseCommand();
			break;
		case CommandType::ClearFileChanges:
			_impl->fileManager->resetWatch(); //clears cache
			break;
		case CommandType::ExecScript:
			handleExecuteScript(command);
			break;
		case CommandType::ExecAndReturn:
			handleExecuteScript(command);
			break;
		case CommandType::ExecFile:
			executeFile(command);
			break;
		case CommandType::Help:
			handleHelpCommand(command);
			break;
		case CommandType::ListVariables:
			handleListVariablesCommand(command.raw().value("delta", false), command);
			break;
		case CommandType::GetVariable:
			handleGetVariableCommand(command);
			break;
		case CommandType::ToggleWatch:
			_impl->watchVariables = command.raw().value("watch", false);
			if (_impl->watchVariables)
				handleListVariablesCommand(false, command);
			break;
		case CommandType::SaveData:
			handleSaveEnvCommand();
			break;
		case CommandType::ClearEnvironment:
			handleClearEnvironment(command);
			break;
		case CommandType::CreateEnvironment:
			handleCreateEnvironment(command);
			break;
		default:
			LOG(WARNING) << "unknown command type";
			break;
	}
}

void
RC2::RSession::handleJsonCommand(string json)
{
	try {
		if (json.length() < 1)
			return;
		LOG(INFO) << "json=" << json;
		json2 doc;
		try {
			std::istringstream istr(json);
			istr >> doc;
		} catch (std::invalid_argument &iae) {
			LOG(WARNING) << "parse exception:" << iae.what();
			return;
		} catch (std::exception &ex) {
			LOG(WARNING) << "unknown exception parsing:" << ex.what();
		}
		_impl->validateIncomingJson(doc);
		JsonCommand command(doc);
		if (command.type() == CommandType::Open) {
			if (_impl->isOpen ) {
				LOG(WARNING) << "duplicate open message received";
				return;
			}
			handleOpenCommand(command);
		} else {
			handleCommand(command);
		}
	} catch (std::runtime_error error) {
		LOG(WARNING) << "handleJsonCommand error: " << error.what();
		sendJsonToClientSource(formatErrorAsJson(kError_ExecFile_MarkdownFailed, error.what(), true));
	}
	_impl->currentQueryId = 0;
}

void
RC2::RSession::handleOpenCommand(JsonCommand &cmd)
{
	if (_impl->isOpen ) {
		LOG(WARNING) << "duplicate open message received";
		return;
	}
	LOG(INFO) << cmd.raw() << endl;
	_impl->wspaceId = cmd.raw()["wspaceId"];
	_impl->apiVersion = cmd.apiVersion();
	try {
		string swizzlePath = _impl->rc2home + "/rsrc/swizzle.R";

		_impl->sessionRecId = cmd.raw()["sessionRecId"];
		string dbhost(cmd.valueForKey("dbhost"));
		string dbuser(cmd.valueForKey("dbuser"));
		string dbname(cmd.valueForKey("dbname"));
		string dbport(cmd.valueForKey("dbport"));
		string dbpassword(cmd.valueForKey("dbpassword"));
		if (dbport.length() < 4) { dbport = "5432"; }
		ostringstream connectString;
		connectString << "postgresql://" << dbuser << "@" << dbhost << ":" << dbport << "/" 
			<< dbname << "?application_name=rsession&sslmode=disable";
		if (dbpassword.length() > 0)
			connectString << "&password=" << dbpassword;
		LOG(INFO) << connectString.str();
		
		
		if (NULL ==  _impl->eventBase) {
			LOG(WARNING) << "handleOpenCommand called before prepareForRunLoop()";
			abort();
		}
		string workDir = "/tmp/" + GenerateUUID();
		int rc = RC2::MakeDirectoryPath(workDir, 0755);
		if (rc != 0) {
			throw std::runtime_error("failed to create working directory");
		}
		_impl->tmpDir = std::move(std::unique_ptr<TemporaryDirectory>(new TemporaryDirectory(workDir, false)));
		LOG(INFO) << "wd=" << _impl->tmpDir->getPath();
		auto connection = make_shared<PGDBConnection>();
		connection->connect(connectString.str());
		setenv("TMPDIR", workDir.c_str(), 1);
		setenv("TEMP", workDir.c_str(), 1);
		setenv("R_DEFAULT_DEVICE", "png", 1);
		_impl->R->parseEvalQNT("setwd(\"" + escape_quotes(workDir) + "\")");
		_impl->ignoreOutput = true;
		_impl->R->parseEvalQNT("library(rmarkdown)");
		_impl->R->parseEvalQNT("library(tools)");
		_impl->R->parseEvalQNT("library(rc2, quietly = TRUE)");
		_impl->R->parseEvalQNT("rm(argv)"); //RInside creates this even though we passed NULL
		_impl->R->parseEvalQNT("options(device = \"rc2.pngdev\", bitmapType = \"cairo\")");
		_impl->R->parseEvalQNT("source(\"" + escape_quotes(swizzlePath) + "\", keep.source=FALSE)");
		_impl->fileManager->initFileManager(workDir, connection, _impl->wspaceId, _impl->sessionRecId);
		bool haveRData = _impl->fileManager->loadRData();
		if (haveRData) {
			LOG(G3LOG_DEBUG) << "loading .RData";
			_impl->R->parseEvalQNT("load(\".RData\")");
		}
		_impl->ignoreOutput = false;
		json2 response =  { {"msg", "openresponse"}, {"success", true} };
		sendJsonToClientSource(response.dump());
		_impl->isOpen = true;
	} catch (std::runtime_error &err) {
		json2 error = { {"msg", "openresponse"}, {"success", false}, {"errorMessage", err.what()} };
		sendJsonToClientSource(error.dump());
		// close self since not able to connect
		json2 closeMsg = { {"msg", "close"} };
		scheduleDelayedCommand(closeMsg.dump());
	}
}

void 
RC2::RSession::handleCloseCommand()
{
	if (_impl->properlyClosed) {
		LOG(WARNING) << "duplicate handleCloseCommand" << std::endl;
		return;
	}
	_impl->properlyClosed = true;
	handleSaveEnvCommand();
	event_base_loopbreak(_impl->eventBase);
}

void 
RC2::RSession::handleSaveEnvCommand()
{
	LOG(INFO) << "saving .RData" << std::endl;
//	BooleanWatcher watch(&_impl->ignoreOutput);
	_impl->R->parseEvalQNT("save.image()");
	_impl->fileManager->saveRData();
}

void
RC2::RSession::handleClearEnvironment(RC2::JsonCommand& command)
{
	bool watching = _impl->watchVariables;
	long envId = command.envId();
	if (envId == 0) {
		// clear the global environment
		SEXP ans=NULL;
		bool oldIgnore = _impl->ignoreOutput;
		_impl->ignoreOutput = true;
		_impl->R->parseEvalR("rm(list=ls())", ans);
		_impl->ignoreOutput = oldIgnore;
	} else {
		_impl->environments.erase(envId);		
	}
	if (watching) {
		auto envWatcher = _impl->env(command.envId());
		json2 results = {
			{"msg", "variableupdate"},
			{"delta", false}
		};
		envWatcher->addVariables(results, false, _impl->apiVersion);
		if (!command.clientData().is_null())
			results["clientData"] = command.clientData();
		sendJsonToClientSource(results.dump());
	}
}

void
RC2::RSession::handleCreateEnvironment(RC2::JsonCommand& command)
{
	string transId = command.argument();
	if (transId.length() < 1) {
		//error
		sendJsonToClientSource(formatErrorAsJson(kError_MissingTransactionId, "missing transactionId", 0));
		return;
	}
	int newId = _impl->environmentCounter;
	while (_impl->environments.count(newId) > 0) {
		newId++;
	}
	_impl->environmentCounter += 1;
	_impl->environments[newId] = unique_ptr<EnvironmentWatcher>(new EnvironmentWatcher(Environment::global_env(), getExecuteCallback()));
	json results = { { "msg", "envCreated" }, {"contextId", newId }, {"transactionId", transId} };
	sendJsonToClientSource(results.dump()
		
	);
}


void
RC2::RSession::handleExecuteScript(JsonCommand& command) {
	LOG(INFO) << "exec:" << command.argument();
	bool sendDelta = _impl->watchVariables || command.watchVariables();
	auto envWatcher = _impl->env(command.envId());
	if (sendDelta) {
		LOG(INFO) << " watching for changes";
		envWatcher->captureEnvironment();
	}
	_impl->fileManager->resetWatch();
	SEXP ans=NULL;
	RInside::ParseEvalResult result = _impl->R->parseEvalR(command.argument(), ans, envWatcher->getEnvironment());
	LOG(INFO) << "parseEvalR returned " << (ans != NULL);
	flushOutputBuffer();
	if (result == RInside::ParseEvalResult::PE_SUCCESS) {
		      scheduleExecCompleteAcknowledgment(command, _impl->currentQueryId);
		if (sendDelta) {
			LOG(INFO) << "scheduling list variables";
			json2 msg = { {"msg", "listVariables"}, {"delta", true}, {"contextId", command.envId()} };
			scheduleDelayedCommand(msg.dump());
		}
	} else if (result == RInside::ParseEvalResult::PE_INCOMPLETE) {
		sendTextToClient("Incomplete R statement\n", true);
	}
}

void
RC2::RSession::handleExecuteAndReturn(RC2::JsonCommand& command)
{
	LOG(INFO) << "execAndReturn:" << command.argument();
	
}

void
RC2::RSession::handleListVariablesCommand(bool delta, JsonCommand& command)
{
	auto envWatcher = _impl->env(command.envId());
	json2 results = {
		{"msg", "variableupdate"},
		{"delta", delta},
		{"contextId", command.envId()}
	};
	envWatcher->addVariables(results, false, _impl->apiVersion);
	if (!command.clientData().is_null())
		results["clientData"] = command.clientData();
	sendJsonToClientSource(results.dump());
}

void
RC2::RSession::handleGetVariableCommand(JsonCommand &command)
{
	json2 value = _impl->env(command.envId())->toJson(command.argument());
	LOG(INFO) << "get variable:" <<command.argument();
	json2 results = {
		{"msg", "variablevalue"},
		{"name", command.argument()},
		{"value", value},
		{"contextId", command.envId()},
		{"startTime", command.startTimeStr()}
	};
	if (!command.clientData().is_null())
		results["clientData"] = command.clientData();
	sendJsonToClientSource(results.dump());
}

void
RC2::RSession::handleHelpCommand(JsonCommand& command)
{
	string helpCmd("help(\"" + escape_quotes(command.argument()) + "\")");
	LOG(INFO) << "help:" << helpCmd;
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
		LOG(WARNING) << "help got error:" << err.what();
	}
	_impl->ignoreOutput = false;
}

void
RC2::RSession::executeFile(JsonCommand& command) {
	long fileId = atol(command.argument().c_str());
	string fpath;
	if (!_impl->fileManager->filePathForId(fileId, fpath)) {
		ostringstream errbuff;
		errbuff << "unknown file:" << fileId;
		sendJsonToClientSource(formatErrorAsJson(kError_UnknownFile, errbuff.str(), _impl->currentQueryId));
		return;
	}
	fs::path p(fpath);
	clearFileChanges();
	_impl->fileManager->resetWatch();
	if (_impl->watchVariables)
		_impl->env(command.envId())->captureEnvironment();
	if (p.extension() == ".Rmd") {
		executeRMarkdown(fpath, fileId, command);
	} else if (p.extension() == ".Rnw") {
		executeSweave(fpath, fileId, command);
	} else if (p.extension() == ".R") {
		string rcmd = "source(file=\"" + escape_quotes(fpath) + "\", echo=TRUE)";
		LOG(INFO) << "executing:" << rcmd;
		_impl->sourceInProgress = true;
		_impl->R->parseEvalQNT(rcmd);	
		flushOutputBuffer();
		_impl->sourceInProgress = false;
		      scheduleExecCompleteAcknowledgment(command, _impl->currentQueryId);
		if (_impl->watchVariables)
			scheduleDelayedCommand("{\"msg\":\"listVariables\", \"delta\":true}");
	} else {
		string errMsg = "unsupported file type:" + fpath;
		sendJsonToClientSource(formatErrorAsJson(kError_Execfile_InvalidInput, errMsg, _impl->currentQueryId));
	}
}

void
RC2::RSession::executeRMarkdown(string fileName, long fileId, JsonCommand& command)
{
	TemporaryDirectory tmp(false);
	bool fileGenerated = false;

	fs::path origFileName(fileName);
	string htmlName = origFileName.stem().native() + ".html";
	fs::path htmlPath = _impl->tmpDir->getPath();
	htmlPath /= htmlName;
	fs::path srcPath(_impl->tmpDir->getPath());
	srcPath /= fileName;
	fs::path destPath(tmp.getPath());
	destPath /= fileName;
	fs::path tmpHtmlPath(tmp.getPath());
	tmpHtmlPath /= htmlName;

	fs::copy_file(srcPath, destPath, fs::copy_option::overwrite_if_exists);
	string rcmd = "setwd('" + tmp.getPath() + "');render(\"" + escape_quotes(fileName) + "\", output_format=html_document());";
	fs::path opath(_impl->tmpDir->getPath());
	opath /= origFileName;
	bool generatedHtml = false;
	{
		Impl::NotifySuspender suspender(*_impl);
		_impl->captureStdOut = true;
		_impl->captureStdOut = "";
		LOG(INFO) << "executing: " << rcmd;
		_impl->R->parseEvalQNT(rcmd);
		_impl->captureStdOut = false;
		flushOutputBuffer();
		if (fs::exists(tmpHtmlPath)) {
			generatedHtml = true;
			fs::copy_file(tmpHtmlPath, htmlPath, fs::copy_option::overwrite_if_exists);
		} else {
			LOG(WARNING) << "Rmd failed to generate html:" << _impl->stdOutCapture;
		}
	}
	//reset working directory
	rcmd = "setwd(\"" + _impl->tmpDir->getPath() + "\");options(dev=\"rc2.pngdev\", bitmapType=\"cairo\")";
	_impl->R->parseEvalQNT(rcmd);
		
	boost::system::error_code ec;
	fileGenerated = fs::exists(htmlPath) && fs::file_size(htmlPath, ec) > 0;
	if (generatedHtml && fileGenerated) {
		Impl::NotifySuspender suspender(*_impl);
		FileInfo finfo;
		_impl->fileManager->findOrAddFile(htmlName, finfo);
		      scheduleExecCompleteAcknowledgment(command, _impl->currentQueryId, &finfo);
	} else {
		sendJsonToClientSource(formatErrorAsJson(kError_ExecFile_MarkdownFailed, "failed to generate html:" + _impl->stdOutCapture, _impl->currentQueryId));
	}
}

void
RC2::RSession::executeSweave(string filePath, long fileId, JsonCommand& command)
{
	BoolResetter reset(&(_impl->ignoreOutput), true);
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
		      scheduleExecCompleteAcknowledgment(command, _impl->currentQueryId);
		return;
	}
	fs::path texPath(scratchPath);
	texPath /= baseName + ".tex";
	sigset_t blockMask, emptyMask;
	try {
		SignalSuspender(SIGCHLD);
	if (!fs::exists(texPath)) {
		//there was an error
			throw GenericException("sweave failed", pdfError);
		}
		pid_t parent = getpid();
		pid_t pid;
		{
			pid = _impl->knit(texPath.native());
			LOG(INFO) << "launched sweave: " << pid;
		}
		// start timer to kill pid if takes too long
		auto callback = [](evutil_socket_t fd, short flags, void *param) {
			pid_t *pid = (pid_t*)param;
			LOG(INFO) << "timeout on sweave process: " << *pid;
			kill(*pid, SIGKILL);
		};
		struct timeval timeoutInterval = {5, 0};
		struct event *timerEvt =  event_new(_impl->eventBase, -1, EV_TIMEOUT, callback, &pid);
		event_add(timerEvt, &timeoutInterval);
		// and child signal handler
		// callback when a SIGCHLD is received
		auto sigCallback = [](int signal, short flags, void *param) {
			SweavePdfData *data = (SweavePdfData*)param;
			int status;
			int childPid = waitpid(data->pid, &status, 0);
			assert(childPid == data->pid);
			PdfStatus pstat = successPdfStatus;
			if (WIFSIGNALED(status)) {
				int sigStatus = WTERMSIG(status);
				LOG(WARNING) << "pdf generation received signal: " << sigStatus;
				pstat = killedPdfStatus;
			} else if (!(WIFEXITED(status))) {
				LOG(WARNING) << "waitpid returned abnormal status: " << status;
				pstat = otherPdfStatus;
			}
			data->session->_impl->handleSweaveOutput(data);
			delete data;
		};
		SweavePdfData *sigData = new SweavePdfData (command, this, pid, srcPath, scratchPath, baseName, _impl->currentQueryId);
		struct event *sigEvt = event_new(_impl->eventBase, SIGCHLD, EV_SIGNAL, sigCallback, sigData);
		event_add(sigEvt, NULL);
	} catch (GenericException &ge) {
		int clientErrorCode = clientErrorForKnitExceptionCode((KnitExceptionCode)ge.code());
		LOG(WARNING) << "error generating sweave pdf:" << ge.code() << " : " << ge.what();
		sendJsonToClientSource(formatErrorAsJson(clientErrorCode, ge.what(), _impl->currentQueryId));
	} catch (std::exception &e) {
		LOG(WARNING) << "unknown exception generating sweave pdf: " << e.what();
		sendJsonToClientSource(formatErrorAsJson(kError_Unknown, e.what(), _impl->currentQueryId));
	}
}

void
RC2::RSession::clearFileChanges()
{
	json2 ignoredJson;
	_impl->addImagesToJson(ignoredJson);
	_impl->currentImageName = "";
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
		LOG(INFO) << "sending:" << json << "(" << json.length() << " bytes)";
		int32_t header[2];
		header[0] = kRSessionMagicNumber;
		header[1] = json.length();
		header[0] = htonl(header[0]);
		header[1] = htonl(header[1]);
		evbuffer_add(_impl->outBuffer, &header, sizeof(header));
		evbuffer_add(_impl->outBuffer, json.c_str(), json.length());
		
		bufferevent_write_buffer(_impl->eventBuffer, _impl->outBuffer);
	} else {
		LOG(WARNING) << "output w/o client:" << json;
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
		{"is_error", is_error}
	};
	if (_impl->currentQueryId > 0)
		response["queryId"] = _impl->currentQueryId;
	return response.dump();
}

int
RC2::RSession::getApiVersion() const
{
	return _impl->apiVersion;
}

void
RC2::RSession::setApiVersion(int version) {
	assert(version >= 0 && version <= 1);
	_impl->apiVersion = version;
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

RC2::ExecuteCallback RC2::RSession::getExecuteCallback()
{
	 return _impl->getExecuteCallback();
}

const RC2::FileManager* RC2::RSession::getFileManager() const
{
	return _impl->fileManager.get();
}

void RC2::RSession::setFileManager ( RC2::FileManager* manager )
{
	unique_ptr<FileManager> fm(manager);
	_impl->fileManager = std::move(fm);
}

string 
RC2::RSession::incomingJsonSchemaPath()
{
	// when installed, should have beeen copied to home
	static const char *schemaFileName = "/compute.incoming.schema.json";
	fs::path filePath(_impl->rc2home);
	filePath /= schemaFileName;
	if (fs::exists(filePath)) return filePath.native();
	// if not copied, look for a checkout of the rc2.git project in rc2home
	filePath = _impl->rc2home;
	filePath /= "rc2root/documentation";
	filePath /= schemaFileName;
	// if doesn't exist here, we can't find it
	assert(fs::exists(filePath));
	return filePath.native();
}

bool 
RC2::RSession::loadEnvironment()
{
	return _impl->fileManager->loadRData();
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
	LOG(WARNING) << "logging " << severity << ":" << msg;
}

