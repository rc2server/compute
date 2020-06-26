#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include "../src/RC2Logging.h"
#include "../src/DBFileSource.hpp"
#include "common/RC2Utils.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "testlib/TestPGDBConnection.hpp"
#include "testlib/TestingSession.hpp"


using namespace std;
namespace fs = boost::filesystem;

namespace testing {

	class DBSourceTest: public ::testing::Test {
	public:
		RC2::TemporaryDirectory tmpDir;
		RC2::DBFileSource source;
		shared_ptr<RC2::TestPGDBConnection> db;
		
	protected:
		virtual void SetUp() {
			using namespace RC2;
			db = make_shared<RC2::TestPGDBConnection>();
			db->connect("");
			source.initializeSource(db, 1);
			source.setWorkingDir(tmpDir.getPath());
		}
		
		virtual void TearDown() {
			//nuke the fake .RData we created
			db->executeQuery("delete from rcworkspacedata where id = 1");
			db = nullptr;
		}
	};
	
	TEST_F(DBSourceTest, loadRData)
	{
		string data("foo\nbar\n");
		fs::path path = this->tmpDir.getPath();
		path += "/.RData";
		ofstream ofs(path.c_str());
		ofs << data;
		ofs.close();
		
		source.saveRData();
		fs:remove(path);
		source.loadRData();
		ASSERT_EQ(data.length(), fs::file_size(path));
		
		ifstream ifs(path.c_str());
		string line, text;
		while (getline(ifs, line)) {
			text += line + "\n";
		}
		ifs.close();
		ASSERT_EQ(data, text);
	}

};
