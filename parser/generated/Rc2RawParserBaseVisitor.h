
// Generated from Rc2Parser/Rc2RawParser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "Rc2RawParserVisitor.h"


/**
 * This class provides an empty implementation of Rc2RawParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  Rc2RawParserBaseVisitor : public Rc2RawParserVisitor {
public:

  virtual antlrcpp::Any visitDocument(Rc2RawParser::DocumentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitChunk(Rc2RawParser::ChunkContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMdown(Rc2RawParser::MdownContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCode(Rc2RawParser::CodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInlineCode(Rc2RawParser::InlineCodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInlineEQ(Rc2RawParser::InlineEQContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEquation(Rc2RawParser::EquationContext *ctx) override {
    return visitChildren(ctx);
  }


};

