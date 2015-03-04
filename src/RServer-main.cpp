#include <iostream>
#include "RServer.hpp"

int 
main(int argc, char** argv) 
{
	RServer server;
	server.parseArgs(argc, argv);
	server.startRunLoop();
    return 0;
}

