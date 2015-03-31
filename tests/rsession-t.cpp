#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "common/RC2Utils.hpp"
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"

using std::cerr;
using std::endl;

namespace RC2 {

class TestingSession : public RSession {
	public:
		using RSession::RSession;
		virtual void	sendJsonToClientSource(std::string json);
		
		void doJson(std::string json);
};

void
TestingSession::sendJsonToClientSource(std::string json) {
	cerr << json << endl;
}

void
TestingSession::doJson(std::string json) {
	handleJsonCommand(json);
}


namespace testing {

	TEST(RSessionTest, basicScript)
	{
		RSessionCallbacks *callbacks = new RSessionCallbacks();
		TestingSession *session = new TestingSession(callbacks);
		session->doJson("{\"msg\":\"open\", \"argument\": \"\"}");

//		ASSERT_EQ(1, 2-1);
	}

};
};
