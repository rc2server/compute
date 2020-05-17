#include "RmdParser.hpp"
#include <iostream>
#include <memory>
#include "antlr4-runtime.h"
#include "generated/Rc2Lexer.h"
#include "generated/Rc2RawParser.h"
#include "generated/RFilter.h"
#include "generated/RLexer.h"
#include "generated/RParser.h"
#include "RStringVisitor.hpp"
#include "Rc2ParserListener.hpp"
#include "Chunk.hpp"
#include "common/ZeroInitializedStruct.hpp"
#include <openssl/sha.h>
#include "Rc2RFilter.hpp"

using std::vector;
using std::string;

string
RC2::CalculateSHA256Hash(string& input) 
{
	unsigned char obuf[SHA256_DIGEST_LENGTH + 1];
	bzero(obuf, SHA256_DIGEST_LENGTH + 1);
	SHA256((unsigned char*)input.data(), input.length(), obuf);
	unsigned char hexbuf[5];
	bzero(hexbuf, 5);
	string output = "0x";
	for(int i=0; i < sizeof(obuf); ++i) {
		sprintf((char*)hexbuf, "%02X", obuf[i]);
		output += (char*)hexbuf;
	}
	return output;
//	std::stringstream ss;
//	ss << std::hex << obuf << std::endl;
//	return ss.str();
}


namespace RC2 {

	struct ParserData: ZeroInitializedStruct {
		// necessary to preserve Chunks in listener
		unique_ptr<Rc2ParserListener>		listener;
		ErrorReporter						errorReporter;
	};
	
RmdParser::RmdParser() 
	: _impl(new ParserData())
{
}

RmdParser::~RmdParser() {
}

vector<Chunk*>
RmdParser::parseRmdSource ( std::string source ) {
	vector<std::reference_wrapper<Chunk>> chunks;

	antlr4::ANTLRInputStream input ( source );
	Rc2Lexer lexer ( &input );
	antlr4::CommonTokenStream tokens ( &lexer );
	auto rawTokens = tokens.getTokens();
	Rc2RFilter filter ( &tokens );
	filter.stream();
	tokens.reset();

	Rc2RawParser parser ( &tokens );
	parser.removeErrorListeners();
	auto doc = parser.document();
	antlr4::tree::ParseTreeWalker walker;
	_impl->listener = std::make_unique<Rc2ParserListener>(&(_impl->errorReporter));
	walker.walk ( _impl->listener.get(), doc );
	return _impl->listener->chunks();
}


std::vector<std::string>
RmdParser::stringsInRCode ( std::string source ) {
	antlr4::ANTLRInputStream input ( source );
	RLexer lexer ( &input );
	antlr4::CommonTokenStream tokens ( &lexer );
	Rc2RFilter filter ( &tokens );
	filter.stream();
	tokens.reset();

	RParser parser ( &tokens );
	antlr4::tree::ParseTree *tree = parser.prog();
	RStringVisitor visitor;
	visitor.visit ( tree );

	return visitor.strings;
}

}; // end namespace
