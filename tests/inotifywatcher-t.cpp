#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <thread>
#include <mutex>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include "../src/InotifyFileWatcher.hpp"
#include "common/RC2Utils.hpp"

//TODO: add test case for deleting parent folder

using namespace std;

namespace RC2 {
namespace testing {

	void
	writeDummyFile(string fname)
	{
		ofstream file;
		file.open(fname);
		file << "data here" << endl;
		file.close();
	}

	void
	modifyDummyFile(string fname)
	{
		ofstream file;
		file.open(fname, ofstream::out | ofstream::app);
		file << "data here" << endl;
		file.close();
	}

	class InotifyWatcherTest : public ::testing::Test {
		protected:
			static void SetUpTestCase() {
				boost::log::core::get()->set_filter(
					boost::log::trivial::severity >= (boost::log::trivial::warning)
				);
			}
	};

	TEST_F(InotifyWatcherTest, simpleAddTest)
	{
		event_base *eb = event_base_new();
		InotifyFileWatcher watcher;
		watcher.setEventBase(eb);
		vector<string> added, modified, deleted;
		TemporaryDirectory tmpDir;
		mutex mx;

		watcher.initializeWatcher(tmpDir.getPath());
		watcher.startWatch();
		
		//add a foo object
		string tfilename = tmpDir.getPath() + "/foo";
		mx.lock();
		thread t([&] () {
			unique_lock<mutex> ulock(mx);
			writeDummyFile(tfilename);
			event_base_loopbreak(eb);
		});
		mx.unlock();

		event_base_loop(eb, EVLOOP_ONCE);
		watcher.stopWatch(added, modified, deleted);
		
		ASSERT_TRUE(added.size() == 1);
		ASSERT_TRUE(modified.size() == 0);
		ASSERT_TRUE(deleted.size() == 0);
		//wait for bg threads to continue
		t.join();

		event_base_free(eb);
	}

	TEST_F(InotifyWatcherTest, simpleModifyTest)
	{
		TemporaryDirectory tmpDir;
		string tfilename = tmpDir.getPath() + "/foo";
		writeDummyFile(tfilename);

		event_base *eb = event_base_new();
		InotifyFileWatcher watcher;
		watcher.setEventBase(eb);
		vector<string> added, modified, deleted;

		watcher.initializeWatcher(tmpDir.getPath());
		watcher.startWatch();
		
		//modify a foo object
		mutex mx;
		mx.lock();
		thread t([&] () {
			unique_lock<mutex> ulock(mx);
		ofstream file;
		file.open(tfilename, ofstream::out | ofstream::app);
		file << "data here" << endl;
		file.close();
			event_base_loopbreak(eb);
		});
		mx.unlock();

		event_base_loop(eb, EVLOOP_ONCE); //modified
		event_base_loop(eb, EVLOOP_ONCE); //closed w/write
		watcher.stopWatch(added, modified, deleted);
		
		ASSERT_TRUE(added.size() == 0);
		ASSERT_TRUE(modified.size() == 1);
		ASSERT_TRUE(deleted.size() == 0);
		//wait for bg threads to continue
		t.join();

		event_base_free(eb);
	}

	TEST_F(InotifyWatcherTest, simpleDeleteTest)
	{
		TemporaryDirectory tmpDir;
		string tfilename = tmpDir.getPath() + "/foo";
		writeDummyFile(tfilename);

		event_base *eb = event_base_new();
		InotifyFileWatcher watcher;
		watcher.setEventBase(eb);
		vector<string> added, modified, deleted;

		watcher.initializeWatcher(tmpDir.getPath());
		watcher.startWatch();
		
		//delete foo
		mutex mx;
		mx.lock();
		thread t([&] () {
			unique_lock<mutex> ulock(mx);
			unlink(tfilename.c_str());
			event_base_loopbreak(eb);
		});
		mx.unlock();

		event_base_loop(eb, EVLOOP_ONCE); //deleted
		watcher.stopWatch(added, modified, deleted);
		
		ASSERT_TRUE(added.size() == 0);
		ASSERT_TRUE(modified.size() == 0);
		ASSERT_TRUE(deleted.size() == 1);
		//wait for bg threads to continue
		t.join();

		event_base_free(eb);
	}

};
};
