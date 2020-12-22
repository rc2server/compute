#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include <memory>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/range/adaptors.hpp>
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
		
		// TODO: need to add test that performs two updates and make sure get message only those messages returned
		
		TEST_F(PreviewTest, basicUpdate)
		{
			EnvironmentWatcher envWatcher(Rcpp::Environment::global_env(), session->getExecuteCallback());
			FileInfo finfo;
			session->copyFileToWorkingDirectory("preview1.Rmd");
			fileManager->findOrAddFile("preview1.Rmd", finfo);
			PreviewData pd(5, fileManager, finfo, &envWatcher, session);
			finfo.version += 1;
			string uident("5344gf");
			try {
				pd.update(finfo, uident, 4, false);
				ASSERT_EQ(session->messageCount(), 4);
				auto message = session->popMessage();
				message = session->popMessage();
				ASSERT_EQ(message["updateIdentifier"], "5344gf");
				ASSERT_EQ(message["previewId"], 5);
				ASSERT_EQ(message["chunkId"], 1);
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], 3);
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], -1);
			} catch (const std::exception& e) {
				cout << "exception: " << e.what() << endl;
			}
			
		}

		bool jsonContains(json container, json checks) { 
			auto targets = checks.get<std::map<std::string, json>>();
			for (const auto & key : targets | boost::adaptors::map_keys) {
				if ((container.count(key) < 1) || container[key] != checks[key]) return false;
			}
			return true;
		}

		// Executes a Rmd. Then updates assignment so y should be 12 instead of 24. Test that that is true
		TEST_F(PreviewTest, invalidateFutureChunksIncldingPrevious)
		{
			//previewCacheInvalidate.Rmd
			int previewId = 6;
			EnvironmentWatcher envWatcher(Rcpp::Environment::global_env(), session->getExecuteCallback());
			FileInfo finfo;
			string filePath = "previewCacheInvalidate.Rmd";
			session->copyFileToWorkingDirectory(filePath.c_str());
			fileManager->findOrAddFile(filePath.c_str(), finfo);
			PreviewData pd(previewId, fileManager, finfo, &envWatcher, session);
			finfo.version += 1;
			string uident("5344gf");
			try {
				pd.update(finfo, uident, -1, false);
				// that should have returned for every chunk
				ASSERT_EQ(session->messageCount(), 4);
				// TODO: message for chunk 3 should have the output of "[1] 24".
				auto message = session->popMessage(); // update previewUpdateStarted
				message = session->popMessage(); // chunk 1 update
				message = session->popMessage(); // chunk 3 update
				string c3content = message["content"];
				ASSERT_NE(c3content.find("[1] 24"), string::npos);
				session->emptyMessages();
				cout << "first pass done" << endl;
				// edit a change to the file
				string contents = SlurpFile(filePath.c_str());
				UpdateFile(filePath.c_str(), "y * 4", "y * 2");
				// now execute first chunk
				pd.update(finfo, uident, 1, false);
				ASSERT_EQ(session->messageCount(), 3);
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdateStarted"}, {"updateIdentifier", uident} }));
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdated"}, {"chunkId", 1} } ));
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], -1);
				// get result for chunk 3
				session->emptyMessages();
				pd.update(finfo, uident, 3, true);
				ASSERT_EQ(session->messageCount(), 4);
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdateStarted"}, {"updateIdentifier", uident} }));
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdated"}, {"chunkId", 1} } ));
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdated"}, {"chunkId", 3} } ));
				c3content = message["content"];
				ASSERT_NE(c3content.find("[1] 12"), string::npos);
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], -1);
			} catch (const std::exception& e) {
				cout << "exception: " << e.what() << endl;
			}
		}

		// Executes a Rmd. Then updates assignment so y should be 12 instead of 24. Test that that is true
		TEST_F(PreviewTest, invalidateFutureChunks)
		{
			//previewCacheInvalidate.Rmd
			int previewId = 6;
			EnvironmentWatcher envWatcher(Rcpp::Environment::global_env(), session->getExecuteCallback());
			FileInfo finfo;
			string filePath = "previewCacheInvalidate.Rmd";
			session->copyFileToWorkingDirectory(filePath.c_str());
			fileManager->findOrAddFile(filePath.c_str(), finfo);
			PreviewData pd(previewId, fileManager, finfo, &envWatcher, session);
			finfo.version += 1;
			string uident("5344gf");
			try {
				pd.update(finfo, uident, -1, false);
				// that should have returned for every chunk
				ASSERT_EQ(session->messageCount(), 4);
				// TODO: message for chunk 3 should have the output of "[1] 24".
				auto message = session->popMessage(); // update previewUpdateStarted
				message = session->popMessage(); // chunk 1 update
				message = session->popMessage(); // chunk 3 update
				string c3content = message["content"];
				ASSERT_NE(c3content.find("[1] 24"), string::npos);
				session->emptyMessages();
				cout << "first pass done" << endl;
				// edit a change to the file
				string contents = SlurpFile(filePath.c_str());
				UpdateFile(filePath.c_str(), "y * 4", "y * 2");
				// now execute first chunk
				pd.update(finfo, uident, 1, false);
				ASSERT_EQ(session->messageCount(), 3);
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdateStarted"}, {"updateIdentifier", uident} }));
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdated"}, {"chunkId", 1} } ));
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], -1);
				// get result for chunk 3
				session->emptyMessages();
				pd.update(finfo, uident, 3, false);
				ASSERT_EQ(session->messageCount(), 3);
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdateStarted"}, {"updateIdentifier", uident} }));
				message = session->popMessage();
				ASSERT_TRUE(jsonContains(message, { {"msg", "previewUpdated"}, {"chunkId", 3} } ));
				c3content = message["content"];
				ASSERT_NE(c3content.find("[1] 12"), string::npos);
				message = session->popMessage();
				ASSERT_EQ(message["chunkId"], -1);
			} catch (const std::exception& e) {
				cout << "exception: " << e.what() << endl;
			}
		}
		
		TEST_F(PreviewTest, viaJson)
		{
			// initialize the preview
			FileInfo finfo;
			session->copyFileToWorkingDirectory("preview1.Rmd");
			fileManager->findOrAddFile("preview1.Rmd", finfo);
			session->emptyMessages();
			string initIdent = "bahhbahh";
			json initJson;
			initJson["msg"] = "initPreview";
			initJson["fileId"] = finfo.id;
			initJson["argument"] = "";
			initJson["updateIdentifier"] = initIdent;
			session->doJson(initJson.dump(2));
			ASSERT_EQ(session->messageCount(), 1);
			auto initResponse = session->popMessage();
			ASSERT_EQ(initResponse["msg"], "previewInited");
			ASSERT_EQ(initResponse["errorCode"], 0);
			int previewId = initResponse["previewId"];
			ASSERT_GT(previewId, 0);
			ASSERT_EQ(initIdent, initResponse["updateIdentifier"]);

			// send a first update to get all
			json upmsg1;
			string updateIdent = "xxfe";
			upmsg1["msg"] = "updatePreview";
			upmsg1["updateIdentifier"] = updateIdent;
			upmsg1["previewId"] = previewId;
			upmsg1["argument"] = "";
			session->emptyMessages();
			session->doJson(upmsg1.dump(2));
		}

	}; // end testing namespace
}; // end RC2 namespace
