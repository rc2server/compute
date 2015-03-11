#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "../src/RServer.hpp"

using std::cerr;
using std::endl;

namespace RC2 {
namespace testing {

	TEST(RServerTest, listenOnDefault)
	{
		cerr << "starting" << endl;
		ASSERT_EQ(1, 2-1);
		cerr << "creating server" << endl;
		RServer server;
		cerr << "server created" << endl;
	}

};
};
