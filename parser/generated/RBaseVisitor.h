
// Generated from Rc2Parser/R.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RVisitor.h"


/**
 * This class provides an empty implementation of RVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  RBaseVisitor : public RVisitor {
public:

  virtual antlrcpp::Any visitProg(RParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignment(RParser::AssignmentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSingleExp(RParser::SingleExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileLoop(RParser::WhileLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHelpRequest(RParser::HelpRequestContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRepeatLoop(RParser::RepeatLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExpTildeExp(RParser::ExpTildeExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitListExp(RParser::ListExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDescendant(RParser::DescendantContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHouse(RParser::HouseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitListRef(RParser::ListRefContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMulDiv(RParser::MulDivContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHexRule(RParser::HexRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitArray(RParser::ArrayContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStringRule(RParser::StringRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTildeExp(RParser::TildeExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCompoundStmt(RParser::CompoundStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumRule(RParser::NumRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitKeywordRule(RParser::KeywordRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForLoop(RParser::ForLoopContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIdRule(RParser::IdRuleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPlusMinus(RParser::PlusMinusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDefineFunc(RParser::DefineFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComparison(RParser::ComparisonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignExp(RParser::AssignExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOrExpr(RParser::OrExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCallFunc(RParser::CallFuncContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfelseStmt(RParser::IfelseStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNotExpr(RParser::NotExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfStmt(RParser::IfStmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUserExpr(RParser::UserExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitColon(RParser::ColonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComment(RParser::CommentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDollarOrAt(RParser::DollarOrAtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPlusNeg(RParser::PlusNegContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAndExpr(RParser::AndExprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExprlist(RParser::ExprlistContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFormlist(RParser::FormlistContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForm(RParser::FormContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSublist(RParser::SublistContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSub(RParser::SubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionCall(RParser::FunctionCallContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRcomparison(RParser::RcomparisonContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAssignOp(RParser::AssignOpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNumber(RParser::NumberContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRcomment(RParser::RcommentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitIfKeyWord(RParser::IfKeyWordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElseKeyword(RParser::ElseKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRepeatKeyword(RParser::RepeatKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitWhileKeyword(RParser::WhileKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitForKeyword(RParser::ForKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBreakKeyword(RParser::BreakKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInKeyword(RParser::InKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitHexKeyword(RParser::HexKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunctionKeyword(RParser::FunctionKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTrueKeyword(RParser::TrueKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFalseKeyword(RParser::FalseKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNaKeyword(RParser::NaKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNanKeyword(RParser::NanKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInfKeyword(RParser::InfKeywordContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitNullKeyword(RParser::NullKeywordContext *ctx) override {
    return visitChildren(ctx);
  }


};

