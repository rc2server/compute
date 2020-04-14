// GCC + Antlr results in an error message in ATN.cpp. This is used to disable those warnings
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"

#include "RmdParser.hpp"
#include "generated/Rc2Lexer.h"
#include "generated/Rc2RawParser.h"

#pragma GCC diagnostic pop
namespace RC2 {

	RmdParser::RmdParser() {
	}
	
	RmdParser::~RmdParser() {
	}
	
};
