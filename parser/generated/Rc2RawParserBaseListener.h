
// Generated from ../../Rc2Parser/Rc2RawParser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "Rc2RawParserListener.h"


/**
 * This class provides an empty implementation of Rc2RawParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  Rc2RawParserBaseListener : public Rc2RawParserListener {
public:

  virtual void enterDocument(Rc2RawParser::DocumentContext * /*ctx*/) override { }
  virtual void exitDocument(Rc2RawParser::DocumentContext * /*ctx*/) override { }

  virtual void enterChunk(Rc2RawParser::ChunkContext * /*ctx*/) override { }
  virtual void exitChunk(Rc2RawParser::ChunkContext * /*ctx*/) override { }

  virtual void enterMdown(Rc2RawParser::MdownContext * /*ctx*/) override { }
  virtual void exitMdown(Rc2RawParser::MdownContext * /*ctx*/) override { }

  virtual void enterCode(Rc2RawParser::CodeContext * /*ctx*/) override { }
  virtual void exitCode(Rc2RawParser::CodeContext * /*ctx*/) override { }

  virtual void enterInlineCode(Rc2RawParser::InlineCodeContext * /*ctx*/) override { }
  virtual void exitInlineCode(Rc2RawParser::InlineCodeContext * /*ctx*/) override { }

  virtual void enterInlineEQ(Rc2RawParser::InlineEQContext * /*ctx*/) override { }
  virtual void exitInlineEQ(Rc2RawParser::InlineEQContext * /*ctx*/) override { }

  virtual void enterEquation(Rc2RawParser::EquationContext * /*ctx*/) override { }
  virtual void exitEquation(Rc2RawParser::EquationContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

