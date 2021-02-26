#include "WSSession.hpp"
#include <tclap/CmdLine.h>
#include <event2/event.h>
#include "RC2Logging.h"
#include "uWebSockets/src/App.h"
#include <event2/event.h>
#include <event2/event.h>
#include <event2/event.h>

struct WSData {
	std::function<bool(std::string)> sendMessage;
	bool connected = false;
};

RC2::WSSession::WSSession(RSessionCallbacks *callbacks)
	: RSession(callbacks)
{
}

RC2::WSSession::~WSSession() {
}

bool
RC2::WSSession::parseArguments(int argc, char *argv[]) {
	try {
		TCLAP::CmdLine cmdLine("Handle a remote R connection", ' ', "0.1");
		
		TCLAP::ValueArg<uint16_t> portArg("p", "port", "port webserver should listen on", 
									 true, -1, "http port", cmdLine);
				
		cmdLine.parse(argc, argv);
		listenPort = portArg.getValue();
		if (listenPort < 2000)
			LOG_FATAL << "invalid port number: " << listenPort;
	} catch (TCLAP::ArgException &e) {
		LOG_FATAL << "error:" << e.error();
	}
	return true;
}

void RC2::WSSession::sendJsonToClientSource ( std::__cxx11::string json ) {
	if (_logOutgoingJson)
		LOG_INFO << "sending: " << json;
	wsData->sendMessage(json);
}

static void dummy_signal_handler(int something, short unknown, void *arg) {
	LOG_INFO << "got usr2 signal";
}

void
RC2::WSSession::prepareForRunLoop() {
	RSession::prepareForRunLoop();
	std::thread wsThread([this]{ startListening(); });
	wsThread.detach();
	// listen for signal on main thread. need active event or loop ends immediately. 
	// normal version listens for socket. we need to listen for something else
	struct event *sigEvent;
	struct event_base *eb = getEventBase();
	sigEvent = evsignal_new(eb, SIGUSR2, dummy_signal_handler, NULL);
	event_add(sigEvent, NULL);
}

void
RC2::WSSession::startListening() {
	using std::string_view;
	using uWS::OpCode;
	// api uses some kinda struct
	wsData = std::make_unique<WSData>();
	auto thePort = listenPort;

//	LOG_INFO << "attempting to listen";
	// the websocket handler below supposidly can skip unused handlers (drain, ping, pong) 
	// but we get compile errors without them
	uWS::SSLApp({
		.key_file_name = "",
		.cert_file_name = "",
		.passphrase = ""
	}).ws<WSData>("/*", {
		.compression = uWS::SHARED_COMPRESSOR,
		.maxPayloadLength = 16 * 1024,
		.idleTimeout = 3600,
		.maxBackpressure = 1024 * 1024,
		.upgrade = [this](auto *res, auto *req, auto *context) {
			LOG_INFO << "ws upgrade";
			if (wsData->connected) {
				res->writeStatus("418 Teapot Already in Use");
				res->end();
				return;
			}
			res->upgrade(wsData.get(), 
			req->getHeader("sec-websocket-key"),
			req->getHeader("sec-websocket-protocol"),
			req->getHeader("sec-websocket-extensions"),
			context);
			wsData->connected = true;
		},
		.open = [this](auto *ws) {
			LOG_INFO << "ws open";
			wsData->sendMessage = [ws](std::string message) -> bool {
				// TODO: check return value
				if (ws->send(message, OpCode::TEXT) == false) {
					LOG_ERROR << "failed to send message via ws";
					return false;
				}
				return true;
			};
			wsData->connected = true;
		},
		.message = [this](auto *ws, string_view message, OpCode opCode) {
			LOG_INFO << "ws message";
//			if (opCode != OpCode::TEXT) {
//				LOG_ERROR << "ws received binary data, not supported";
//				return;
//			}
			string jsonStr(message);
			scheduleDelayedCommand(jsonStr);
		},
		.drain = [](auto *ws) {
		},
		.ping = [](auto *ws) {
		},
		.pong = [](auto *ws) {
		},
		.close = [this](auto *ws, int code, string_view message) {
			LOG_INFO << "ws close";
			handleCloseCommand(true);
		}
	}).listen(listenPort, [thePort](auto *token) {
		if (token) {
			LOG_INFO << "listening on port " << thePort;
		}
	}).run();
}
