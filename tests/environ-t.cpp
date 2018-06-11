#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include "../src/RC2Logging.h"
#include "common/RC2Utils.hpp"
#include "json.hpp"
#include <RInside.h>
#include "../src/RSession.hpp"
//#include "../src/RSessionCallbacks.hpp"
#include "../src/EnvironmentWatcher.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestingSession.hpp"

using json = nlohmann::json;
using namespace std;
namespace fs = boost::filesystem;

namespace testing {
	class EnvironTest : public BaseSessionTest {
		virtual void pureVirtual() {}
	};
	
	TEST_F(EnvironTest, basicScript)
	{
		Rcpp::Environment global;
		Rcpp::Nullable<Rcpp::StringVector> preGlobal = global["aa"];
		ASSERT_TRUE(preGlobal.isNull());
//		ASSERT_FALSE(preGlobal.length() == 1);
//		ASSERT_TRUE(R_NilValue == Rcpp::Environment::global_env()["aa"]);
		cout << "creating env" << endl;
		Rcpp::Environment testEnv(Rcpp::new_env(global));
		ASSERT_FALSE(testEnv == global);
		cout << "created new env" << endl;
		SEXP ans;
		session->getInside()->parseEvalR("aa <- c(2,4,5)", ans, &testEnv);
		cout << "executed" << endl;
		Rcpp::Nullable<Rcpp::StringVector> testAaW = testEnv["aa"];
		Rcpp::Nullable<Rcpp::StringVector> globalAaW = global["aa"];
		cout << "got aa: " << testAaW.isNotNull() << ", gaa:" << globalAaW.isNotNull() << endl;
		Rcpp::StringVector testAa = testAaW.get();
		cout << "casting aa" << endl;
//		ASSERT_TRUE(postGlobal.isNull());
//		std::cout << "aa=" << testAa[0] << "; g=" << globalAa[0] << std::endl;
		ASSERT_TRUE(testAa[0] == "2");
		ASSERT_TRUE(globalAaW.isNull());
	}
};



