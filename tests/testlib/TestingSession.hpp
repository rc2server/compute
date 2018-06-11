#pragma once
#include <gtest/gtest.h>
#include <string>
#include <queue>
#include <event2/event.h>
#include <event2/thread.h>
#include "../../src/RSession.hpp"
#include "../../src/RSessionCallbacks.hpp"
#include "../../src/FileManager.hpp"
#include "../../src/RC2Logging.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

namespace RC2 {
	
	map<string,int64_t> mapArayToMapMap(json jarray);
	void myevent_logger(int severity, const char *msg);	
	
	class TestingSession : public RSession {
	public:
		
		TestingSession(RSessionCallbacks *callbacks, FileManager *fm=nullptr);
		
		virtual void	sendJsonToClientSource(std::string json);
		
		bool fileExists(string filename);
		void copyFileToWorkingDirectory(string srcPath);
		
		void doJson(std::string json);
		void execScript(std::string rcode);
		
		RInside* getInside() const { return getRInside(); };
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json popMessage();
		
		void startCountdown(int count);
		void executeDelayedJson(string msg);
		
		ExecuteCallback getExecCallback() { return getExecuteCallback(); }
		bool doLoadEnvironment() { return loadEnvironment(); }
		
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
	
	struct TestLogging {
		TestLogging() {
			using namespace g3;
			static std::unique_ptr<LogWorker> logworker{ LogWorker::createLogWorker() };
			auto sinkHandle = logworker->addSink(std2::make_unique<RC2::CustomSink>(),
												 &RC2::CustomSink::ReceiveLogMessage);
			
			// initialize the logger before it can receive LOG calls
			initializeLogging(logworker.get());
		}
	};
	
	class BaseSessionTest : public ::testing::Test {
	protected:
		static RSessionCallbacks *callbacks;
		static TestingSession *session;
		static TestingFileManager *fileManager;
//		static unique_ptr<TestLogging> testLogger;
		
		static void SetUpTestCase() {
			cerr << "setting up..." << endl;
			evthread_use_pthreads();
			fileManager = new TestingFileManager();
			callbacks = new RSessionCallbacks();
			session = new TestingSession(callbacks, fileManager);
			using namespace g3;
			static std::unique_ptr<LogWorker> logworker{ LogWorker::createLogWorker() };
			auto sinkHandle = logworker->addSink(std2::make_unique<RC2::CustomSink>(),
												 &RC2::CustomSink::ReceiveLogMessage);
			
			// initialize the logger before it can receive LOG calls
			initializeLogging(logworker.get());
			event_set_log_callback(myevent_logger);
			session->prepareForRunLoop();
			session->doJson("{\"msg\":\"open\", \"argument\": \"\", \"wspaceId\":1, \"sessionRecId\":1, \"dbhost\":\"localhost\", \"dbuser\":\"rc2\", \"dbname\":\"rc2test\", \"dbpass\":\"rc2\"}");
			cerr << "setup complete" << endl;
//			fileManager->setWorkingDir(session->getWorkingDirectory());
		}
		
		static void TearDownTestCase() {
			try {
				session->doJson("{\"msg\":\"close\"}");
				delete session;
				session = NULL;
				delete callbacks;
			} catch (...) {
			}
		}
		
		virtual void SetUp() {
			session->emptyMessages();
		}
		
		virtual void TearDown() {
		}
		
		virtual void pureVirtual() = 0;
	}; 
	
};
