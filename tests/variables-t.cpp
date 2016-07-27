#include "../src/RC2Logging.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include <Rcpp.h>
#include "common/RC2Utils.hpp"
#include "testlib/TestingSession.hpp"
#include "src/EnvironmentWatcher.hpp"

using json = nlohmann::json;
using namespace std;

namespace testing {

	class VarTest : public BaseSessionTest {
		virtual void pureVirtual() {}
	};
	
	TEST_F(VarTest, getVariable)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"testVar<-22\"}");
		session->emptyMessages();
		session->doJson("{\"msg\":\"getVariable\", \"argument\":\"testVar\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "variablevalue");
		ASSERT_TRUE(results["value"]["value"][0] == 22);
	}

	TEST_F(VarTest, listVariables)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"rm(list=ls())\"}");
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"x<-22;y<-11\"}");
		session->emptyMessages();
		session->doJson("{\"msg\":\"listVariables\", \"argument\":\"\", \"watch\":true}");
		cerr << "message size:" << session->_messages.size() << endl;
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		auto vars = results["variables"];
		auto vals = vars["values"];
		auto elems = mapArayToMapMap(results["variables"]);
		ASSERT_TRUE(results["msg"] == "variableupdate");
		ASSERT_EQ(elems["x"], 22);
		ASSERT_EQ(elems["y"], 11);
	}

	TEST_F(VarTest, basicWatcher)
	{
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(), session->getExecCallback());
		//number
		session->execScript("x <- 20");
		session->emptyMessages();
		json xjson = watcher.toJson("x");
		ASSERT_EQ(xjson["type"], "d");
		ASSERT_EQ(xjson["value"][0], 20);
		//boolean
		session->execScript("b <- c(TRUE,FALSE)");
		json bjson = watcher.toJson("b");
		ASSERT_EQ(bjson["type"], "b");
		json vals = bjson["value"];
		ASSERT_TRUE(vals.is_array());
		bool val1 = vals[0];
		ASSERT_EQ(val1, true);
		ASSERT_EQ(vals[1], false);
		//strings
		session->execScript("ss <- c(\\\"foo\\\",\\\"bar\\\")");
		json sjson = watcher.toJson("ss");
		ASSERT_EQ(sjson["type"], "s");
		ASSERT_TRUE(sjson["value"].is_array());
		ASSERT_EQ(sjson["value"][0], "foo");
		ASSERT_EQ(sjson["value"][1], "bar");
		//complex
		session->execScript("cpx <- complex(real=c(1.345, -0.134), imaginary = c(0.485, -1.212))");
		json cjson = watcher.toJson("cpx");
		ASSERT_EQ(cjson["type"], "c");
		ASSERT_TRUE(cjson["value"].is_array());
		ASSERT_EQ(cjson["value"][0], "1.345+0.485i");
		ASSERT_EQ(cjson["value"][1], "-0.134-1.212i");
		//POSIXct
		session->execScript("ctd <- as.POSIXct(\\\"2016-05-04 11:11:11 GMT\\\")");
		json djson = watcher.toJson("ctd");
		ASSERT_EQ(djson["class"], "POSIXct");
		ASSERT_EQ(djson["value"], 1462385471);
		//function
		session->execScript("hlp <- help");
		json funjson = watcher.toJson("hlp");
		ASSERT_EQ(funjson["class"], "function");
		string prefix("function (topic)");
		string body = funjson["body"];
		ASSERT_TRUE(!body.compare(0, prefix.size(), prefix));
		
		//TODO: Date, POSIXlt, matrix, array, list, s3, s4
	}
	
	TEST_F(VarTest, factorTest) {
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(),  session->getExecCallback());
		session->execScript("f <- factor(c(1,1,3,4,2,5), labels = letters[1:5])");
		json fjson = watcher.toJson("f");
		ASSERT_EQ(fjson["type"], "f");
		ASSERT_EQ(fjson["levels"][0], "a");
		ASSERT_EQ(fjson["levels"][1], "b");
		ASSERT_EQ(fjson["levels"][2], "c");
		ASSERT_EQ(fjson["levels"][3], "d");
		ASSERT_EQ(fjson["levels"][4], "e");
		ASSERT_EQ(fjson["value"][0], 1);
		ASSERT_EQ(fjson["value"][1], 1);
		ASSERT_EQ(fjson["value"][2], 3);
		ASSERT_EQ(fjson["value"][3], 4);
		ASSERT_EQ(fjson["value"][4], 2);
		ASSERT_EQ(fjson["value"][5], 5);
	}

	TEST_F(VarTest, dframeTest) {
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(), session->getExecCallback());
		session->execScript("df <- women");
		json df = watcher.toJson("df");
		ASSERT_EQ(df["nrow"], 15);
		ASSERT_EQ(df["ncol"], 2);
		ASSERT_EQ(df["types"][0], "d");
		ASSERT_EQ(df["types"][1], "d");
		ASSERT_EQ(df["cols"][0], "height");
		ASSERT_EQ(df["cols"][1], "weight");
		ASSERT_EQ(df["row.names"][0], "1");
		ASSERT_EQ(df["row.names"][14], "15");
		ASSERT_EQ(df["rows"][0][0], 58.0);
		ASSERT_EQ(df["rows"][0][1], 115.0);
		ASSERT_EQ(df["rows"][14][0], 72.0);
		ASSERT_EQ(df["rows"][14][1], 164.0);
	}

	TEST_F(VarTest, simpleDelta) {
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(),  session->getExecCallback());
		session->execScript("x <- 2; y <- 4");
		watcher.captureEnvironment();
		session->execScript("x <- y / 4");
		json delta = watcher.jsonDelta();
		ASSERT_EQ(delta["assigned"].size(), 1);
		ASSERT_EQ(delta["removed"].size(), 0);
		ASSERT_EQ(delta["assigned"]["x"]["type"], "d");
		watcher.captureEnvironment();
		session->execScript("rm(y)");
		delta = watcher.jsonDelta();
		ASSERT_EQ(delta["assigned"].size(), 0);
		ASSERT_EQ(delta["removed"].size(), 1);
		ASSERT_EQ(delta["removed"][0], "y");
	}
};

