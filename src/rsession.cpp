#include <cstdlib>
#include <iostream>
#include "common/RC2Utils.hpp"
#include <tclap/CmdLine.h>
#include "RSession.hpp"
//#include "FormattedException.hpp"
#include <execinfo.h>
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include <regex>
#include "RSessionCallbacks.hpp"
#include "InputBufferManager.hpp"
#include <RInside.h>
#include <event2/buffer.h>

using namespace std;
namespace fs = boost::filesystem;

const uint32_t kRSessionMagicNumber = 0x21;
extern Rboolean R_Visible;

static string escape_quotes(const string before);
static string formatErrorAsJson(int errorCode, string details);


RC2::RSession::RSession(RSessionCallbacks *callbacks)
{
	std::cerr << "RSession created" << endl;
//	_dataInProgress = nullptr;
//	_clientSource = nullptr;
	_socket = 0;
	_open = false;
	_ignoreOutput = false;
	_watchVariables = false;
	_sourceInProgress = false;
	_callbacks = callbacks;
	//TODO: set callbacks output lambda
}

RC2::RSession::~RSession()
{
//	if (_dataInProgress)
//		dispatch_release(_dataInProgress);
	if (_R) {
		_R->parseEvalQNT("save.image()");
		delete _R;
		_R = nullptr;
	}
	if (_inputBuffer)
		delete _inputBuffer;
//	if (_fileWatcher) {
//		delete _fileWatcher;
//		_fileWatcher = nullptr;
//	}
//	if (_outFunction)
//		Block_release(_outFunction);
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
		_socket = portArg.getValue();
		_verbose = switchArg.getValue();
		
	} catch (TCLAP::ArgException &e) {
		std::cerr << "error:" << e.error() << endl;
	}
	return true;
}

void
RC2::RSession::installExitHandler(event_callback_fn callback)
{
}

void
RC2::RSession::prepareForRunLoop()
{
	struct event_config *config = event_config_new();
	event_config_require_features(config, EV_FEATURE_FDS);
	_eventBase = event_base_new_with_config(config);
	event_config_free(config);
	_inputBuffer = new InputBufferManager();
	evutil_make_socket_nonblocking(_socket);
	_eventBuffer = bufferevent_socket_new(_eventBase, _socket, 0);
	if (_eventBuffer == nullptr) {
		cerr << "failed to create bufferevent socket" << endl;
		return;
	}
	bufferevent_enable(_eventBuffer, EV_READ|EV_WRITE);
	bufferevent_setcb(_eventBuffer,
		[](struct bufferevent *bev, void *ctx) {
			RSession *me = static_cast<RSession*>(ctx);
			me->_inputBuffer->appendData(bufferevent_get_input(bev));
			if (me->_inputBuffer->hasCompleteMessage()) {
				me->handleJsonCommand(me->_inputBuffer->popCurrentMessage());
			}
		},
		nullptr,
		nullptr,
		this);
}

void
RC2::RSession::handleJsonCommand(string json)
{
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
}

void
RC2::RSession::handleOpenCommand(string arg)
{
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
	
	if (_sourceInProgress) {
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

