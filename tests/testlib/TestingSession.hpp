#pragma once
#include <gtest/gtest.h>
#include <string>
#include <queue>
#include <event2/event.h>
#include <event2/thread.h>
#include "../../src/RSession.hpp"
#include "../../src/RSessionCallbacks.hpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

namespace RC2 {
	
	map<string,int64_t> mapArayToMapMap(json jarray);
	void myevent_logger(int severity, const char *msg);	
	
	class TestingSession : public RSession {
	public:
		using RSession::RSession;
		virtual void	sendJsonToClientSource(std::string json);
		
		bool fileExists(string filename);
		void copyFileToWorkingDirectory(string srcPath);
		
		void doJson(std::string json);
		void execScript(std::string rcode);
		
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json popMessage();
		
		void startCountdown(int count);
		void executeDelayedJson(string msg);
		
		event_base* cheatBase() { return getEventBase(); }
		queue<string> _messages;
		bool countingDown;
		int countDown;
	};	
}


namespace testing {
	using namespace RC2;
	
	class BaseSessionTest : public ::testing::Test {
	protected:
		static RSessionCallbacks *callbacks;
		static TestingSession *session;
		
		static void SetUpTestCase() {
			evthread_use_pthreads();
			callbacks = new RSessionCallbacks();
			session = new TestingSession(callbacks);
			event_set_log_callback(myevent_logger);
			session->prepareForRunLoop();
			session->doJson("{\"msg\":\"open\", \"argument\": \"\", \"wspaceId\":1, \"sessionRecId\":1, \"dbhost\":\"localhost\", \"dbuser\":\"rc2\", \"dbname\":\"rc2test\", \"dbpass\":\"rc2\"}");
			cerr << "SetupTestCase session open\n";
		}
		
		static void TearDownTestCase() {
			try {
				delete session;
				session = NULL;
				delete callbacks;
			} catch (...) {
			}
		}
		
		virtual void SetUp() {
			session->emptyMessages();
		}
		
		virtual void TearDown() {
		}
		
		virtual void pureVirtual() = 0;
	}; 
	
};
