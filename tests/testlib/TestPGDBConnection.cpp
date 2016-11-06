#include <gtest/gtest.h>
#include <iostream>
#include "TestPGDBConnection.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#include "../common/RC2Utils.hpp"
#include "json.hpp"

namespace fs = boost::filesystem;
using json = nlohmann::json;

RC2::TestPGDBConnection::TestPGDBConnection()
{

}

void RC2::TestPGDBConnection::connect (std::string connectStr)
{
	std::string strToTry = readConfig();
	if (strToTry.length() > 0) {
		try {
			PGDBConnection::connect(strToTry);
			return;
		} catch (...) {
			ADD_FAILURE() << "failed to connect to db from config file";
		}
	}
	try {
		PGDBConnection::connect("postgresql://rc2@localhost/rc2test?application_name=rc2+unit+tests&sslmode=disable");
		return;
	} catch (...) {
		ADD_FAILURE() << "failed to connect to rc2unittest db";
	} 
	return RC2::PGDBConnection::connect(connectStr);
}

std::string RC2::TestPGDBConnection::readConfig()
{
	std::string results;
	//if run from command line, likely not in tests directory
	fs::path configPath("tests/testdbconfig.json");
	if (!fs::exists(configPath)) {
		//assume we are in the tests directory
		configPath = "testdbconfig.json";
	}
	if (fs::exists(configPath)) {
		std::string contents = SlurpFile(configPath.c_str());
		json doc;
		try {
			std::istringstream istr(contents);
			istr >> doc;
			results = doc["connectString"];
		} catch (std::invalid_argument &iae) {
			ADD_FAILURE() << "parse exception reading testdbconfig.json:" << iae.what();
		} catch (std::exception &ex) {
			ADD_FAILURE() << "unknown exception parsing testdbconfig.json:" << ex.what();
		}
	} else {
		std::cerr << "db config file does not exist: " << fs::current_path() << std::endl;
	}
	return results;
}
