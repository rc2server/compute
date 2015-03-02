/* 
 * File:   main.cpp
 * Author: mlilback
 *
 * Created on February 26, 2015, 2:47 PM
 */

#include <cstdlib>
#include <iostream>
#include "common/RC2Utils.hpp"
#include "RSessionCallbacks.hpp"

using namespace std;

static RC2::RSessionCallbacks *callbacks=NULL;

void
exitHandler()
{
	std::cerr << "terminate called " << std::endl;
	abort();
}

int 
main(int argc, char** argv) 
{
	std::set_terminate(exitHandler);
	
	callbacks = new RC2::RSessionCallbacks();
	
    return 0;
}

