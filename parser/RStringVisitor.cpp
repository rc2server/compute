#include "RStringVisitor.hpp"

namespace RC2 {

RStringVisitor::RStringVisitor()
{
}

antlrcpp::Any 
RStringVisitor::visitStringRule(RParser::StringRuleContext *ctx) {
	auto str = ctx->getText();
	// trim the surrounding quotes
	strings.push_back(str.substr(1, str.size() - 2));
	return RBaseVisitor::visitStringRule(ctx);
}

}; // end namespace
