#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include "common/RC2Utils.hpp"
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"

using namespace std;

namespace RC2 {

string
stringForJsonKey(json::Object &obj, string key) {
	return ((json::String)obj[key]).Value();
}

class TestingSession : public RSession {
	public:
		using RSession::RSession;
		virtual void	sendJsonToClientSource(std::string json);
		
		void doJson(std::string json);
		
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json::Object popMessage();
		
		queue<string> _messages;
};

void
TestingSession::sendJsonToClientSource(std::string json) {
	_messages.push(json);
//	cerr << json << endl;
}

json::Object
TestingSession::popMessage() {
	string json = _messages.front();
	json::Object doc;
	try {
		std::istringstream ist(json);
		json::Reader::Read(doc, ist);
	} catch (json::Reader::ParseException &pe) {
		cerr << "parse exception:" << pe.what() << endl;
	} catch (std::exception &ex) {
		cerr << "unknown exception parsing:" << ex.what() << endl;
	}
	return doc;
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

		session->emptyMessages();
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"2*2\"}");
		ASSERT_EQ(session->_messages.size(), 2);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "results");
		ASSERT_TRUE(stringForJsonKey(results1, "string") == "[1] 4\n");

//		ASSERT_EQ(1, 2-1);
	}

};
};
