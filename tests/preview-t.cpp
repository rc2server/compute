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
		
		// TODO: need to add test that performs two updates and make sure get message only those messages returned
		
		TEST_F(PreviewTest, basicUpdate)
		{
			EnvironmentWatcher envWatcher(Rcpp::Environment::global_env(), session->getExecuteCallback());
			FileInfo finfo;
			session->copyFileToWorkingDirectory("test1.Rmd");
			fileManager->findOrAddFile("test1.Rmd", finfo);
			PreviewData pd(5, fileManager, finfo, &envWatcher, session);
			finfo.version += 1;
			string uident("5344gf");
			try {
				pd.update(finfo, uident, 4, false);
				ASSERT_EQ(session->messageCount(), 1);
				auto message = session->popMessage();
				ASSERT_EQ(message["updateIdentifier"], "5344gf");
				ASSERT_EQ(message["previewId"], 5);
				ASSERT_EQ(message["chunkIndex"], 3);
			} catch (const std::exception& e) {
				cout << "exception: " << e.what() << endl;
			}
			
		}
		
		TEST_F(PreviewTest, viaJson)
		{
			// initialize the preview
			FileInfo finfo;
			session->copyFileToWorkingDirectory("test1.Rmd");
			fileManager->findOrAddFile("test1.Rmd", finfo);
			session->emptyMessages();
			json initJson;
			initJson["msg"] = "initPreview";
			initJson["fileId"] = finfo.id;
			initJson["argument"] = "";
			session->doJson(initJson.dump(2));
			ASSERT_EQ(session->messageCount(), 1);
			auto initResponse = session->popMessage();
			ASSERT_EQ(initResponse["msg"], "previewInited");
			ASSERT_EQ(initResponse["errorCode"], 0);
			int previewId = initResponse["previewId"];
			ASSERT_GT(previewId, 0);
			cout << "previewid=" << previewId << endl;
		}

	}; // end testing namespace
}; // end RC2 namespace
