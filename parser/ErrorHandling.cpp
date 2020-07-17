#include "ErrorHandling.hpp"

void LexerErrorHandler::syntaxError ( antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e ) 
{
	throw new std::exception();
}
