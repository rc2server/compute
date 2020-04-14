
// Generated from ../../Rc2Parser/Rc2RawParser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "Rc2RawParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by Rc2RawParser.
 */
class  Rc2RawParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by Rc2RawParser.
   */
    virtual antlrcpp::Any visitDocument(Rc2RawParser::DocumentContext *context) = 0;

    virtual antlrcpp::Any visitChunk(Rc2RawParser::ChunkContext *context) = 0;

    virtual antlrcpp::Any visitMdown(Rc2RawParser::MdownContext *context) = 0;

    virtual antlrcpp::Any visitCode(Rc2RawParser::CodeContext *context) = 0;

    virtual antlrcpp::Any visitInlineCode(Rc2RawParser::InlineCodeContext *context) = 0;

    virtual antlrcpp::Any visitInlineEQ(Rc2RawParser::InlineEQContext *context) = 0;

    virtual antlrcpp::Any visitEquation(Rc2RawParser::EquationContext *context) = 0;


};

