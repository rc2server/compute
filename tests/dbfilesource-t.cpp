#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include "../src/DBFileSource.hpp"
#include "common/RC2Utils.hpp"


using namespace std;

namespace RC2 {
namespace testing {

	TEST(DBFileSourceTest, basicTest)
	{
		const long wspaceId = 1;
		PGconn *dbcon = PQconnectdb("postgresql://rc2@localhost/rc2test?application_name=rsession");
		DBFileSource dbsrc;
		TemporaryDirectory tmpDir;
		dbsrc.initializeSource(dbcon, wspaceId);
		dbsrc.setWorkingDir(tmpDir.getPath());
		
		char msg[255];
		snprintf(msg, 255, "where wspaceid = %ld", wspaceId);
		dbsrc.loadFiles(msg);

		ASSERT_TRUE(dbsrc.filesById_.size() == 2);
//		ASSERT_STREQ(json.c_str(), ib.popCurrentMessage().c_str());

		PQfinish(dbcon);
	}

};
};
