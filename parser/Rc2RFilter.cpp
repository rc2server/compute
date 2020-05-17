#include <memory>
#include "Rc2RFilter.hpp"

Rc2RFilter::Rc2RFilter(antlr4::TokenStream *tokens)
	: RFilter(tokens)
{
	strategy_ = std::make_shared<FilterErrorStrategy>();
	removeErrorListeners();
	addErrorListener(&listener_);
	auto ref = std::ref(strategy_);
 	setErrorHandler(strategy_);
}


void FilterErrorListener::syntaxError ( antlr4::Recognizer* recognizer, antlr4::Token* offendingSymbol, size_t line, size_t charPositionInLine, const std::string& msg, std::exception_ptr e )
{
	// should log
}

void FilterErrorStrategy::reportInputMismatch ( antlr4::Parser* recognizer, const antlr4::InputMismatchException& e ) {
	std::string tokenStr = getSymbolText(e.getOffendingToken());
	if(tokenStr == "$" || tokenStr == "$$" || tokenStr == "```\n")
		return;
	if(tokenStr == "\n") {
		//if prceeded by a backtick then it is end of inline code and ok
		if(e.getCtx()->getText() == "`")
			return;
	}
	DefaultErrorStrategy::reportInputMismatch(recognizer, e);
}
