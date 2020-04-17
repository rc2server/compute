#include "RmdParser.hpp"
#include <iostream>
#include "antlr4-runtime.h"
#include "generated/Rc2Lexer.h"
#include "generated/Rc2RawParser.h"
#include "generated/RFilter.h"
#include "generated/RLexer.h"
#include "generated/RParser.h"
#include "RStringVisitor.hpp"

namespace RC2 {

RmdParser::RmdParser() {
}
	
RmdParser::~RmdParser() {
}
	
std::vector<std::string> 
RmdParser::stringsInRCode(std::string source)
{
	antlr4::ANTLRInputStream input(source);
	RLexer lexer(&input);
	antlr4::CommonTokenStream tokens(&lexer);
	RFilter filter(&tokens);
	filter.stream();
	tokens.reset();
	
	RParser parser(&tokens);
	antlr4::tree::ParseTree *tree = parser.prog();
	RStringVisitor visitor;
	visitor.visit(tree);
	
	std::cout << "strings:" << std::endl;
	for (auto i = visitor.strings.begin(); i != visitor.strings.end(); ++i)
		std::cout << *i << std::endl;
	
	return visitor.strings;
}

}; // end namespace
