
// Generated from Rc2Parser/R.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by RParser.
 */
class  RListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterProg(RParser::ProgContext *ctx) = 0;
  virtual void exitProg(RParser::ProgContext *ctx) = 0;

  virtual void enterAssignment(RParser::AssignmentContext *ctx) = 0;
  virtual void exitAssignment(RParser::AssignmentContext *ctx) = 0;

  virtual void enterSingleExp(RParser::SingleExpContext *ctx) = 0;
  virtual void exitSingleExp(RParser::SingleExpContext *ctx) = 0;

  virtual void enterWhileLoop(RParser::WhileLoopContext *ctx) = 0;
  virtual void exitWhileLoop(RParser::WhileLoopContext *ctx) = 0;

  virtual void enterHelpRequest(RParser::HelpRequestContext *ctx) = 0;
  virtual void exitHelpRequest(RParser::HelpRequestContext *ctx) = 0;

  virtual void enterRepeatLoop(RParser::RepeatLoopContext *ctx) = 0;
  virtual void exitRepeatLoop(RParser::RepeatLoopContext *ctx) = 0;

  virtual void enterExpTildeExp(RParser::ExpTildeExpContext *ctx) = 0;
  virtual void exitExpTildeExp(RParser::ExpTildeExpContext *ctx) = 0;

  virtual void enterListExp(RParser::ListExpContext *ctx) = 0;
  virtual void exitListExp(RParser::ListExpContext *ctx) = 0;

  virtual void enterDescendant(RParser::DescendantContext *ctx) = 0;
  virtual void exitDescendant(RParser::DescendantContext *ctx) = 0;

  virtual void enterHouse(RParser::HouseContext *ctx) = 0;
  virtual void exitHouse(RParser::HouseContext *ctx) = 0;

  virtual void enterListRef(RParser::ListRefContext *ctx) = 0;
  virtual void exitListRef(RParser::ListRefContext *ctx) = 0;

  virtual void enterMulDiv(RParser::MulDivContext *ctx) = 0;
  virtual void exitMulDiv(RParser::MulDivContext *ctx) = 0;

  virtual void enterHexRule(RParser::HexRuleContext *ctx) = 0;
  virtual void exitHexRule(RParser::HexRuleContext *ctx) = 0;

  virtual void enterArray(RParser::ArrayContext *ctx) = 0;
  virtual void exitArray(RParser::ArrayContext *ctx) = 0;

  virtual void enterStringRule(RParser::StringRuleContext *ctx) = 0;
  virtual void exitStringRule(RParser::StringRuleContext *ctx) = 0;

  virtual void enterTildeExp(RParser::TildeExpContext *ctx) = 0;
  virtual void exitTildeExp(RParser::TildeExpContext *ctx) = 0;

  virtual void enterCompoundStmt(RParser::CompoundStmtContext *ctx) = 0;
  virtual void exitCompoundStmt(RParser::CompoundStmtContext *ctx) = 0;

  virtual void enterNumRule(RParser::NumRuleContext *ctx) = 0;
  virtual void exitNumRule(RParser::NumRuleContext *ctx) = 0;

  virtual void enterKeywordRule(RParser::KeywordRuleContext *ctx) = 0;
  virtual void exitKeywordRule(RParser::KeywordRuleContext *ctx) = 0;

  virtual void enterForLoop(RParser::ForLoopContext *ctx) = 0;
  virtual void exitForLoop(RParser::ForLoopContext *ctx) = 0;

  virtual void enterIdRule(RParser::IdRuleContext *ctx) = 0;
  virtual void exitIdRule(RParser::IdRuleContext *ctx) = 0;

  virtual void enterPlusMinus(RParser::PlusMinusContext *ctx) = 0;
  virtual void exitPlusMinus(RParser::PlusMinusContext *ctx) = 0;

  virtual void enterDefineFunc(RParser::DefineFuncContext *ctx) = 0;
  virtual void exitDefineFunc(RParser::DefineFuncContext *ctx) = 0;

  virtual void enterComparison(RParser::ComparisonContext *ctx) = 0;
  virtual void exitComparison(RParser::ComparisonContext *ctx) = 0;

  virtual void enterAssignExp(RParser::AssignExpContext *ctx) = 0;
  virtual void exitAssignExp(RParser::AssignExpContext *ctx) = 0;

  virtual void enterOrExpr(RParser::OrExprContext *ctx) = 0;
  virtual void exitOrExpr(RParser::OrExprContext *ctx) = 0;

  virtual void enterCallFunc(RParser::CallFuncContext *ctx) = 0;
  virtual void exitCallFunc(RParser::CallFuncContext *ctx) = 0;

  virtual void enterIfelseStmt(RParser::IfelseStmtContext *ctx) = 0;
  virtual void exitIfelseStmt(RParser::IfelseStmtContext *ctx) = 0;

  virtual void enterNotExpr(RParser::NotExprContext *ctx) = 0;
  virtual void exitNotExpr(RParser::NotExprContext *ctx) = 0;

  virtual void enterIfStmt(RParser::IfStmtContext *ctx) = 0;
  virtual void exitIfStmt(RParser::IfStmtContext *ctx) = 0;

  virtual void enterUserExpr(RParser::UserExprContext *ctx) = 0;
  virtual void exitUserExpr(RParser::UserExprContext *ctx) = 0;

  virtual void enterColon(RParser::ColonContext *ctx) = 0;
  virtual void exitColon(RParser::ColonContext *ctx) = 0;

  virtual void enterComment(RParser::CommentContext *ctx) = 0;
  virtual void exitComment(RParser::CommentContext *ctx) = 0;

  virtual void enterDollarOrAt(RParser::DollarOrAtContext *ctx) = 0;
  virtual void exitDollarOrAt(RParser::DollarOrAtContext *ctx) = 0;

  virtual void enterPlusNeg(RParser::PlusNegContext *ctx) = 0;
  virtual void exitPlusNeg(RParser::PlusNegContext *ctx) = 0;

  virtual void enterAndExpr(RParser::AndExprContext *ctx) = 0;
  virtual void exitAndExpr(RParser::AndExprContext *ctx) = 0;

  virtual void enterExprlist(RParser::ExprlistContext *ctx) = 0;
  virtual void exitExprlist(RParser::ExprlistContext *ctx) = 0;

  virtual void enterFormlist(RParser::FormlistContext *ctx) = 0;
  virtual void exitFormlist(RParser::FormlistContext *ctx) = 0;

  virtual void enterForm(RParser::FormContext *ctx) = 0;
  virtual void exitForm(RParser::FormContext *ctx) = 0;

  virtual void enterSublist(RParser::SublistContext *ctx) = 0;
  virtual void exitSublist(RParser::SublistContext *ctx) = 0;

  virtual void enterSub(RParser::SubContext *ctx) = 0;
  virtual void exitSub(RParser::SubContext *ctx) = 0;

  virtual void enterFunctionCall(RParser::FunctionCallContext *ctx) = 0;
  virtual void exitFunctionCall(RParser::FunctionCallContext *ctx) = 0;

  virtual void enterRcomparison(RParser::RcomparisonContext *ctx) = 0;
  virtual void exitRcomparison(RParser::RcomparisonContext *ctx) = 0;

  virtual void enterAssignOp(RParser::AssignOpContext *ctx) = 0;
  virtual void exitAssignOp(RParser::AssignOpContext *ctx) = 0;

  virtual void enterNumber(RParser::NumberContext *ctx) = 0;
  virtual void exitNumber(RParser::NumberContext *ctx) = 0;

  virtual void enterRcomment(RParser::RcommentContext *ctx) = 0;
  virtual void exitRcomment(RParser::RcommentContext *ctx) = 0;

  virtual void enterIfKeyWord(RParser::IfKeyWordContext *ctx) = 0;
  virtual void exitIfKeyWord(RParser::IfKeyWordContext *ctx) = 0;

  virtual void enterElseKeyword(RParser::ElseKeywordContext *ctx) = 0;
  virtual void exitElseKeyword(RParser::ElseKeywordContext *ctx) = 0;

  virtual void enterRepeatKeyword(RParser::RepeatKeywordContext *ctx) = 0;
  virtual void exitRepeatKeyword(RParser::RepeatKeywordContext *ctx) = 0;

  virtual void enterWhileKeyword(RParser::WhileKeywordContext *ctx) = 0;
  virtual void exitWhileKeyword(RParser::WhileKeywordContext *ctx) = 0;

  virtual void enterForKeyword(RParser::ForKeywordContext *ctx) = 0;
  virtual void exitForKeyword(RParser::ForKeywordContext *ctx) = 0;

  virtual void enterBreakKeyword(RParser::BreakKeywordContext *ctx) = 0;
  virtual void exitBreakKeyword(RParser::BreakKeywordContext *ctx) = 0;

  virtual void enterInKeyword(RParser::InKeywordContext *ctx) = 0;
  virtual void exitInKeyword(RParser::InKeywordContext *ctx) = 0;

  virtual void enterHexKeyword(RParser::HexKeywordContext *ctx) = 0;
  virtual void exitHexKeyword(RParser::HexKeywordContext *ctx) = 0;

  virtual void enterFunctionKeyword(RParser::FunctionKeywordContext *ctx) = 0;
  virtual void exitFunctionKeyword(RParser::FunctionKeywordContext *ctx) = 0;

  virtual void enterTrueKeyword(RParser::TrueKeywordContext *ctx) = 0;
  virtual void exitTrueKeyword(RParser::TrueKeywordContext *ctx) = 0;

  virtual void enterFalseKeyword(RParser::FalseKeywordContext *ctx) = 0;
  virtual void exitFalseKeyword(RParser::FalseKeywordContext *ctx) = 0;

  virtual void enterNaKeyword(RParser::NaKeywordContext *ctx) = 0;
  virtual void exitNaKeyword(RParser::NaKeywordContext *ctx) = 0;

  virtual void enterNanKeyword(RParser::NanKeywordContext *ctx) = 0;
  virtual void exitNanKeyword(RParser::NanKeywordContext *ctx) = 0;

  virtual void enterInfKeyword(RParser::InfKeywordContext *ctx) = 0;
  virtual void exitInfKeyword(RParser::InfKeywordContext *ctx) = 0;

  virtual void enterNullKeyword(RParser::NullKeywordContext *ctx) = 0;
  virtual void exitNullKeyword(RParser::NullKeywordContext *ctx) = 0;


};

