#pragma once

#include "RSession.hpp"
#include "uWebSockets/src/App.h"

struct WSData;

namespace RC2 {

	class WSSession: public RSession {
	public:
		explicit WSSession(RSessionCallbacks *callbacks);
		virtual ~WSSession();
		
		virtual bool parseArguments(int argc, char *argv[]) override;
		
		virtual void prepareForRunLoop() override;

		virtual void sendJsonToClientSource(string json) override;
	
	protected:
		std::unique_ptr<WSData> wsData;
		uint16_t listenPort;
		
		void startListening();
	};

}; //namespace
