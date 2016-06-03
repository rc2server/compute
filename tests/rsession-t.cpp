#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include "../src/RC2Logging.h"
#include "common/RC2Utils.hpp"
#include "json.hpp"
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestingSession.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = boost::filesystem;

namespace testing {
	class SessionTest : public BaseSessionTest {
		virtual void pureVirtual() {}
	};
	
	TEST_F(SessionTest, basicScript)
	{
		//need to delay action until after startEventLoop()
		std::thread t([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"2*2\"}");
		});
		t.detach();
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results1 = session->popMessage();
		ASSERT_TRUE(results1["msg"] == "results");
		ASSERT_TRUE(results1["string"] == "[1] 4\n");
	}

	TEST_F(SessionTest, execFiles)
	{
// 		std::thread t([]() {
// 			std::this_thread::sleep_for(std::chrono::milliseconds(1));
// 			session->doJson("{\"msg\":\"execFile\", \"argument\":\"3\"}");
// 		});
// 		t.detach();
		session->executeDelayedJson("{\"msg\":\"execFile\", \"argument\":\"3\"}");
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "results");
		ASSERT_TRUE(results["string"] == "\n> x <- 4\n\n> x * x\n[1] 16\n");
	}

	TEST_F(SessionTest, execRMD)
	{
		session->copyFileToWorkingDirectory("chapter.Rmd");
		fileManager->addFile(4, "chapter.Rmd", 1);
		session->executeDelayedJson("{\"msg\":\"execFile\", \"argument\":\"4\"}");
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results1 = session->popMessage();
		json results2 = session->popMessage();
		ASSERT_TRUE(results2["msg"] == "showoutput");
		ASSERT_TRUE(session->fileExists(results2["fileName"]));
	}

	TEST_F(SessionTest, execSweave)
	{
		session->executeDelayedJson("{\"msg\":\"execFile\", \"argument\":\"5\"}");
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results1 = session->popMessage();
		json results2 = session->popMessage();
		ASSERT_TRUE(results2["msg"] == "showoutput");
		ASSERT_TRUE(session->fileExists(results2["fileName"]));
	}

	TEST_F(SessionTest, genImages)
	{
		session->executeDelayedJson("{\"msg\":\"execScript\", \"argument\":\"plot(rnorm(21)); plot(rnorm(12))\"}");
		//need to delay action until after startEventLoop()
//		std::thread t([]() {
//			std::this_thread::sleep_for(std::chrono::milliseconds(100));
//			session->doJson("{\"msg\":\"execScript\", \"argument\":\"plot(rnorm(21))\"}");
//			session->doJson("{\"msg\":\"execScript\", \"argument\":\"xz <- matrix(1:10, ncol=5);write(t(xz), file=\\\"mjl.data\\\")\"}");
			
//		});
//		t.detach();
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "execComplete");
		ASSERT_TRUE(results["images"].is_array());
		ASSERT_EQ(results["images"].size(), 1);
		//should really make sure image is valid? How do we do that?
	}

	TEST_F(SessionTest, singleHelp)
	{
		session->emptyMessages();
		session->doJson("{\"msg\":\"help\", \"argument\":\"lm\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "help");
		ASSERT_TRUE(results["topic"] == "lm");
	}

/*	TEST_F(SessionTest, multipleHelp)
	{
		session->doJson("{\"msg\":\"help\", \"argument\":\"lm\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "results");
		ASSERT_TRUE(stringForJsonKey(results1, "helpTopic") == "lm");
	}
*/

	TEST_F(SessionTest, watchVariables)
	{
		//need to delay action until after startEventLoop()
		std::thread t([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"x <- 2\"}");
			session->doJson("{\"msg\":\"toggleVariableWatch\", \"watch\":true}");
			session->emptyMessages();
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"x <- 4\"}");
		});
		t.detach();
		session->startCountdown(4);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 3);
		session->popMessage(); //execComplete for execScript
		json results1 = session->popMessage();
		ASSERT_TRUE(results1["msg"] == "variableupdate");
		ASSERT_TRUE((bool)results1["delta"]);
		ASSERT_EQ(results1["variables"]["assigned"]["x"]["value"][0], 4);
	}
	/** This test doesn't work because things need to happen in a particular order and there are race condtions, or else message counts vary.

	TEST_F(SessionTest, saveRData)
	{
		std::thread t([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			session->execScript("xy <- 11");
			session->doJson("{\"msg\":\"saveEnv\"}");
			session->execScript("xy <- 22");
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			ASSERT_TRUE(session->doLoadEnvironment());
			session->emptyMessages();
			session->doJson("{\"msg\":\"getVariable\", \"argument\":\"xy\"}");
		});
		t.detach();
		session->startCountdown(4);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		cout << results.dump() << std::endl;
		ASSERT_EQ(results["msg"], "variablevalue");
		ASSERT_EQ(results["name"], "xy");
		session->doJson("{\"msg\":\"close\"}");
	} */
};



