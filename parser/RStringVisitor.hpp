#pragma once

#include "generated/RBaseVisitor.h"
#include <vector>
#include <string>

namespace RC2 {

class RStringVisitor: public RBaseVisitor {
public:
	RStringVisitor();
	
	std::vector<std::string> strings;

	virtual antlrcpp::Any visitStringRule(RParser::StringRuleContext *ctx) override;
	
};	
	
}; // end namespace
