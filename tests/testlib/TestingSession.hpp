#pragma once
#include "gtest/gtest.h"
#include <string>
#include <queue>
#include <event2/event.h>
#include <event2/thread.h>
#include "../../src/RSession.hpp"
#include "../../src/RSessionCallbacks.hpp"
#include "../../src/FileManager.hpp"
#include "../../src/RC2Logging.h"
#include "../../src/EnvironmentWatcher.hpp"
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

namespace RC2 {
	
	map<string,int64_t> mapArayToMapMap(json jarray);
	void myevent_logger(int severity, const char *msg);	
	
	class TestingSession : public RSession {
	public:
		
		explicit TestingSession(RSessionCallbacks *callbacks, FileManager *fm=nullptr);
		
		virtual void	sendJsonToClientSource(std::string json);
		
		bool fileExists(string filename);
		void copyFileToWorkingDirectory(string srcPath);
		
		void doJson(std::string json);
		void execScript(std::string rcode);
		
		RInside* getInside() const { return getRInside(); };
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json popMessage();
		
		void startCountdown(int count);
		void executeDelayedJson(string msg, int delay = 1);
		
		RInside::ParseEvalResult parseEvalR(std::string query, SEXP &answer, EnvironmentWatcher &watcher);

		using RSession::getExecuteCallback;
		
		ExecuteCallback getExecCallback() { return getExecuteCallback(); }
		bool doLoadEnvironment() { return loadEnvironment(); }
		
		void setApi(int version) { setApiVersion(version); }
		void clearGlobalEnvironment();
		
		virtual string incomingJsonSchemaPath();
		
		event_base* cheatBase() { return getEventBase(); }
		queue<string> _messages;
		bool countingDown;
		int countDown;
	};	
	
	class TestingFileManager : public FileManager {
	public:
		TestingFileManager();
		virtual ~TestingFileManager();
		virtual void 	initFileManager(std::string connectString, int wspaceId, int sessionRecId);
		
// 		virtual void	resetWatch();
// 		virtual void	checkWatch(std::vector<long> &imageIds, long &batchId);
// 		virtual void	cleanupImageWatch();
		
		virtual bool	loadRData();
		virtual void	saveRData();
		
		virtual bool	filePathForId(long fileId, std::string& filePath);
		virtual void	findOrAddFile(std::string fname, FileInfo &info);
		virtual bool	fileInfoForId(long fileId, FileInfo &info);
		
		virtual void	suspendNotifyEvents();
		virtual void	resumeNotifyEvents();
		
		//file should have been copied to the working directory
		void	addFile(long fileId, std::string name, long version);
	
	protected:
		std::vector<FileInfo> _files;
	};
}


namespace testing {
	using namespace RC2;
	
	class BaseSessionTest : public ::testing::Test {
	protected:
		static RSessionCallbacks *callbacks;
		static TestingSession *session;
		static TestingFileManager *fileManager;
		static void initStaticObjects();

		static void SetUpTestCase() {
			cerr << "setting up..." << endl;
			initStaticObjects();
			evthread_use_pthreads();
// 			try {
// 				fileManager = new TestingFileManager();
// 				callbacks = new RSessionCallbacks();
// 				session = new TestingSession(callbacks, fileManager);
// 			} catch (std::exception &e) {
// 				cout << "exception in setup:" << e.what() << endl;
// 			}
			session->prepareForRunLoop();
			session->doJson(R"({"msg":"open", "argument": "", "wspaceId":1, "sessionRecId":1, "dbhost":"localhost", "dbuser":"rc2", "dbname":"rc2test", "dbpassword":"rc2"})");
			cerr << "setup complete" << endl;
//			fileManager->setWorkingDir(session->getWorkingDirectory());
		}
		
		static void TearDownTestCase() {
		}
		
		virtual void SetUp() {
			if (session == nullptr) return;
			session->emptyMessages();
		}
		
		virtual void TearDown() {
		}
		
		virtual void pureVirtual() = 0;
	}; 
	
};
