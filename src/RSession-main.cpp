#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "common/RC2Utils.hpp"
#include "RSessionCallbacks.hpp"
#include "RSession.hpp"

using namespace std;

static RC2::RSessionCallbacks *callbacks=NULL;
static RC2::RSession *session=NULL;

void
exitHandler()
{
	std::cerr << "terminate called " << std::endl;
	abort();
}

void
signalHandler(evutil_socket_t socket, short flags, void* arg)
{
	std::cerr << "signal called " << flags << std::endl;
	abort();
}

int 
main(int argc, char** argv) 
{
	std::set_terminate(exitHandler);
	
	callbacks = new RC2::RSessionCallbacks();
	session = new RC2::RSession(callbacks);
	session->prepareForRunLoop();
	session->installExitHandler(signalHandler);
	
	
    return 0;
}

