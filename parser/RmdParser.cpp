#include "RmdParser.hpp"
#include <iostream>
#include "antlr4-runtime.h"
#include "generated/Rc2Lexer.h"
#include "generated/Rc2RawParser.h"
#include "generated/RFilter.h"
#include "generated/RLexer.h"
#include "generated/RParser.h"
#include "RStringVisitor.hpp"
#include "Rc2ParserListener.hpp"
#include "Chunk.hpp"

using std::vector;
using std::string;

namespace RC2 {

RmdParser::RmdParser() {
}
	
RmdParser::~RmdParser() {
}
	
vector<std::reference_wrapper<Chunk>> 
RmdParser::parseRmdSource(std::string source)
{
	vector<std::reference_wrapper<Chunk>> chunks;
	
	antlr4::ANTLRInputStream input(source);
	Rc2Lexer lexer(&input);

	auto allTokens = lexer.getAllTokens();
	auto vocab = lexer.getVocabulary();
	for (auto aToken = allTokens.begin(); aToken != allTokens.end(); ++aToken) {
		auto name = vocab.getDisplayName((*aToken)->getType());
		std::cout << name << " = " << (*aToken)->getText() << std::endl;
	}
	lexer.reset();
	
	antlr4::CommonTokenStream tokens(&lexer);

	RFilter filter(&tokens);
	filter.stream();
	tokens.reset();

	Rc2RawParser parser(&tokens);
	auto doc = parser.document();
	antlr4::tree::ParseTreeWalker walker;
	ErrorReporter errors;
	Rc2ParserListener listener(&errors);
	walker.walk(&listener, doc);
	
	return listener.chunks();
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
	
//	std::cout << "strings:" << std::endl;
//	for (auto i = visitor.strings.begin(); i != visitor.strings.end(); ++i)
//		std::cout << *i << std::endl;
	
	return visitor.strings;
}

}; // end namespace
