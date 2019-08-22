#include "../src/RC2Logging.h"
#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include <Rcpp.h>
#include <RInside.h>
#include "common/RC2Utils.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestingSession.hpp"
#include "src/EnvironmentWatcher.hpp"

using json = nlohmann::json;
using namespace std;

namespace testing {

	class EvaluateTest : public BaseSessionTest {
//		RC2::TemporaryDirectory tmpDir;
		virtual void pureVirtual() {}
	};
	
	TEST_F(EvaluateTest, basicEvaluate)
	{
		EnvironmentWatcher watcher(session->getExecCallback());
		auto inside = session->getInside();
		SEXP answer=NULL;
		string query("library(evaluate); evaluate(\"doofus()\")");
		RInside::ParseEvalResult result = inside->parseEvalR(query, answer, watcher.getEnvironment());
		ASSERT_EQ(result, RInside::ParseEvalResult::PE_SUCCESS);
		RObject resultObjecct;
		resultObjecct = answer;
		ASSERT_EQ(resultObjecct.sexp_type(), VECSXP);
		json debugObj = {};
		watcher.valueToJson("output", resultObjecct, debugObj, true);
		cout << "obj=" << debugObj.dump(4) << endl;
		Rcpp::List resultList(resultObjecct);
		Rcpp::List contents = resultList[1];
		RObject elem1 = contents[0];
		ASSERT_EQ(elem1.sexp_type(), STRSXP);
		cout << "query=" << elem1.sexp_type() << endl;
//		session->doJson("{\"msg\":\"execScript\", \"argument\":\"testVar<-22\"}");
//		session->emptyMessages();
//		session->doJson("{\"msg\":\"getVariable\", \"argument\":\"testVar\"}");
//		ASSERT_EQ(session->_messages.size(), 1);
//		json results = session->popMessage();
//		ASSERT_TRUE(results["msg"] == "variablevalue");
//		ASSERT_TRUE(results["value"]["value"][0] == 22);
	}
}