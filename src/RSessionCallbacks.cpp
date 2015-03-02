#include "RSessionCallbacks.hpp"
#include <iostream>

namespace RC2 {

void
RSessionCallbacks::ShowMessage(const char *message)
{
	std::cout << "show message:" << message << std::endl;
}

void 
RSessionCallbacks::WriteConsole( const std::string& line, int type )
{
	_writeLambda(line, type > 0);
}

};
