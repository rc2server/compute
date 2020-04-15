
// Generated from Rc2Parser/R.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RListener.h"


/**
 * This class provides an empty implementation of RListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  RBaseListener : public RListener {
public:

  virtual void enterProg(RParser::ProgContext * /*ctx*/) override { }
  virtual void exitProg(RParser::ProgContext * /*ctx*/) override { }

  virtual void enterAssignment(RParser::AssignmentContext * /*ctx*/) override { }
  virtual void exitAssignment(RParser::AssignmentContext * /*ctx*/) override { }

  virtual void enterSingleExp(RParser::SingleExpContext * /*ctx*/) override { }
  virtual void exitSingleExp(RParser::SingleExpContext * /*ctx*/) override { }

  virtual void enterWhileLoop(RParser::WhileLoopContext * /*ctx*/) override { }
  virtual void exitWhileLoop(RParser::WhileLoopContext * /*ctx*/) override { }

  virtual void enterHelpRequest(RParser::HelpRequestContext * /*ctx*/) override { }
  virtual void exitHelpRequest(RParser::HelpRequestContext * /*ctx*/) override { }

  virtual void enterRepeatLoop(RParser::RepeatLoopContext * /*ctx*/) override { }
  virtual void exitRepeatLoop(RParser::RepeatLoopContext * /*ctx*/) override { }

  virtual void enterExpTildeExp(RParser::ExpTildeExpContext * /*ctx*/) override { }
  virtual void exitExpTildeExp(RParser::ExpTildeExpContext * /*ctx*/) override { }

  virtual void enterListExp(RParser::ListExpContext * /*ctx*/) override { }
  virtual void exitListExp(RParser::ListExpContext * /*ctx*/) override { }

  virtual void enterDescendant(RParser::DescendantContext * /*ctx*/) override { }
  virtual void exitDescendant(RParser::DescendantContext * /*ctx*/) override { }

  virtual void enterHouse(RParser::HouseContext * /*ctx*/) override { }
  virtual void exitHouse(RParser::HouseContext * /*ctx*/) override { }

  virtual void enterListRef(RParser::ListRefContext * /*ctx*/) override { }
  virtual void exitListRef(RParser::ListRefContext * /*ctx*/) override { }

  virtual void enterMulDiv(RParser::MulDivContext * /*ctx*/) override { }
  virtual void exitMulDiv(RParser::MulDivContext * /*ctx*/) override { }

  virtual void enterHexRule(RParser::HexRuleContext * /*ctx*/) override { }
  virtual void exitHexRule(RParser::HexRuleContext * /*ctx*/) override { }

  virtual void enterArray(RParser::ArrayContext * /*ctx*/) override { }
  virtual void exitArray(RParser::ArrayContext * /*ctx*/) override { }

  virtual void enterStringRule(RParser::StringRuleContext * /*ctx*/) override { }
  virtual void exitStringRule(RParser::StringRuleContext * /*ctx*/) override { }

  virtual void enterTildeExp(RParser::TildeExpContext * /*ctx*/) override { }
  virtual void exitTildeExp(RParser::TildeExpContext * /*ctx*/) override { }

  virtual void enterCompoundStmt(RParser::CompoundStmtContext * /*ctx*/) override { }
  virtual void exitCompoundStmt(RParser::CompoundStmtContext * /*ctx*/) override { }

  virtual void enterNumRule(RParser::NumRuleContext * /*ctx*/) override { }
  virtual void exitNumRule(RParser::NumRuleContext * /*ctx*/) override { }

  virtual void enterKeywordRule(RParser::KeywordRuleContext * /*ctx*/) override { }
  virtual void exitKeywordRule(RParser::KeywordRuleContext * /*ctx*/) override { }

  virtual void enterForLoop(RParser::ForLoopContext * /*ctx*/) override { }
  virtual void exitForLoop(RParser::ForLoopContext * /*ctx*/) override { }

  virtual void enterIdRule(RParser::IdRuleContext * /*ctx*/) override { }
  virtual void exitIdRule(RParser::IdRuleContext * /*ctx*/) override { }

  virtual void enterPlusMinus(RParser::PlusMinusContext * /*ctx*/) override { }
  virtual void exitPlusMinus(RParser::PlusMinusContext * /*ctx*/) override { }

  virtual void enterDefineFunc(RParser::DefineFuncContext * /*ctx*/) override { }
  virtual void exitDefineFunc(RParser::DefineFuncContext * /*ctx*/) override { }

  virtual void enterComparison(RParser::ComparisonContext * /*ctx*/) override { }
  virtual void exitComparison(RParser::ComparisonContext * /*ctx*/) override { }

  virtual void enterAssignExp(RParser::AssignExpContext * /*ctx*/) override { }
  virtual void exitAssignExp(RParser::AssignExpContext * /*ctx*/) override { }

  virtual void enterOrExpr(RParser::OrExprContext * /*ctx*/) override { }
  virtual void exitOrExpr(RParser::OrExprContext * /*ctx*/) override { }

  virtual void enterCallFunc(RParser::CallFuncContext * /*ctx*/) override { }
  virtual void exitCallFunc(RParser::CallFuncContext * /*ctx*/) override { }

  virtual void enterIfelseStmt(RParser::IfelseStmtContext * /*ctx*/) override { }
  virtual void exitIfelseStmt(RParser::IfelseStmtContext * /*ctx*/) override { }

  virtual void enterNotExpr(RParser::NotExprContext * /*ctx*/) override { }
  virtual void exitNotExpr(RParser::NotExprContext * /*ctx*/) override { }

  virtual void enterIfStmt(RParser::IfStmtContext * /*ctx*/) override { }
  virtual void exitIfStmt(RParser::IfStmtContext * /*ctx*/) override { }

  virtual void enterUserExpr(RParser::UserExprContext * /*ctx*/) override { }
  virtual void exitUserExpr(RParser::UserExprContext * /*ctx*/) override { }

  virtual void enterColon(RParser::ColonContext * /*ctx*/) override { }
  virtual void exitColon(RParser::ColonContext * /*ctx*/) override { }

  virtual void enterComment(RParser::CommentContext * /*ctx*/) override { }
  virtual void exitComment(RParser::CommentContext * /*ctx*/) override { }

  virtual void enterDollarOrAt(RParser::DollarOrAtContext * /*ctx*/) override { }
  virtual void exitDollarOrAt(RParser::DollarOrAtContext * /*ctx*/) override { }

  virtual void enterPlusNeg(RParser::PlusNegContext * /*ctx*/) override { }
  virtual void exitPlusNeg(RParser::PlusNegContext * /*ctx*/) override { }

  virtual void enterAndExpr(RParser::AndExprContext * /*ctx*/) override { }
  virtual void exitAndExpr(RParser::AndExprContext * /*ctx*/) override { }

  virtual void enterExprlist(RParser::ExprlistContext * /*ctx*/) override { }
  virtual void exitExprlist(RParser::ExprlistContext * /*ctx*/) override { }

  virtual void enterFormlist(RParser::FormlistContext * /*ctx*/) override { }
  virtual void exitFormlist(RParser::FormlistContext * /*ctx*/) override { }

  virtual void enterForm(RParser::FormContext * /*ctx*/) override { }
  virtual void exitForm(RParser::FormContext * /*ctx*/) override { }

  virtual void enterSublist(RParser::SublistContext * /*ctx*/) override { }
  virtual void exitSublist(RParser::SublistContext * /*ctx*/) override { }

  virtual void enterSub(RParser::SubContext * /*ctx*/) override { }
  virtual void exitSub(RParser::SubContext * /*ctx*/) override { }

  virtual void enterFunctionCall(RParser::FunctionCallContext * /*ctx*/) override { }
  virtual void exitFunctionCall(RParser::FunctionCallContext * /*ctx*/) override { }

  virtual void enterRcomparison(RParser::RcomparisonContext * /*ctx*/) override { }
  virtual void exitRcomparison(RParser::RcomparisonContext * /*ctx*/) override { }

  virtual void enterAssignOp(RParser::AssignOpContext * /*ctx*/) override { }
  virtual void exitAssignOp(RParser::AssignOpContext * /*ctx*/) override { }

  virtual void enterNumber(RParser::NumberContext * /*ctx*/) override { }
  virtual void exitNumber(RParser::NumberContext * /*ctx*/) override { }

  virtual void enterRcomment(RParser::RcommentContext * /*ctx*/) override { }
  virtual void exitRcomment(RParser::RcommentContext * /*ctx*/) override { }

  virtual void enterIfKeyWord(RParser::IfKeyWordContext * /*ctx*/) override { }
  virtual void exitIfKeyWord(RParser::IfKeyWordContext * /*ctx*/) override { }

  virtual void enterElseKeyword(RParser::ElseKeywordContext * /*ctx*/) override { }
  virtual void exitElseKeyword(RParser::ElseKeywordContext * /*ctx*/) override { }

  virtual void enterRepeatKeyword(RParser::RepeatKeywordContext * /*ctx*/) override { }
  virtual void exitRepeatKeyword(RParser::RepeatKeywordContext * /*ctx*/) override { }

  virtual void enterWhileKeyword(RParser::WhileKeywordContext * /*ctx*/) override { }
  virtual void exitWhileKeyword(RParser::WhileKeywordContext * /*ctx*/) override { }

  virtual void enterForKeyword(RParser::ForKeywordContext * /*ctx*/) override { }
  virtual void exitForKeyword(RParser::ForKeywordContext * /*ctx*/) override { }

  virtual void enterBreakKeyword(RParser::BreakKeywordContext * /*ctx*/) override { }
  virtual void exitBreakKeyword(RParser::BreakKeywordContext * /*ctx*/) override { }

  virtual void enterInKeyword(RParser::InKeywordContext * /*ctx*/) override { }
  virtual void exitInKeyword(RParser::InKeywordContext * /*ctx*/) override { }

  virtual void enterHexKeyword(RParser::HexKeywordContext * /*ctx*/) override { }
  virtual void exitHexKeyword(RParser::HexKeywordContext * /*ctx*/) override { }

  virtual void enterFunctionKeyword(RParser::FunctionKeywordContext * /*ctx*/) override { }
  virtual void exitFunctionKeyword(RParser::FunctionKeywordContext * /*ctx*/) override { }

  virtual void enterTrueKeyword(RParser::TrueKeywordContext * /*ctx*/) override { }
  virtual void exitTrueKeyword(RParser::TrueKeywordContext * /*ctx*/) override { }

  virtual void enterFalseKeyword(RParser::FalseKeywordContext * /*ctx*/) override { }
  virtual void exitFalseKeyword(RParser::FalseKeywordContext * /*ctx*/) override { }

  virtual void enterNaKeyword(RParser::NaKeywordContext * /*ctx*/) override { }
  virtual void exitNaKeyword(RParser::NaKeywordContext * /*ctx*/) override { }

  virtual void enterNanKeyword(RParser::NanKeywordContext * /*ctx*/) override { }
  virtual void exitNanKeyword(RParser::NanKeywordContext * /*ctx*/) override { }

  virtual void enterInfKeyword(RParser::InfKeywordContext * /*ctx*/) override { }
  virtual void exitInfKeyword(RParser::InfKeywordContext * /*ctx*/) override { }

  virtual void enterNullKeyword(RParser::NullKeywordContext * /*ctx*/) override { }
  virtual void exitNullKeyword(RParser::NullKeywordContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

