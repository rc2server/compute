
// Generated from Rc2Parser/Rc2RawParser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "Rc2RawParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by Rc2RawParser.
 */
class  Rc2RawParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterDocument(Rc2RawParser::DocumentContext *ctx) = 0;
  virtual void exitDocument(Rc2RawParser::DocumentContext *ctx) = 0;

  virtual void enterChunk(Rc2RawParser::ChunkContext *ctx) = 0;
  virtual void exitChunk(Rc2RawParser::ChunkContext *ctx) = 0;

  virtual void enterMdown(Rc2RawParser::MdownContext *ctx) = 0;
  virtual void exitMdown(Rc2RawParser::MdownContext *ctx) = 0;

  virtual void enterCode(Rc2RawParser::CodeContext *ctx) = 0;
  virtual void exitCode(Rc2RawParser::CodeContext *ctx) = 0;

  virtual void enterInlineCode(Rc2RawParser::InlineCodeContext *ctx) = 0;
  virtual void exitInlineCode(Rc2RawParser::InlineCodeContext *ctx) = 0;

  virtual void enterInlineEQ(Rc2RawParser::InlineEQContext *ctx) = 0;
  virtual void exitInlineEQ(Rc2RawParser::InlineEQContext *ctx) = 0;

  virtual void enterEquation(Rc2RawParser::EquationContext *ctx) = 0;
  virtual void exitEquation(Rc2RawParser::EquationContext *ctx) = 0;


};

