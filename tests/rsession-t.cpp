#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include "../src/RC2Logging.h"
#include "common/RC2Utils.hpp"
#include <nlohmann/json.hpp>
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"
#include "../src/RJsonEncoder.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestingSession.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = boost::filesystem;
auto sleepTime = std::chrono::milliseconds(10);

namespace testing {
	class SessionTest : public BaseSessionTest {
		virtual void pureVirtual() {}
	};
	
	TEST_F(SessionTest, nonuserExec)
	{
		using namespace Rcpp;
		SEXP result;
		Rcpp::Environment env;

		session->executeNonUserCode("rc2.evaluateCode(\"2+2\")", result, &env);
		ASSERT_NE(result, nullptr);

		RObject resultObj(result);
		ASSERT_EQ(resultObj.sexp_type(), 19);

		List rlist(result);
		ASSERT_EQ(rlist.size(), 2);
		List valList1(rlist[1]);
//		for (auto x: as<StringVector>(valList1.attr("names")))
//			cout << "n=" << x << endl;
		ASSERT_EQ(valList1.size(), 4);
		string cname = valList1.attr("class");
		ASSERT_EQ(cname, "rc2value");
//		cout << "data val is " << TYPEOF(valList1["data"]) << endl;
		StringVector strs(as<StringVector>(valList1["str"]));
		ASSERT_EQ(strs[0], "[1] 4");
//		RObject val(valList1["data"]);
//		cout << "v=" << val.sexp_type() << endl;
	}
	
	TEST_F(SessionTest, evaluate)
	{
		RInside* inside = session->getInside();
		SEXP resultSEXP;
		inside->parseEval("wrapper <- evaluateWrapper()", resultSEXP);
		inside->parseEval("wrapper$evaluate(\"2*2\")", resultSEXP);
		//		inside->parseEval("wrapper <- evaluateWrapper(); wrapper.rc2evaluate(\"2+2\")", resultSEXP);
		ASSERT_NE(resultSEXP, nullptr);
		Rcpp::List evalLists(resultSEXP);
		cout << "cnt=" << evalLists.size() << endl;
		RObject fullVal(resultSEXP);
		RJsonEncoder encoder(session->getExecCallback());
		json values = encoder.toJson(fullVal, false);
		cout << "json=" << values.dump(4) << endl;
		ASSERT_GT(evalLists.size(), 0);
		
// 		Rcpp::List lval(robj);
// 		std::cout << "lv=" << lval.length() << std::endl;
// 		RObject sval(lval(0));
// 		std::cout << "l=" << lval.sexp_type() << " sv=" << sval.sexp_type() << std::endl;
// 		Rcpp::StringVector sv2(sval);
// 		Rcpp::String str(sv2(0));
// 		std::cout << "n=" << str.get_cstring() << std::endl;
	}
	
	TEST_F(SessionTest, basicScript)
	{
		//need to delay action until after startEventLoop()
		std::thread t([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"4\", \"queryId\": 2}");
		});
		t.detach();		
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results1 = session->popMessage();
		ASSERT_TRUE(results1["msg"] == "results");
		ASSERT_TRUE(results1["string"] == "[1] 4\n");
	}

		TEST_F(SessionTest, basicScriptWithQueryIdAndStartTime)
	{
		//need to delay action until after startEventLoop()
		std::thread t([]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"rnorm(20)\", \"queryId\":3, \"startTime\": \"1502817203955\"}");
		});
		t.detach();
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results1 = session->popMessage();
		ASSERT_TRUE(results1["msg"] == "results");
		string results = results1["string"];
		string prefix = " [1] ";
		ASSERT_TRUE(std::equal(prefix.begin(), prefix.end(), results.begin()));
		int queryId = results1["queryId"];
		ASSERT_TRUE(queryId == 3);
	}


	TEST_F(SessionTest, execFiles)
	{
		session->copyFileToWorkingDirectory("test1.R");
		fileManager->addFile(3, "test1.R", 1);
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
		session->copyFileToWorkingDirectory("test1.Rnw");
		fileManager->addFile(5, "test1.Rnw", 1);
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
		session->startCountdown(1);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "execComplete");
		ASSERT_TRUE(results["images"].is_array());
		ASSERT_EQ(results["images"].size(), 2);
		//should really make sure image is valid? How do we do that?
	}

	TEST_F(SessionTest, singleHelp)
	{
//		session->execScript("library(rc2)");
//		session->execScript("options(device = \"rc2.pngdev\", bitmapType = \"cairo\")");
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
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"rm(list = ls())\"}");
			session->doJson("{\"msg\":\"execScript\", \"argument\":\"x <- 2\"}");
			session->doJson(R"({"msg": "toggleVariableWatch", "argument": "", "watch": true})");
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



