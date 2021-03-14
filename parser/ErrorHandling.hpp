#pragma once

#include "antlr4-runtime.h"

class LexerErrorHandler: public antlr4::BaseErrorListener {
public:
		void syntaxError ( antlr4::Recognizer * recognizer, antlr4::Token * offendingSymbol, size_t line, size_t charPositionInLine, const std::string & msg, std::exception_ptr e ) override;
};

class ParserException: public std::exception {
public:
	ParserException(const std::string& msg, size_t line, size_t charPositionInLine)
		{
			std::ostringstream details;
			details << "Parser error at line " << line << ":" << charPositionInLine << ": " << msg;
			message = details.str();
		}
		
	const char* what() const noexcept { return message.c_str(); }
protected:
	std::string message;
};
