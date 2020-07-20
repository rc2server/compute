#pragma once

#include "antlr4-runtime.h"

class LexerErrorHandler: public antlr4::BaseErrorListener {
public:
		void syntaxError ( antlr4::Recognizer * recognizer, antlr4::Token * offendingSymbol, size_t line, size_t charPositionInLine, const std::string & msg, std::exception_ptr e ) override;
};

class ParserException: public std::exception {
public:
	ParserException(int eCode)
		: code(eCode)
		{}
		
	int getCode() const { return code; }
protected:
	int code;
};

#define kParserError_SyntaxError 701
