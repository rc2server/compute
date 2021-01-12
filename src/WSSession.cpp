#include "WSSession.hpp"
#include <tclap/CmdLine.h>
#include "RC2Logging.h"
#include "uWebSockets/src/App.h"

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

void
RC2::WSSession::prepareForRunLoop() {
	RSession::prepareForRunLoop();
	std::thread wsThread([this]{ startListening(); });
	wsThread.detach();
}

void
RC2::WSSession::startListening() {
	// api uses some kinda struct
	struct PerSocketData { };
	
	uWS::SSLApp({
		.key_file_name = "",
		.cert_file_name = "",
		.passphrase = ""
	}).ws<PerSocketData>("/*", {
		.compression = uWS::SHARED_COMPRESSOR,
		.maxPayloadLength = 16 * 1024,
		.idleTimeout = 3600,
		.maxBackpressure = 1 * 1024 * 1024,
		.open = [](auto *ws) {
		},
		.message = [this](auto *ws, std::string_view message, uWS::OpCode opCode) {
			if (opCode != uWS::TEXT) {
				LOG_ERROR << "ws received binary data, not supported";
				return;
			}
			string jsonStr(message);
			handleJsonCommand(jsonStr);
		},
		.drain = [](auto *ws) {
			/* Check ws->getBufferedAmount() here */
		},
		.ping = [](auto *ws) {
			/* Not implemented yet */
		},
		.pong = [](auto *ws) {
			/* Not implemented yet */
		},
		.close = [this](auto *ws, int code, std::string_view message) {
			handleCloseCommand(true);
			// TODO: exit this thread
		}
	}).listen(listenPort, [](auto *token) {
		if (token) {
			std::cout << "Listening on port " << 9001 << std::endl;
		}
	}).run();
}

