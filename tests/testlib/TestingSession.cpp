#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include <event2/event.h>
#include <event2/thread.h>
#include <glog/logging.h>
#include "common/RC2Utils.hpp"
#include "json.hpp"
#include "../src/RSession.hpp"
#include "../src/RSessionCallbacks.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

using json = nlohmann::json;
using namespace std;
namespace fs = boost::filesystem;

namespace RC2 {

	map<string,int64_t> mapArayToMapMap(json jarray) {
		map<string,int64_t> outMap;
		for(auto& element : jarray) {
			outMap[element["name"]] =  element["value"][0];
		}
		return outMap;
	}
	
	void myevent_logger(int severity, const char *msg) {
		string val(msg);
// 		stderr << val << endl;
	}
	

class TestingSession : public RSession {
	public:
		using RSession::RSession;
		virtual void	sendJsonToClientSource(std::string json);
		
		bool fileExists(string filename);
		void copyFileToWorkingDirectory(string srcPath);
// 		void removeAllWorkingFiles();
		
		void doJson(std::string json);
		
		inline void emptyMessages() { while (!_messages.empty()) _messages.pop(); }
		
		json2 popMessage();
		
		void startCountdown(int count);
		void executeDelayedJson(string msg);
		
		event_base* cheatBase() { return getEventBase(); }
		queue<string> _messages;
		bool countingDown;
		int countDown;
};

void TestingSession::startCountdown ( int count )
{
	countDown = count; 
	countingDown = true;
}

void TestingSession::executeDelayedJson ( string msg )
{
	struct DelayJson {
		TestingSession *session;
		string msg;
	};
	DelayJson *arg = new DelayJson();
	arg->session = this;
	arg->msg = msg;
	struct timeval delay = {0, 1};
	event_callback_fn closure = [](int f, short fl, void* arg) { 
		DelayJson *msg = (DelayJson*)arg;
		msg->session->doJson(msg->msg); 
		delete msg;
	};
	struct event *ev = event_new(getEventBase(), -1, 0, closure, arg);
	event_priority_set(ev, 0);
	event_add(ev, &delay);
}


void
TestingSession::sendJsonToClientSource(std::string jsonStr) {
	_messages.push(jsonStr);
	cerr <<"t-json:" <<  jsonStr << " (" << countDown << ")" << endl;
	if (countingDown) {
		countDown--;
		if (countDown <= 0) {
			stopEventLoop();
		}
	}
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

json2
TestingSession::popMessage() {
	string str = _messages.front();
	_messages.pop();
	json2 j = json2::parse(str);
	return j;
}

void
TestingSession::doJson(std::string json) {
	handleJsonCommand(json);
}

};
