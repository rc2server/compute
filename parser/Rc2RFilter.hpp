#pragma once
#include <memory>
#include "Rc2RFilter.hpp"
#include "generated/RFilter.h"
#include "ANTLRErrorStrategy.h"
#include "ANTLRErrorListener.h"
#include "DefaultErrorStrategy.h"
#include "BaseErrorListener.h"
#include "generated/RFilter.h"
#include "Parser.h"

class FilterErrorStrategy: public antlr4::DefaultErrorStrategy {
public:
	void reportInputMismatch ( antlr4::Parser * recognizer, const antlr4::InputMismatchException & e ) override;
};

class FilterErrorListener : public antlr4::BaseErrorListener {
public:
	void syntaxError ( antlr4::Recognizer * recognizer, antlr4::Token * offendingSymbol, size_t line, size_t charPositionInLine, const std::string & msg, std::exception_ptr e );
};

class Rc2RFilter: public RFilter {
public:
	Rc2RFilter(antlr4::TokenStream *tokens);
	
	std::shared_ptr<FilterErrorStrategy> strategy_;
	FilterErrorListener listener_;
};
