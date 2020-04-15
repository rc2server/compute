
// Generated from Rc2Parser/R.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by RParser.
 */
class  RVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by RParser.
   */
    virtual antlrcpp::Any visitProg(RParser::ProgContext *context) = 0;

    virtual antlrcpp::Any visitAssignment(RParser::AssignmentContext *context) = 0;

    virtual antlrcpp::Any visitSingleExp(RParser::SingleExpContext *context) = 0;

    virtual antlrcpp::Any visitWhileLoop(RParser::WhileLoopContext *context) = 0;

    virtual antlrcpp::Any visitHelpRequest(RParser::HelpRequestContext *context) = 0;

    virtual antlrcpp::Any visitRepeatLoop(RParser::RepeatLoopContext *context) = 0;

    virtual antlrcpp::Any visitExpTildeExp(RParser::ExpTildeExpContext *context) = 0;

    virtual antlrcpp::Any visitListExp(RParser::ListExpContext *context) = 0;

    virtual antlrcpp::Any visitDescendant(RParser::DescendantContext *context) = 0;

    virtual antlrcpp::Any visitHouse(RParser::HouseContext *context) = 0;

    virtual antlrcpp::Any visitListRef(RParser::ListRefContext *context) = 0;

    virtual antlrcpp::Any visitMulDiv(RParser::MulDivContext *context) = 0;

    virtual antlrcpp::Any visitHexRule(RParser::HexRuleContext *context) = 0;

    virtual antlrcpp::Any visitArray(RParser::ArrayContext *context) = 0;

    virtual antlrcpp::Any visitStringRule(RParser::StringRuleContext *context) = 0;

    virtual antlrcpp::Any visitTildeExp(RParser::TildeExpContext *context) = 0;

    virtual antlrcpp::Any visitCompoundStmt(RParser::CompoundStmtContext *context) = 0;

    virtual antlrcpp::Any visitNumRule(RParser::NumRuleContext *context) = 0;

    virtual antlrcpp::Any visitKeywordRule(RParser::KeywordRuleContext *context) = 0;

    virtual antlrcpp::Any visitForLoop(RParser::ForLoopContext *context) = 0;

    virtual antlrcpp::Any visitIdRule(RParser::IdRuleContext *context) = 0;

    virtual antlrcpp::Any visitPlusMinus(RParser::PlusMinusContext *context) = 0;

    virtual antlrcpp::Any visitDefineFunc(RParser::DefineFuncContext *context) = 0;

    virtual antlrcpp::Any visitComparison(RParser::ComparisonContext *context) = 0;

    virtual antlrcpp::Any visitAssignExp(RParser::AssignExpContext *context) = 0;

    virtual antlrcpp::Any visitOrExpr(RParser::OrExprContext *context) = 0;

    virtual antlrcpp::Any visitCallFunc(RParser::CallFuncContext *context) = 0;

    virtual antlrcpp::Any visitIfelseStmt(RParser::IfelseStmtContext *context) = 0;

    virtual antlrcpp::Any visitNotExpr(RParser::NotExprContext *context) = 0;

    virtual antlrcpp::Any visitIfStmt(RParser::IfStmtContext *context) = 0;

    virtual antlrcpp::Any visitUserExpr(RParser::UserExprContext *context) = 0;

    virtual antlrcpp::Any visitColon(RParser::ColonContext *context) = 0;

    virtual antlrcpp::Any visitComment(RParser::CommentContext *context) = 0;

    virtual antlrcpp::Any visitDollarOrAt(RParser::DollarOrAtContext *context) = 0;

    virtual antlrcpp::Any visitPlusNeg(RParser::PlusNegContext *context) = 0;

    virtual antlrcpp::Any visitAndExpr(RParser::AndExprContext *context) = 0;

    virtual antlrcpp::Any visitExprlist(RParser::ExprlistContext *context) = 0;

    virtual antlrcpp::Any visitFormlist(RParser::FormlistContext *context) = 0;

    virtual antlrcpp::Any visitForm(RParser::FormContext *context) = 0;

    virtual antlrcpp::Any visitSublist(RParser::SublistContext *context) = 0;

    virtual antlrcpp::Any visitSub(RParser::SubContext *context) = 0;

    virtual antlrcpp::Any visitFunctionCall(RParser::FunctionCallContext *context) = 0;

    virtual antlrcpp::Any visitRcomparison(RParser::RcomparisonContext *context) = 0;

    virtual antlrcpp::Any visitAssignOp(RParser::AssignOpContext *context) = 0;

    virtual antlrcpp::Any visitNumber(RParser::NumberContext *context) = 0;

    virtual antlrcpp::Any visitRcomment(RParser::RcommentContext *context) = 0;

    virtual antlrcpp::Any visitIfKeyWord(RParser::IfKeyWordContext *context) = 0;

    virtual antlrcpp::Any visitElseKeyword(RParser::ElseKeywordContext *context) = 0;

    virtual antlrcpp::Any visitRepeatKeyword(RParser::RepeatKeywordContext *context) = 0;

    virtual antlrcpp::Any visitWhileKeyword(RParser::WhileKeywordContext *context) = 0;

    virtual antlrcpp::Any visitForKeyword(RParser::ForKeywordContext *context) = 0;

    virtual antlrcpp::Any visitBreakKeyword(RParser::BreakKeywordContext *context) = 0;

    virtual antlrcpp::Any visitInKeyword(RParser::InKeywordContext *context) = 0;

    virtual antlrcpp::Any visitHexKeyword(RParser::HexKeywordContext *context) = 0;

    virtual antlrcpp::Any visitFunctionKeyword(RParser::FunctionKeywordContext *context) = 0;

    virtual antlrcpp::Any visitTrueKeyword(RParser::TrueKeywordContext *context) = 0;

    virtual antlrcpp::Any visitFalseKeyword(RParser::FalseKeywordContext *context) = 0;

    virtual antlrcpp::Any visitNaKeyword(RParser::NaKeywordContext *context) = 0;

    virtual antlrcpp::Any visitNanKeyword(RParser::NanKeywordContext *context) = 0;

    virtual antlrcpp::Any visitInfKeyword(RParser::InfKeywordContext *context) = 0;

    virtual antlrcpp::Any visitNullKeyword(RParser::NullKeywordContext *context) = 0;


};

