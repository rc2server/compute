#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include "../src/RC2Logging.h"
#include "testlib/TestPGDBConnection.hpp"
#include "testlib/TestingSession.hpp"

using namespace std;

namespace testing {
	
	class PGDBConnectionTest: public ::testing::Test {
	public:
		static shared_ptr<RC2::TestPGDBConnection> db;
//		static TestLogging testLogging;
		
	protected:
		static void SetUpTestCase() {
			db = make_shared<RC2::TestPGDBConnection>();
			db->connect("");
		}
	};

	TEST_F(PGDBConnectionTest, connectionOpen) {
		ASSERT_TRUE(db->connectionOpen()) << "connection failed to open";
		long one = db->longFromQuery("select 1");
		ASSERT_EQ(one, 1);
	}
	
	TEST_F(PGDBConnectionTest, testNotifications) {
		DBResult listenRes = db->executeQuery("listen rcfile");
		ASSERT_TRUE(listenRes.commandOK());
		auto notifyResult = db->executeQuery("notify rcfile, 'foo'");
		ASSERT_TRUE(notifyResult.commandOK());
		db->consumeInput();
		string name, payload;
		ASSERT_TRUE(db->checkForNotification(name, payload));
		ASSERT_EQ(name, "rcfile");
		ASSERT_EQ(payload, "foo");
	}
	

	shared_ptr<RC2::TestPGDBConnection> PGDBConnectionTest::db;
//	TestLogging PGDBConnectionTest::testLogging;	
};
