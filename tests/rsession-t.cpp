#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "common/RC2Utils.hpp"
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

namespace RC2 {

string
stringForJsonKey(json::Object &obj, string key) {
	return ((json::String)obj[key]).Value();
}

class TestingSession : public RSession {
	public:
		using RSession::RSession;
		virtual void	sendJsonToClientSource(std::string json);
		
		bool fileExists(string filename);
		void copyFileToWorkingDirectory(string srcPath);
		void removeAllWorkingFiles();
		
		void doJson(std::string json);
		
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json::Object popMessage();
		
		queue<string> _messages;
};

void
TestingSession::sendJsonToClientSource(std::string json) {
	_messages.push(json);
	cerr << json << endl;
}

bool
TestingSession::fileExists(string filename)
{
	fs::path fp = getWorkingDirectory();
	fp /= filename;
	return fs::exists(fp);
}

void
TestingSession::copyFileToWorkingDirectory(string srcPath)
{
	fs::path src = GetPathForExecutable(getpid());
	src = src.parent_path();
	src /= srcPath;
	fs::path dest = getWorkingDirectory();
	dest /= src.filename();
	if (!fs::exists(src))
		throw runtime_error("no such file");
	fs::copy_file(src, dest, fs::copy_option::overwrite_if_exists);
}

void
TestingSession::removeAllWorkingFiles()
{
	fs::path wd(getWorkingDirectory());
	for (fs::directory_iterator end_itr, itr(wd); itr != end_itr; ++itr) {
		remove_all(itr->path());
	}
}

json::Object
TestingSession::popMessage() {
	string json = _messages.front();
	json::Object doc;
	try {
		std::istringstream ist(json);
		json::Reader::Read(doc, ist);
	} catch (json::Reader::ParseException &pe) {
		cerr << "parse exception:" << pe.what() << endl;
	} catch (std::exception &ex) {
		cerr << "unknown exception parsing:" << ex.what() << endl;
	}
	return doc;
}

void
TestingSession::doJson(std::string json) {
	handleJsonCommand(json);
}


namespace testing {

	class SessionTest : public ::testing::Test {
		protected:
			static RSessionCallbacks *callbacks;
			static TestingSession *session;
			
			static void SetUpTestCase() {
				boost::log::core::get()->set_filter(
					boost::log::trivial::severity >= (boost::log::trivial::info)
				);
				callbacks = new RSessionCallbacks();
				session = new TestingSession(callbacks);
				session->doJson("{\"msg\":\"open\", \"argument\": \"\"}");
				cerr << "SetupTestCase session open\n";
			}
			
			static void TearDownTestCase() {
				try {
					delete session;
					delete callbacks;
				} catch (...) {
				}
			}
			
			virtual void SetUp() {
				session->emptyMessages();
			}
			
			virtual void TearDown() {
				session->removeAllWorkingFiles();
			}
	}; 

	RSessionCallbacks* SessionTest::callbacks = NULL;
	TestingSession* SessionTest::session = NULL;

	TEST_F(SessionTest, basicScript)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"2*2\"}");
		ASSERT_EQ(session->_messages.size(), 2);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "results");
		ASSERT_TRUE(stringForJsonKey(results1, "string") == "[1] 4\n");
	}

	TEST_F(SessionTest, execFiles)
	{
		session->copyFileToWorkingDirectory("test1.R");
		session->doJson("{\"msg\":\"execFile\", \"argument\":\"test1.R\"}");
		ASSERT_EQ(session->_messages.size(), 2);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "results");
		ASSERT_TRUE(stringForJsonKey(results1, "string") == "\n> x <- 4\n\n> x * x\n[1] 16\n");
	}

	TEST_F(SessionTest, execRMD)
	{
		session->copyFileToWorkingDirectory("test1.Rmd");
		session->doJson("{\"msg\":\"execFile\", \"argument\":\"test1.Rmd\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "execComplete");
		ASSERT_TRUE(session->fileExists("test1.html"));
	}

	TEST_F(SessionTest, execSweave)
	{
		session->copyFileToWorkingDirectory("test1.Rnw");
		session->doJson("{\"msg\":\"execFile\", \"argument\":\"test1.Rnw\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "execComplete");
		ASSERT_TRUE(session->fileExists("test1.pdf"));
	}

	TEST_F(SessionTest, genImages)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"plot(rnorm(11))\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "execComplete");
		ASSERT_TRUE(session->fileExists("rc2img001.png"));
	}
/*
	TEST_F(SessionTest, singleHelp)
	{
		session->doJson("{\"msg\":\"help\", \"argument\":\"lm\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json::Object results1 = session->popMessage();
		ASSERT_TRUE(stringForJsonKey(results1, "msg") == "execComplete");
	}
*/
};
};


