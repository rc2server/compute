#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <memory>
#include "../src/InputBufferManager.hpp"
#include "common/RC2Utils.hpp"


using namespace std;

namespace RC2 {
namespace testing {

	void
	setBufferToJson(evbuffer *buffer, string json)
	{
		size_t sz = evbuffer_get_length(buffer);
		if (sz > 0)
			evbuffer_drain(buffer, sz);
		uint32_t val = htonl(kRSessionMagicNumber);
		evbuffer_add(buffer, &val, sizeof(val));
		val = htonl(json.length());
		evbuffer_add(buffer, &val, sizeof(val));
		evbuffer_add(buffer, json.c_str(), json.length());
	}

	TEST(InputBufferTest, singleInputTest)
	{
		InputBufferManager ib;
		evbuffer *buffer = evbuffer_new();
		string json = "{ \"a\":22, \"name\":\"foo\"}";
		setBufferToJson(buffer, json);
		ib.appendData(buffer);
		
		ASSERT_TRUE(ib.hasCompleteMessage());
		ASSERT_STREQ(json.c_str(), ib.popCurrentMessage().c_str());
	}

	TEST(InputBufferTest, doubleInputTest)
	{
		InputBufferManager ib;
		evbuffer *buffer = evbuffer_new();
		string json = "{ \"a\":22, \"name\":\"foo\",";
		string json2 = "\"b\":11, \"stype\":\"bar\"}";
		setBufferToJson(buffer, json);
		ib.appendData(buffer);
		setBufferToJson(buffer, json2);
		ib.appendData(buffer);
		
		json += json2;
		ASSERT_TRUE(ib.hasCompleteMessage());
		ASSERT_STREQ(json.c_str(), ib.popCurrentMessage().c_str());
	}

};
};
