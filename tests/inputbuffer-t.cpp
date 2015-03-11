#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include "../src/InputBufferManager.hpp"

using std::cerr;
using std::endl;

namespace RC2 {
namespace testing {

	TEST(InputBufferTest, basicTest)
	{
		InputBufferManager ib;
		evbuffer *buffer = evbuffer_new();
		
		
		ASSERT_EQ(1, 2-1);
	}

};
};
