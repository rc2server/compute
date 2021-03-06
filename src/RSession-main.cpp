#include <cstdlib>
#include <iostream>
#include <signal.h>
#include "RC2Logging.h"
#include "common/RC2Utils.hpp"
#include "RSessionCallbacks.hpp"
#include "RSession.hpp"
#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

static RC2::RSessionCallbacks *callbacks=nullptr;
static RC2::RSession *session=nullptr;

void
exitHandler()
{
	cerr << "terminate called " << endl;
	abort();
}

void
signalHandler(short flags)
{
	cerr << "signal called " << flags << endl;
	abort();
}

int 
main(int argc, char** argv) 
{
	std::set_terminate(exitHandler);

	if (fs::exists("/usr/lib/R"))
		setenv("R_HOME", "/usr/lib/R", 0);
	else
		setenv("R_HOME", "/usr/local/lib/R", 0);
	
	callbacks = new RC2::RSessionCallbacks();
	session = new RC2::RSession(callbacks);
	session->parseArguments(argc, argv);
	session->prepareForRunLoop();
	session->installExitHandler(signalHandler);
	session->startEventLoop();
	cerr << "event loop exited" << endl;
    return 0;
}

