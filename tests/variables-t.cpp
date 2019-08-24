#include "../src/RC2Logging.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include <Rcpp.h>
#include "common/RC2Utils.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestingSession.hpp"
#include "src/EnvironmentWatcher.hpp"

using json = nlohmann::json;
using namespace std;

namespace testing {

	class VarTest : public BaseSessionTest {
//		RC2::TemporaryDirectory tmpDir;
		virtual void pureVirtual() {}
	};
	
	TEST_F(VarTest, getVariable)
	{
		session->execScript("testVar<-22");
		session->emptyMessages();
		string msg = R"foo({"msg":"getVariable", "argument":"testVar"})foo";
		cerr << "HERE sending: " << msg << endl;
		session->doJson(msg);
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "variablevalue");
		ASSERT_TRUE(results["value"]["value"][0] == 22);
	}

	TEST_F(VarTest, listVariables)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"rm(list=ls())\"}");
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"z <- c('foo','bar')\"}");
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"x<-22;y<-11\"}");
		session->emptyMessages();
		session->doJson("{\"msg\":\"listVariables\", \"argument\":\"\", \"watch\":true}");
		cerr << "message size:" << session->_messages.size() << endl;
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "variableupdate");
		auto vars = results["variables"];
		auto xval = vars["x"];
		auto xvals = xval["value"];
		ASSERT_EQ(xvals[0], 22);
		// TODO: test more values are correct
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
		long val = djson["value"];
		ASSERT_EQ(1462374671, val);
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
		ASSERT_EQ(df["columns"][0]["name"], "height");
		ASSERT_EQ(df["columns"][0]["type"], "d");
		ASSERT_EQ(df["row.names"][0], "1");
		ASSERT_EQ(df["row.names"][14], "15");
		ASSERT_EQ(df["columns"][0]["values"][0], 58.0);
		ASSERT_EQ(df["columns"][1]["values"][0], 115.0);
		ASSERT_EQ(df["columns"][0]["values"][14], 72.0);
		ASSERT_EQ(df["columns"][1]["values"][1], 117.0);
	}

	TEST_F(VarTest, complexDframe) {
		session->emptyMessages();
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(), session->getExecCallback());
		// CRASH: using tmp dir before created in session open
		session->copyFileToWorkingDirectory("dframe.R");
		fileManager->addFile(4, "dframe.R", 1);
		session->executeDelayedJson("{\"msg\":\"execFile\", \"argument\":\"4\"}");
		session->startCountdown(2);
		session->startEventLoop();
		ASSERT_EQ(session->_messages.size(), 2);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "results");
		json df = watcher.toJson("cdf");
//		cout << "val = " << df.dump(4) << endl;
		int nrow = df["nrow"];
		int ncol = df["ncol"];
		ASSERT_EQ(ncol, 4);
		ASSERT_EQ(nrow, 6);
		auto columns = df["columns"];
		auto namesCol = columns[2];
		ASSERT_EQ(namesCol["type"], "s");
		ASSERT_EQ(namesCol["values"][3], "Mario");
		
		auto c2Column = std::find_if(columns.begin(), columns.end(), [](const json& x) {
			return x.is_object() and x.value("name", "") == "c2";
		});
		ASSERT_FALSE(c2Column == columns.end()); // make sure *res is the column we're looking for
		ASSERT_EQ((*c2Column)["values"][0], 3.14);
		ASSERT_TRUE((*c2Column)["values"][1] == nullptr);
		ASSERT_EQ((*c2Column)["values"][2], "NaN");
		ASSERT_EQ((*c2Column)["values"][3], 21.0);
		ASSERT_EQ((*c2Column)["values"][4], "Inf");
		ASSERT_EQ((*c2Column)["values"][5], "-Inf");
	}
	
	TEST_F(VarTest, matrixTest) {
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(), session->getExecCallback());
		session->execScript("mat <- matrix(data=1:8, nrow=4, ncol=2, dimnames=list(c(\\\"x\\\",\\\"y\\\",\\\"z\\\",\\\"a\\\"), c(\\\"foo\\\",\\\"bar\\\")))");
		json mat = watcher.toJson("mat");
		int numrows = mat["nrow"];
		ASSERT_EQ(numrows, 4);
		int numcols = mat["ncol"];
		ASSERT_EQ(numcols, 2);
	}
	
	TEST_F(VarTest, doubleMatrix) {
		EnvironmentWatcher watcher(Rcpp::Environment::global_env(), session->getExecCallback());
		session->execScript("mat <- matrix(data=c(2.1, NaN, 4.3, -Inf), nrow=2, ncol=2, dimnames=list(c(\\\"x\\\",\\\"y\\\"), c(\\\"foo\\\",\\\"bar\\\")))");
		json mat = watcher.toJson("mat");
		cerr << "val = " << mat.dump(4) << endl;
		int numrows = mat["nrow"];
		ASSERT_EQ(numrows, 2);
		int numcols = mat["ncol"];
		ASSERT_EQ(numcols, 2);
		// TODO: test that the values are correct. This is hard b/c json wants to use stl what requires homogenous containers
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

