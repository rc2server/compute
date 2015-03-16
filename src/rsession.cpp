#include <cstdlib>
#include <iostream>
#include <tclap/CmdLine.h>
#include <execinfo.h>
#include <sys/stat.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <regex>
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

using namespace std;
namespace fs = boost::filesystem;

extern Rboolean R_Visible;

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details);

struct RC2::RSession::Impl {
	event_base*					eventBase;
	struct bufferevent*			eventBuffer;
	struct evbuffer*			outBuffer;
	InputBufferManager			inputBuffer;
	RInside*					R;
	std::unique_ptr<TemporaryDirectory>	tmpDir;
//			KQueueFileWatcher*			_fileWatcher;
//			dispatch_io_t				_clientSource;
//			dispatch_io_t				_stdoutSource;
//			dispatch_io_t				_stderrSource;
//			dispatch_data_t				_dataInProgress;
//			dispatch_queue_t			_queue;
//	std::string					outBuffer;
//			OutputCallback				_outFunction; //if no _clientSource, this is used (likely for testing)
	int							socket;
	bool						open;
	bool						ignoreOutput;
	bool						sourceInProgress;
	bool						watchVariables;
};


RC2::RSession::RSession(RSessionCallbacks *callbacks)
		: _impl{}
{
	std::cerr << "RSession created" << endl;
//	_clientSource = nullptr;
	_callbacks = callbacks;
	//TODO: set callbacks output lambda
}

RC2::RSession::~RSession()
{
	if (_impl->R) {
		_impl->R->parseEvalQNT("save.image()");
		delete _impl->R;
		_impl->R = nullptr;
	}
//	if (_fileWatcher) {
//		delete _fileWatcher;
//		_fileWatcher = nullptr;
//	}
//	if (_outFunction)
//		Block_release(_outFunction);
	if (nullptr != _impl->outBuffer)
		evbuffer_free(_impl->outBuffer);
	_verbose && std::cerr << "RSession destroyed" << endl;
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
		_verbose = switchArg.getValue();
		
	} catch (TCLAP::ArgException &e) {
		std::cerr << "error:" << e.error() << endl;
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
	struct event_config *config = event_config_new();
	event_config_require_features(config, EV_FEATURE_FDS);
	_impl->eventBase = event_base_new_with_config(config);
	event_config_free(config);
	evutil_make_socket_nonblocking(_impl->socket);
	_impl->eventBuffer = bufferevent_socket_new(_impl->eventBase, _impl->socket, 0);
	if (_impl->eventBuffer == nullptr) {
		cerr << "failed to create bufferevent socket" << endl;
		return;
	}
	bufferevent_enable(_impl->eventBuffer, EV_READ|EV_WRITE);
	bufferevent_setcb(_impl->eventBuffer,
		[](struct bufferevent *bev, void *ctx) {
			RSession *me = static_cast<RSession*>(ctx);
			me->_impl->inputBuffer.appendData(bufferevent_get_input(bev));
			if (me->_impl->inputBuffer.hasCompleteMessage()) {
				me->handleJsonCommand(me->_impl->inputBuffer.popCurrentMessage());
			}
		},
		nullptr,
		nullptr,
		this);
	_impl->outBuffer = evbuffer_new();
}

void
RC2::RSession::startEventLoop()
{
	event_base_loop(_impl->eventBase, 0);
}

void
RC2::RSession::sendJsonToClientSource(string json)
{
	if (json.length() < 1)
		return;
	if (_impl->socket > 0) { //only send if we have a valid socket
		cerr << "sending:" << json << endl;
		int32_t header[2];
		header[0] = kRSessionMagicNumber;
		header[1] = json.length();
		header[0] = htonl(header[0]);
		header[1] = htonl(header[1]);
		evbuffer_add(_impl->outBuffer, &header, sizeof(header));
		evbuffer_add(_impl->outBuffer, json.c_str(), json.length());
		
		bufferevent_write_buffer(_impl->eventBuffer, _impl->outBuffer);
	} else {
		std::cerr << "output w/o client:" << json << endl;
	}
}

void
RC2::RSession::handleJsonCommand(string json)
{
	try {
		if (json.length() < 1)
			return;
		_verbose && std::cerr << "json=" << json << endl;
		json::Object doc;
		try {
			std::istringstream ist(json);
			json::Reader::Read(doc, ist);
		} catch (json::Reader::ParseException &pe) {
			std::cerr << "parse exception:" << pe.what() << endl;
			return;
		} catch (std::exception &ex) {
			std::cerr << "unknown exception parsing:" << ex.what() << endl;
		}
		string msg(((json::String)doc[string("msg")]).Value());
		string arg(((json::String)doc["argument"]).Value());
		if (msg == "open") {
			handleOpenCommand(arg);
		} else {
		}
	} catch (std::runtime_error error) {
		cerr << "handleJsonCommand error: " << error.what() << endl;
		sendJsonToClientSource(error.what());
	}
}

void
RC2::RSession::handleOpenCommand(string arg)
{
	bool outDir = arg.length() < 1;
	if (outDir)
		arg = "/tmp/" + RC2::GenerateUUID();
	_verbose && cerr << "got open message: " << arg << endl;
	struct stat sb;
	if (stat(arg.c_str(), &sb) == 0) {
		if (!S_ISDIR(sb.st_mode)) {
			//exists and not a directory
			throw (FormatErrorAsJson(kError_Open_InvalidDir, "invalid working directory"));
		}
	} else {
		//does not exist. try creating
		int rc = RC2::MakeDirectoryPath(arg, 0755);
		if (rc != 0) {
			throw (FormatErrorAsJson(kError_Open_CreateDirFailed, 
				"failed to create working directory"));
		}
	}
	_impl->tmpDir = std::move(std::unique_ptr<TemporaryDirectory>(new TemporaryDirectory(arg, outDir)));
	setenv("TMPDIR", arg.c_str(), 1);
	setenv("TEMP", arg.c_str(), 1);
	setenv("R_DEFAULT_DEVICE", "png", 1);
	_verbose && std::cerr << "setting wd" << endl;
	_impl->R->parseEvalQNT("setwd(\"" + escape_quotes(arg) + "\")");
	_impl->ignoreOutput = true;
	_impl->R->parseEvalQNT("library(rc2);");
	_impl->R->parseEvalQNT("library(knitr)");
	_impl->R->parseEvalQNT("library(markdown)");
	_impl->R->parseEvalQNT("library(tools)");
	_impl->R->parseEvalQNT("rm(argv)"); //RInside creates this even though we passed NULL
	_impl->ignoreOutput = false;
	_impl->R->parseEvalQNT("options(device = \"rc2.pngdev\")");
	sendJsonToClientSource("{\"msg\":\"opensuccess\"}");
	_impl->open = true;
	_verbose && cerr << "open done" << endl;
//	_fileWatcher = new KQueueFileWatcher();
//	_fileWatcher->initializeWatcher(_tmpDir->getPath()); 
}

void
RC2::RSession::addFileChangesToJson(JsonDictionary& json)
{
	std::vector<std::string> add, mod, del;
//	_fileWatcher->checkFiles(add, mod, del);
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

string
RC2::RSession::formatStringAsJson(string &input, bool is_error)
{
	if (input.length() < 1)
		return "";
	string output = input;
	
	if (_impl->sourceInProgress) {
		std::regex reg("\n\\$value\n[^]*?$");
		output = std::regex_replace(input, reg, std::string("\n"));
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
RC2::RSession::acknowledgeExecComplete(string stime, json::UnknownElement *clientExtras) {
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

