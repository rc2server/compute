#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <memory>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include "testlib/TestingSession.hpp"
#include "../src/RC2Logging.h"
#include "../src/FileManager.hpp"
#include "../src/PreviewData.hpp"

using std::cout;
using std::endl;
using std::string;
using namespace testing;

namespace RC2 {
	class TestPreviewData: PreviewData {
	public:
		using PreviewData::executeChunk;
	};
	
	namespace testing {
		class PreviewTest : public BaseSessionTest {
			virtual void pureVirtual() {}
		};
		
		TEST_F(PreviewTest, basic)
		{
			ASSERT_TRUE(true);
		}
		
		
		TEST_F(PreviewTest, basicCode)
		{
			EnvironmentWatcher envWatcher(Rcpp::Environment::global_env(), session->getExecuteCallback());
			FileInfo finfo;
			session->copyFileToWorkingDirectory("test1.Rmd");
			fileManager->findOrAddFile("test1.Rmd", finfo);
			PreviewData pd(5, fileManager, finfo, &envWatcher,
						   [&] (string str) { },
						   [&] (string code, SEXP& result, Rcpp::Environment* env) { session->executeNonUserCode(code, result, env); }
						);
			finfo.version += 1;
			string uident("5344gf");
			pd.update(finfo, uident, 4, false);
			
		}
	}; // end testing namespace
}; // end RC2 namespace
