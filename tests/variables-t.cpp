#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <queue>
#include <thread>
#include <glog/logging.h>
#include "common/RC2Utils.hpp"
#include "testlib/TestingSession.hpp"

using json = nlohmann::json;
using namespace std;

namespace testing {

	class VarTest : public BaseSessionTest {
		virtual void pureVirtual() {}
	};
	
	TEST_F(VarTest, getVariable)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"testVar<-22\"}");
		session->emptyMessages();
		session->doJson("{\"msg\":\"getVariable\", \"argument\":\"testVar\"}");
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		ASSERT_TRUE(results["msg"] == "variablevalue");
		ASSERT_TRUE(results["value"]["value"][0] == 22);
	}

	TEST_F(VarTest, listVariables)
	{
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"rm(list=ls())\"}");
		session->doJson("{\"msg\":\"execScript\", \"argument\":\"x<-22;y<-11\"}");
		session->emptyMessages();
		session->doJson("{\"msg\":\"listVariables\", \"argument\":\"\", \"watch\":true}");
		ASSERT_EQ(session->_messages.size(), 1);
		json results = session->popMessage();
		auto elems = mapArayToMapMap(results["variables"]["values"]);
		ASSERT_TRUE(results["msg"] == "variableupdate");
		ASSERT_EQ(elems["x"], 22);
		ASSERT_EQ(elems["y"], 11);
	}

};


