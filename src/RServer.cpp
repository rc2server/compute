/* 
 * File:   main.cpp
 * Author: mlilback
 *
 * Created on February 26, 2015, 2:47 PM
 */

#include <cstdlib>
#include "RServer.hpp"
#include "tclap/CmdLine.h"

using namespace std;
//using namespace RC2;

RServer::RServer()
{
	struct event_config *config = event_config_new();
	event_config_require_features(config, EV_FEATURE_FDS);
	_eventBase = event_base_new_with_config(config);
	event_config_free(config);
}

RServer::~RServer()
{
	event_base_free(_eventBase);
}

void
RServer::startRunLoop()
{
	event_base_dispatch(_eventBase);
}

bool
RServer::parseArgs(int argc, char** argv)
{
	try {
		TCLAP::CmdLine cmdLine("Listen for rcompute connections", ' ', "0.1");
		
		TCLAP::ValueArg<uint32_t> portArg("p", "port", "port to listen on", 
			false, 7714, "port", cmdLine);
		
		TCLAP::SwitchArg switchArg("v", "verbose", "enable logging", cmdLine);
			
		cmdLine.parse(argc, argv);
		_port = portArg.getValue();
		_verbose = switchArg.getValue();
		
	} catch (TCLAP::ArgException &e) {
		std::cerr << "error:" << e.error() << std::endl;
	}
	return true;
}

int 
main(int argc, char** argv) 
{
	RServer server;
	server.parseArgs(argc, argv);
	server.startRunLoop();
    return 0;
}

