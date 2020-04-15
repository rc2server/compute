
// Generated from Rc2Parser/R.g4 by ANTLR 4.8


#include "RListener.h"
#include "RVisitor.h"

#include "RParser.h"


using namespace antlrcpp;
using namespace antlr4;

RParser::RParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

RParser::~RParser() {
  delete _interpreter;
}

std::string RParser::getGrammarFileName() const {
  return "R.g4";
}

const std::vector<std::string>& RParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& RParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- ProgContext ------------------------------------------------------------------

RParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::ProgContext::EOF() {
  return getToken(RParser::EOF, 0);
}

std::vector<RParser::Expr_or_assignContext *> RParser::ProgContext::expr_or_assign() {
  return getRuleContexts<RParser::Expr_or_assignContext>();
}

RParser::Expr_or_assignContext* RParser::ProgContext::expr_or_assign(size_t i) {
  return getRuleContext<RParser::Expr_or_assignContext>(i);
}

std::vector<tree::TerminalNode *> RParser::ProgContext::NL() {
  return getTokens(RParser::NL);
}

tree::TerminalNode* RParser::ProgContext::NL(size_t i) {
  return getToken(RParser::NL, i);
}

std::vector<tree::TerminalNode *> RParser::ProgContext::SEMI() {
  return getTokens(RParser::SEMI);
}

tree::TerminalNode* RParser::ProgContext::SEMI(size_t i) {
  return getToken(RParser::SEMI, i);
}


size_t RParser::ProgContext::getRuleIndex() const {
  return RParser::RuleProg;
}

void RParser::ProgContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProg(this);
}

void RParser::ProgContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProg(this);
}


antlrcpp::Any RParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

RParser::ProgContext* RParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 0, RParser::RuleProg);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(38);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RParser::HEX)
      | (1ULL << RParser::INT)
      | (1ULL << RParser::FLOAT)
      | (1ULL << RParser::COMPLEX)
      | (1ULL << RParser::STRING)
      | (1ULL << RParser::ID)
      | (1ULL << RParser::IF)
      | (1ULL << RParser::ELSE)
      | (1ULL << RParser::REPEAT)
      | (1ULL << RParser::WHILE)
      | (1ULL << RParser::FOR)
      | (1ULL << RParser::IN)
      | (1ULL << RParser::BREAK)
      | (1ULL << RParser::NEXT)
      | (1ULL << RParser::FUNCTION)
      | (1ULL << RParser::PAR_OPEN)
      | (1ULL << RParser::BRC_OPEN)
      | (1ULL << RParser::QMARK)
      | (1ULL << RParser::PLUS)
      | (1ULL << RParser::MINUS)
      | (1ULL << RParser::NOT)
      | (1ULL << RParser::TILDE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (RParser::NULLT - 64))
      | (1ULL << (RParser::NaN - 64))
      | (1ULL << (RParser::TRUE - 64))
      | (1ULL << (RParser::FALSE - 64))
      | (1ULL << (RParser::NA - 64))
      | (1ULL << (RParser::INF - 64))
      | (1ULL << (RParser::COMMENT - 64))
      | (1ULL << (RParser::NL - 64)))) != 0)) {
      setState(36);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case RParser::HEX:
        case RParser::INT:
        case RParser::FLOAT:
        case RParser::COMPLEX:
        case RParser::STRING:
        case RParser::ID:
        case RParser::IF:
        case RParser::ELSE:
        case RParser::REPEAT:
        case RParser::WHILE:
        case RParser::FOR:
        case RParser::IN:
        case RParser::BREAK:
        case RParser::NEXT:
        case RParser::FUNCTION:
        case RParser::PAR_OPEN:
        case RParser::BRC_OPEN:
        case RParser::QMARK:
        case RParser::PLUS:
        case RParser::MINUS:
        case RParser::NOT:
        case RParser::TILDE:
        case RParser::NULLT:
        case RParser::NaN:
        case RParser::TRUE:
        case RParser::FALSE:
        case RParser::NA:
        case RParser::INF:
        case RParser::COMMENT: {
          setState(28);
          expr_or_assign();
          setState(32);
          _errHandler->sync(this);
          alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
          while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
            if (alt == 1) {
              setState(29);
              _la = _input->LA(1);
              if (!(_la == RParser::SEMI

              || _la == RParser::NL)) {
              _errHandler->recoverInline(this);
              }
              else {
                _errHandler->reportMatch(this);
                consume();
              } 
            }
            setState(34);
            _errHandler->sync(this);
            alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx);
          }
          break;
        }

        case RParser::NL: {
          setState(35);
          match(RParser::NL);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(40);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(41);
    match(RParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Expr_or_assignContext ------------------------------------------------------------------

RParser::Expr_or_assignContext::Expr_or_assignContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RParser::Expr_or_assignContext::getRuleIndex() const {
  return RParser::RuleExpr_or_assign;
}

void RParser::Expr_or_assignContext::copyFrom(Expr_or_assignContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- AssignmentContext ------------------------------------------------------------------

RParser::ExprContext* RParser::AssignmentContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::AssignOpContext* RParser::AssignmentContext::assignOp() {
  return getRuleContext<RParser::AssignOpContext>(0);
}

RParser::Expr_or_assignContext* RParser::AssignmentContext::expr_or_assign() {
  return getRuleContext<RParser::Expr_or_assignContext>(0);
}

RParser::AssignmentContext::AssignmentContext(Expr_or_assignContext *ctx) { copyFrom(ctx); }

void RParser::AssignmentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignment(this);
}
void RParser::AssignmentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignment(this);
}

antlrcpp::Any RParser::AssignmentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitAssignment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- SingleExpContext ------------------------------------------------------------------

RParser::ExprContext* RParser::SingleExpContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::SingleExpContext::SingleExpContext(Expr_or_assignContext *ctx) { copyFrom(ctx); }

void RParser::SingleExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSingleExp(this);
}
void RParser::SingleExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSingleExp(this);
}

antlrcpp::Any RParser::SingleExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitSingleExp(this);
  else
    return visitor->visitChildren(this);
}
RParser::Expr_or_assignContext* RParser::expr_or_assign() {
  Expr_or_assignContext *_localctx = _tracker.createInstance<Expr_or_assignContext>(_ctx, getState());
  enterRule(_localctx, 2, RParser::RuleExpr_or_assign);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(48);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<Expr_or_assignContext *>(_tracker.createInstance<RParser::AssignmentContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(43);
      expr(0);
      setState(44);
      assignOp();
      setState(45);
      expr_or_assign();
      break;
    }

    case 2: {
      _localctx = dynamic_cast<Expr_or_assignContext *>(_tracker.createInstance<RParser::SingleExpContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(47);
      expr(0);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

RParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RParser::ExprContext::getRuleIndex() const {
  return RParser::RuleExpr;
}

void RParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- WhileLoopContext ------------------------------------------------------------------

tree::TerminalNode* RParser::WhileLoopContext::WHILE() {
  return getToken(RParser::WHILE, 0);
}

tree::TerminalNode* RParser::WhileLoopContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

std::vector<RParser::ExprContext *> RParser::WhileLoopContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::WhileLoopContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::WhileLoopContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::WhileLoopContext::WhileLoopContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::WhileLoopContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileLoop(this);
}
void RParser::WhileLoopContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileLoop(this);
}

antlrcpp::Any RParser::WhileLoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitWhileLoop(this);
  else
    return visitor->visitChildren(this);
}
//----------------- HelpRequestContext ------------------------------------------------------------------

tree::TerminalNode* RParser::HelpRequestContext::QMARK() {
  return getToken(RParser::QMARK, 0);
}

RParser::ExprContext* RParser::HelpRequestContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::HelpRequestContext::HelpRequestContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::HelpRequestContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHelpRequest(this);
}
void RParser::HelpRequestContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHelpRequest(this);
}

antlrcpp::Any RParser::HelpRequestContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitHelpRequest(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RepeatLoopContext ------------------------------------------------------------------

tree::TerminalNode* RParser::RepeatLoopContext::REPEAT() {
  return getToken(RParser::REPEAT, 0);
}

RParser::ExprContext* RParser::RepeatLoopContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::RepeatLoopContext::RepeatLoopContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::RepeatLoopContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRepeatLoop(this);
}
void RParser::RepeatLoopContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRepeatLoop(this);
}

antlrcpp::Any RParser::RepeatLoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitRepeatLoop(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ExpTildeExpContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::ExpTildeExpContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::ExpTildeExpContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::ExpTildeExpContext::TILDE() {
  return getToken(RParser::TILDE, 0);
}

RParser::ExpTildeExpContext::ExpTildeExpContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ExpTildeExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExpTildeExp(this);
}
void RParser::ExpTildeExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExpTildeExp(this);
}

antlrcpp::Any RParser::ExpTildeExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitExpTildeExp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ListExpContext ------------------------------------------------------------------

tree::TerminalNode* RParser::ListExpContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

RParser::ExprContext* RParser::ListExpContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::ListExpContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::ListExpContext::ListExpContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ListExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterListExp(this);
}
void RParser::ListExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitListExp(this);
}

antlrcpp::Any RParser::ListExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitListExp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DescendantContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::DescendantContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::DescendantContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::DescendantContext::DOUBLE_COLON() {
  return getToken(RParser::DOUBLE_COLON, 0);
}

tree::TerminalNode* RParser::DescendantContext::TRIPLE_COLON() {
  return getToken(RParser::TRIPLE_COLON, 0);
}

RParser::DescendantContext::DescendantContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::DescendantContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDescendant(this);
}
void RParser::DescendantContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDescendant(this);
}

antlrcpp::Any RParser::DescendantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitDescendant(this);
  else
    return visitor->visitChildren(this);
}
//----------------- HouseContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::HouseContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::HouseContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::HouseContext::CARAT() {
  return getToken(RParser::CARAT, 0);
}

RParser::HouseContext::HouseContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::HouseContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHouse(this);
}
void RParser::HouseContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHouse(this);
}

antlrcpp::Any RParser::HouseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitHouse(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ListRefContext ------------------------------------------------------------------

RParser::ExprContext* RParser::ListRefContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::ListRefContext::DBRA_OPEN() {
  return getToken(RParser::DBRA_OPEN, 0);
}

RParser::SublistContext* RParser::ListRefContext::sublist() {
  return getRuleContext<RParser::SublistContext>(0);
}

std::vector<tree::TerminalNode *> RParser::ListRefContext::BRA_CLOSE() {
  return getTokens(RParser::BRA_CLOSE);
}

tree::TerminalNode* RParser::ListRefContext::BRA_CLOSE(size_t i) {
  return getToken(RParser::BRA_CLOSE, i);
}

RParser::ListRefContext::ListRefContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ListRefContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterListRef(this);
}
void RParser::ListRefContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitListRef(this);
}

antlrcpp::Any RParser::ListRefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitListRef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MulDivContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::MulDivContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::MulDivContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::MulDivContext::ASTERICK() {
  return getToken(RParser::ASTERICK, 0);
}

tree::TerminalNode* RParser::MulDivContext::SLASH() {
  return getToken(RParser::SLASH, 0);
}

RParser::MulDivContext::MulDivContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::MulDivContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMulDiv(this);
}
void RParser::MulDivContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMulDiv(this);
}

antlrcpp::Any RParser::MulDivContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitMulDiv(this);
  else
    return visitor->visitChildren(this);
}
//----------------- HexRuleContext ------------------------------------------------------------------

tree::TerminalNode* RParser::HexRuleContext::HEX() {
  return getToken(RParser::HEX, 0);
}

RParser::HexRuleContext::HexRuleContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::HexRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHexRule(this);
}
void RParser::HexRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHexRule(this);
}

antlrcpp::Any RParser::HexRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitHexRule(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ArrayContext ------------------------------------------------------------------

RParser::ExprContext* RParser::ArrayContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::ArrayContext::BRA_OPEN() {
  return getToken(RParser::BRA_OPEN, 0);
}

RParser::SublistContext* RParser::ArrayContext::sublist() {
  return getRuleContext<RParser::SublistContext>(0);
}

tree::TerminalNode* RParser::ArrayContext::BRA_CLOSE() {
  return getToken(RParser::BRA_CLOSE, 0);
}

RParser::ArrayContext::ArrayContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ArrayContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterArray(this);
}
void RParser::ArrayContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitArray(this);
}

antlrcpp::Any RParser::ArrayContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitArray(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StringRuleContext ------------------------------------------------------------------

tree::TerminalNode* RParser::StringRuleContext::STRING() {
  return getToken(RParser::STRING, 0);
}

RParser::StringRuleContext::StringRuleContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::StringRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStringRule(this);
}
void RParser::StringRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStringRule(this);
}

antlrcpp::Any RParser::StringRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitStringRule(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TildeExpContext ------------------------------------------------------------------

tree::TerminalNode* RParser::TildeExpContext::TILDE() {
  return getToken(RParser::TILDE, 0);
}

RParser::ExprContext* RParser::TildeExpContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::TildeExpContext::TildeExpContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::TildeExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTildeExp(this);
}
void RParser::TildeExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTildeExp(this);
}

antlrcpp::Any RParser::TildeExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitTildeExp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CompoundStmtContext ------------------------------------------------------------------

tree::TerminalNode* RParser::CompoundStmtContext::BRC_OPEN() {
  return getToken(RParser::BRC_OPEN, 0);
}

RParser::ExprlistContext* RParser::CompoundStmtContext::exprlist() {
  return getRuleContext<RParser::ExprlistContext>(0);
}

tree::TerminalNode* RParser::CompoundStmtContext::BRC_CLOSE() {
  return getToken(RParser::BRC_CLOSE, 0);
}

RParser::CompoundStmtContext::CompoundStmtContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::CompoundStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCompoundStmt(this);
}
void RParser::CompoundStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCompoundStmt(this);
}

antlrcpp::Any RParser::CompoundStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitCompoundStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NumRuleContext ------------------------------------------------------------------

RParser::NumberContext* RParser::NumRuleContext::number() {
  return getRuleContext<RParser::NumberContext>(0);
}

RParser::NumRuleContext::NumRuleContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::NumRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumRule(this);
}
void RParser::NumRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumRule(this);
}

antlrcpp::Any RParser::NumRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNumRule(this);
  else
    return visitor->visitChildren(this);
}
//----------------- KeywordRuleContext ------------------------------------------------------------------

RParser::KeywordContext* RParser::KeywordRuleContext::keyword() {
  return getRuleContext<RParser::KeywordContext>(0);
}

RParser::KeywordRuleContext::KeywordRuleContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::KeywordRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterKeywordRule(this);
}
void RParser::KeywordRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitKeywordRule(this);
}

antlrcpp::Any RParser::KeywordRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitKeywordRule(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ForLoopContext ------------------------------------------------------------------

tree::TerminalNode* RParser::ForLoopContext::FOR() {
  return getToken(RParser::FOR, 0);
}

tree::TerminalNode* RParser::ForLoopContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

tree::TerminalNode* RParser::ForLoopContext::ID() {
  return getToken(RParser::ID, 0);
}

tree::TerminalNode* RParser::ForLoopContext::IN() {
  return getToken(RParser::IN, 0);
}

std::vector<RParser::ExprContext *> RParser::ForLoopContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::ForLoopContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::ForLoopContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::ForLoopContext::ForLoopContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ForLoopContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForLoop(this);
}
void RParser::ForLoopContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForLoop(this);
}

antlrcpp::Any RParser::ForLoopContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitForLoop(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IdRuleContext ------------------------------------------------------------------

tree::TerminalNode* RParser::IdRuleContext::ID() {
  return getToken(RParser::ID, 0);
}

RParser::IdRuleContext::IdRuleContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::IdRuleContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIdRule(this);
}
void RParser::IdRuleContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIdRule(this);
}

antlrcpp::Any RParser::IdRuleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitIdRule(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusMinusContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::PlusMinusContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::PlusMinusContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::PlusMinusContext::PLUS() {
  return getToken(RParser::PLUS, 0);
}

tree::TerminalNode* RParser::PlusMinusContext::MINUS() {
  return getToken(RParser::MINUS, 0);
}

RParser::PlusMinusContext::PlusMinusContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::PlusMinusContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlusMinus(this);
}
void RParser::PlusMinusContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlusMinus(this);
}

antlrcpp::Any RParser::PlusMinusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitPlusMinus(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DefineFuncContext ------------------------------------------------------------------

tree::TerminalNode* RParser::DefineFuncContext::FUNCTION() {
  return getToken(RParser::FUNCTION, 0);
}

tree::TerminalNode* RParser::DefineFuncContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

tree::TerminalNode* RParser::DefineFuncContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::ExprContext* RParser::DefineFuncContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::FormlistContext* RParser::DefineFuncContext::formlist() {
  return getRuleContext<RParser::FormlistContext>(0);
}

RParser::DefineFuncContext::DefineFuncContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::DefineFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDefineFunc(this);
}
void RParser::DefineFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDefineFunc(this);
}

antlrcpp::Any RParser::DefineFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitDefineFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ComparisonContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::ComparisonContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::ComparisonContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

RParser::RcomparisonContext* RParser::ComparisonContext::rcomparison() {
  return getRuleContext<RParser::RcomparisonContext>(0);
}

RParser::ComparisonContext::ComparisonContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ComparisonContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComparison(this);
}
void RParser::ComparisonContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComparison(this);
}

antlrcpp::Any RParser::ComparisonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitComparison(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AssignExpContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::AssignExpContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::AssignExpContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

RParser::AssignOpContext* RParser::AssignExpContext::assignOp() {
  return getRuleContext<RParser::AssignOpContext>(0);
}

RParser::AssignExpContext::AssignExpContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::AssignExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignExp(this);
}
void RParser::AssignExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignExp(this);
}

antlrcpp::Any RParser::AssignExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitAssignExp(this);
  else
    return visitor->visitChildren(this);
}
//----------------- OrExprContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::OrExprContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::OrExprContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::OrExprContext::BAR() {
  return getToken(RParser::BAR, 0);
}

tree::TerminalNode* RParser::OrExprContext::DBAR() {
  return getToken(RParser::DBAR, 0);
}

RParser::OrExprContext::OrExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::OrExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOrExpr(this);
}
void RParser::OrExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOrExpr(this);
}

antlrcpp::Any RParser::OrExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitOrExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CallFuncContext ------------------------------------------------------------------

RParser::ExprContext* RParser::CallFuncContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::CallFuncContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

RParser::SublistContext* RParser::CallFuncContext::sublist() {
  return getRuleContext<RParser::SublistContext>(0);
}

tree::TerminalNode* RParser::CallFuncContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::CallFuncContext::CallFuncContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::CallFuncContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCallFunc(this);
}
void RParser::CallFuncContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCallFunc(this);
}

antlrcpp::Any RParser::CallFuncContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitCallFunc(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfelseStmtContext ------------------------------------------------------------------

tree::TerminalNode* RParser::IfelseStmtContext::IF() {
  return getToken(RParser::IF, 0);
}

tree::TerminalNode* RParser::IfelseStmtContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

std::vector<RParser::ExprContext *> RParser::IfelseStmtContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::IfelseStmtContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::IfelseStmtContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

tree::TerminalNode* RParser::IfelseStmtContext::ELSE() {
  return getToken(RParser::ELSE, 0);
}

RParser::IfelseStmtContext::IfelseStmtContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::IfelseStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfelseStmt(this);
}
void RParser::IfelseStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfelseStmt(this);
}

antlrcpp::Any RParser::IfelseStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitIfelseStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NotExprContext ------------------------------------------------------------------

tree::TerminalNode* RParser::NotExprContext::NOT() {
  return getToken(RParser::NOT, 0);
}

RParser::ExprContext* RParser::NotExprContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

RParser::NotExprContext::NotExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::NotExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNotExpr(this);
}
void RParser::NotExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNotExpr(this);
}

antlrcpp::Any RParser::NotExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNotExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfStmtContext ------------------------------------------------------------------

tree::TerminalNode* RParser::IfStmtContext::IF() {
  return getToken(RParser::IF, 0);
}

tree::TerminalNode* RParser::IfStmtContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

std::vector<RParser::ExprContext *> RParser::IfStmtContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::IfStmtContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::IfStmtContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}

RParser::IfStmtContext::IfStmtContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::IfStmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfStmt(this);
}
void RParser::IfStmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfStmt(this);
}

antlrcpp::Any RParser::IfStmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitIfStmt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- UserExprContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::UserExprContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::UserExprContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::UserExprContext::USER_OP() {
  return getToken(RParser::USER_OP, 0);
}

RParser::UserExprContext::UserExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::UserExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUserExpr(this);
}
void RParser::UserExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUserExpr(this);
}

antlrcpp::Any RParser::UserExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitUserExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ColonContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::ColonContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::ColonContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::ColonContext::COLON() {
  return getToken(RParser::COLON, 0);
}

RParser::ColonContext::ColonContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::ColonContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterColon(this);
}
void RParser::ColonContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitColon(this);
}

antlrcpp::Any RParser::ColonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitColon(this);
  else
    return visitor->visitChildren(this);
}
//----------------- CommentContext ------------------------------------------------------------------

RParser::RcommentContext* RParser::CommentContext::rcomment() {
  return getRuleContext<RParser::RcommentContext>(0);
}

RParser::CommentContext::CommentContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::CommentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComment(this);
}
void RParser::CommentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComment(this);
}

antlrcpp::Any RParser::CommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitComment(this);
  else
    return visitor->visitChildren(this);
}
//----------------- DollarOrAtContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::DollarOrAtContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::DollarOrAtContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::DollarOrAtContext::DOLLAR_CHAR() {
  return getToken(RParser::DOLLAR_CHAR, 0);
}

tree::TerminalNode* RParser::DollarOrAtContext::AT_CHAR() {
  return getToken(RParser::AT_CHAR, 0);
}

RParser::DollarOrAtContext::DollarOrAtContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::DollarOrAtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDollarOrAt(this);
}
void RParser::DollarOrAtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDollarOrAt(this);
}

antlrcpp::Any RParser::DollarOrAtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitDollarOrAt(this);
  else
    return visitor->visitChildren(this);
}
//----------------- PlusNegContext ------------------------------------------------------------------

RParser::ExprContext* RParser::PlusNegContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::PlusNegContext::MINUS() {
  return getToken(RParser::MINUS, 0);
}

tree::TerminalNode* RParser::PlusNegContext::PLUS() {
  return getToken(RParser::PLUS, 0);
}

RParser::PlusNegContext::PlusNegContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::PlusNegContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPlusNeg(this);
}
void RParser::PlusNegContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPlusNeg(this);
}

antlrcpp::Any RParser::PlusNegContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitPlusNeg(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AndExprContext ------------------------------------------------------------------

std::vector<RParser::ExprContext *> RParser::AndExprContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::AndExprContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

tree::TerminalNode* RParser::AndExprContext::AND_OP() {
  return getToken(RParser::AND_OP, 0);
}

RParser::AndExprContext::AndExprContext(ExprContext *ctx) { copyFrom(ctx); }

void RParser::AndExprContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAndExpr(this);
}
void RParser::AndExprContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAndExpr(this);
}

antlrcpp::Any RParser::AndExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitAndExpr(this);
  else
    return visitor->visitChildren(this);
}

RParser::ExprContext* RParser::expr() {
   return expr(0);
}

RParser::ExprContext* RParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  RParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  RParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 4;
  enterRecursionRule(_localctx, 4, RParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(110);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      _localctx = _tracker.createInstance<PlusNegContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;

      setState(51);
      _la = _input->LA(1);
      if (!(_la == RParser::PLUS

      || _la == RParser::MINUS)) {
      _errHandler->recoverInline(this);
      }
      else {
        _errHandler->reportMatch(this);
        consume();
      }
      setState(52);
      expr(28);
      break;
    }

    case 2: {
      _localctx = _tracker.createInstance<CommentContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(53);
      rcomment();
      break;
    }

    case 3: {
      _localctx = _tracker.createInstance<NotExprContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(54);
      match(RParser::NOT);
      setState(55);
      expr(21);
      break;
    }

    case 4: {
      _localctx = _tracker.createInstance<TildeExpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(56);
      match(RParser::TILDE);
      setState(57);
      expr(18);
      break;
    }

    case 5: {
      _localctx = _tracker.createInstance<DefineFuncContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(58);
      match(RParser::FUNCTION);
      setState(59);
      match(RParser::PAR_OPEN);
      setState(61);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << RParser::ID)
        | (1ULL << RParser::PERIOD)
        | (1ULL << RParser::TRIPLE_DOT))) != 0)) {
        setState(60);
        formlist();
      }
      setState(63);
      match(RParser::PAR_CLOSE);
      setState(64);
      expr(15);
      break;
    }

    case 6: {
      _localctx = _tracker.createInstance<CompoundStmtContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(65);
      match(RParser::BRC_OPEN);
      setState(66);
      exprlist();
      setState(67);
      match(RParser::BRC_CLOSE);
      break;
    }

    case 7: {
      _localctx = _tracker.createInstance<IfStmtContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(69);
      match(RParser::IF);
      setState(70);
      match(RParser::PAR_OPEN);
      setState(71);
      expr(0);
      setState(72);
      match(RParser::PAR_CLOSE);
      setState(73);
      expr(12);
      break;
    }

    case 8: {
      _localctx = _tracker.createInstance<IfelseStmtContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(75);
      match(RParser::IF);
      setState(76);
      match(RParser::PAR_OPEN);
      setState(77);
      expr(0);
      setState(78);
      match(RParser::PAR_CLOSE);
      setState(79);
      expr(0);
      setState(80);
      match(RParser::ELSE);
      setState(81);
      expr(11);
      break;
    }

    case 9: {
      _localctx = _tracker.createInstance<ForLoopContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(83);
      match(RParser::FOR);
      setState(84);
      match(RParser::PAR_OPEN);
      setState(85);
      match(RParser::ID);
      setState(86);
      match(RParser::IN);
      setState(87);
      expr(0);
      setState(88);
      match(RParser::PAR_CLOSE);
      setState(89);
      expr(10);
      break;
    }

    case 10: {
      _localctx = _tracker.createInstance<WhileLoopContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(91);
      match(RParser::WHILE);
      setState(92);
      match(RParser::PAR_OPEN);
      setState(93);
      expr(0);
      setState(94);
      match(RParser::PAR_CLOSE);
      setState(95);
      expr(9);
      break;
    }

    case 11: {
      _localctx = _tracker.createInstance<RepeatLoopContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(97);
      match(RParser::REPEAT);
      setState(98);
      expr(8);
      break;
    }

    case 12: {
      _localctx = _tracker.createInstance<HelpRequestContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(99);
      match(RParser::QMARK);
      setState(100);
      expr(7);
      break;
    }

    case 13: {
      _localctx = _tracker.createInstance<KeywordRuleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(101);
      keyword();
      break;
    }

    case 14: {
      _localctx = _tracker.createInstance<ListExpContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(102);
      match(RParser::PAR_OPEN);
      setState(103);
      expr(0);
      setState(104);
      match(RParser::PAR_CLOSE);
      break;
    }

    case 15: {
      _localctx = _tracker.createInstance<IdRuleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(106);
      match(RParser::ID);
      break;
    }

    case 16: {
      _localctx = _tracker.createInstance<StringRuleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(107);
      match(RParser::STRING);
      break;
    }

    case 17: {
      _localctx = _tracker.createInstance<HexRuleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(108);
      match(RParser::HEX);
      break;
    }

    case 18: {
      _localctx = _tracker.createInstance<NumRuleContext>(_localctx);
      _ctx = _localctx;
      previousContext = _localctx;
      setState(109);
      number();
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(168);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(166);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<DescendantContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(112);

          if (!(precpred(_ctx, 31))) throw FailedPredicateException(this, "precpred(_ctx, 31)");
          setState(113);
          _la = _input->LA(1);
          if (!(_la == RParser::TRIPLE_COLON

          || _la == RParser::DOUBLE_COLON)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(114);
          expr(32);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<DollarOrAtContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(115);

          if (!(precpred(_ctx, 30))) throw FailedPredicateException(this, "precpred(_ctx, 30)");
          setState(116);
          _la = _input->LA(1);
          if (!(_la == RParser::DOLLAR_CHAR

          || _la == RParser::AT_CHAR)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(117);
          expr(31);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<HouseContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(118);

          if (!(precpred(_ctx, 29))) throw FailedPredicateException(this, "precpred(_ctx, 29)");
          setState(119);
          match(RParser::CARAT);
          setState(120);
          expr(29);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<ColonContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(121);

          if (!(precpred(_ctx, 27))) throw FailedPredicateException(this, "precpred(_ctx, 27)");
          setState(122);
          match(RParser::COLON);
          setState(123);
          expr(28);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<UserExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(124);

          if (!(precpred(_ctx, 26))) throw FailedPredicateException(this, "precpred(_ctx, 26)");
          setState(125);
          match(RParser::USER_OP);
          setState(126);
          expr(27);
          break;
        }

        case 6: {
          auto newContext = _tracker.createInstance<MulDivContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(127);

          if (!(precpred(_ctx, 25))) throw FailedPredicateException(this, "precpred(_ctx, 25)");
          setState(128);
          _la = _input->LA(1);
          if (!(_la == RParser::ASTERICK

          || _la == RParser::SLASH)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(129);
          expr(26);
          break;
        }

        case 7: {
          auto newContext = _tracker.createInstance<PlusMinusContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(130);

          if (!(precpred(_ctx, 24))) throw FailedPredicateException(this, "precpred(_ctx, 24)");
          setState(131);
          _la = _input->LA(1);
          if (!(_la == RParser::PLUS

          || _la == RParser::MINUS)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(132);
          expr(25);
          break;
        }

        case 8: {
          auto newContext = _tracker.createInstance<ComparisonContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(133);

          if (!(precpred(_ctx, 23))) throw FailedPredicateException(this, "precpred(_ctx, 23)");
          setState(134);
          rcomparison();
          setState(135);
          expr(24);
          break;
        }

        case 9: {
          auto newContext = _tracker.createInstance<AndExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(137);

          if (!(precpred(_ctx, 20))) throw FailedPredicateException(this, "precpred(_ctx, 20)");
          setState(138);
          match(RParser::AND_OP);
          setState(139);
          expr(21);
          break;
        }

        case 10: {
          auto newContext = _tracker.createInstance<OrExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(140);

          if (!(precpred(_ctx, 19))) throw FailedPredicateException(this, "precpred(_ctx, 19)");
          setState(141);
          _la = _input->LA(1);
          if (!(_la == RParser::BAR

          || _la == RParser::DBAR)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(142);
          expr(20);
          break;
        }

        case 11: {
          auto newContext = _tracker.createInstance<ExpTildeExpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(143);

          if (!(precpred(_ctx, 17))) throw FailedPredicateException(this, "precpred(_ctx, 17)");
          setState(144);
          match(RParser::TILDE);
          setState(145);
          expr(18);
          break;
        }

        case 12: {
          auto newContext = _tracker.createInstance<AssignExpContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(146);

          if (!(precpred(_ctx, 16))) throw FailedPredicateException(this, "precpred(_ctx, 16)");
          setState(147);
          assignOp();
          setState(148);
          expr(17);
          break;
        }

        case 13: {
          auto newContext = _tracker.createInstance<ListRefContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(150);

          if (!(precpred(_ctx, 33))) throw FailedPredicateException(this, "precpred(_ctx, 33)");
          setState(151);
          match(RParser::DBRA_OPEN);
          setState(152);
          sublist();
          setState(153);
          match(RParser::BRA_CLOSE);
          setState(154);
          match(RParser::BRA_CLOSE);
          break;
        }

        case 14: {
          auto newContext = _tracker.createInstance<ArrayContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(156);

          if (!(precpred(_ctx, 32))) throw FailedPredicateException(this, "precpred(_ctx, 32)");
          setState(157);
          match(RParser::BRA_OPEN);
          setState(158);
          sublist();
          setState(159);
          match(RParser::BRA_CLOSE);
          break;
        }

        case 15: {
          auto newContext = _tracker.createInstance<CallFuncContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(161);

          if (!(precpred(_ctx, 14))) throw FailedPredicateException(this, "precpred(_ctx, 14)");
          setState(162);
          match(RParser::PAR_OPEN);
          setState(163);
          sublist();
          setState(164);
          match(RParser::PAR_CLOSE);
          break;
        }

        } 
      }
      setState(170);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- ExprlistContext ------------------------------------------------------------------

RParser::ExprlistContext::ExprlistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RParser::ExprContext *> RParser::ExprlistContext::expr() {
  return getRuleContexts<RParser::ExprContext>();
}

RParser::ExprContext* RParser::ExprlistContext::expr(size_t i) {
  return getRuleContext<RParser::ExprContext>(i);
}

std::vector<tree::TerminalNode *> RParser::ExprlistContext::SEMI() {
  return getTokens(RParser::SEMI);
}

tree::TerminalNode* RParser::ExprlistContext::SEMI(size_t i) {
  return getToken(RParser::SEMI, i);
}

std::vector<tree::TerminalNode *> RParser::ExprlistContext::NL() {
  return getTokens(RParser::NL);
}

tree::TerminalNode* RParser::ExprlistContext::NL(size_t i) {
  return getToken(RParser::NL, i);
}


size_t RParser::ExprlistContext::getRuleIndex() const {
  return RParser::RuleExprlist;
}

void RParser::ExprlistContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExprlist(this);
}

void RParser::ExprlistContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExprlist(this);
}


antlrcpp::Any RParser::ExprlistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitExprlist(this);
  else
    return visitor->visitChildren(this);
}

RParser::ExprlistContext* RParser::exprlist() {
  ExprlistContext *_localctx = _tracker.createInstance<ExprlistContext>(_ctx, getState());
  enterRule(_localctx, 6, RParser::RuleExprlist);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(182);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RParser::HEX:
      case RParser::INT:
      case RParser::FLOAT:
      case RParser::COMPLEX:
      case RParser::STRING:
      case RParser::ID:
      case RParser::IF:
      case RParser::ELSE:
      case RParser::REPEAT:
      case RParser::WHILE:
      case RParser::FOR:
      case RParser::IN:
      case RParser::BREAK:
      case RParser::NEXT:
      case RParser::FUNCTION:
      case RParser::PAR_OPEN:
      case RParser::BRC_OPEN:
      case RParser::QMARK:
      case RParser::PLUS:
      case RParser::MINUS:
      case RParser::NOT:
      case RParser::TILDE:
      case RParser::NULLT:
      case RParser::NaN:
      case RParser::TRUE:
      case RParser::FALSE:
      case RParser::NA:
      case RParser::INF:
      case RParser::COMMENT: {
        enterOuterAlt(_localctx, 1);
        setState(171);
        expr(0);
        setState(178);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while (_la == RParser::SEMI

        || _la == RParser::NL) {
          setState(172);
          _la = _input->LA(1);
          if (!(_la == RParser::SEMI

          || _la == RParser::NL)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(174);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if ((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << RParser::HEX)
            | (1ULL << RParser::INT)
            | (1ULL << RParser::FLOAT)
            | (1ULL << RParser::COMPLEX)
            | (1ULL << RParser::STRING)
            | (1ULL << RParser::ID)
            | (1ULL << RParser::IF)
            | (1ULL << RParser::ELSE)
            | (1ULL << RParser::REPEAT)
            | (1ULL << RParser::WHILE)
            | (1ULL << RParser::FOR)
            | (1ULL << RParser::IN)
            | (1ULL << RParser::BREAK)
            | (1ULL << RParser::NEXT)
            | (1ULL << RParser::FUNCTION)
            | (1ULL << RParser::PAR_OPEN)
            | (1ULL << RParser::BRC_OPEN)
            | (1ULL << RParser::QMARK)
            | (1ULL << RParser::PLUS)
            | (1ULL << RParser::MINUS)
            | (1ULL << RParser::NOT)
            | (1ULL << RParser::TILDE))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
            ((1ULL << (_la - 64)) & ((1ULL << (RParser::NULLT - 64))
            | (1ULL << (RParser::NaN - 64))
            | (1ULL << (RParser::TRUE - 64))
            | (1ULL << (RParser::FALSE - 64))
            | (1ULL << (RParser::NA - 64))
            | (1ULL << (RParser::INF - 64))
            | (1ULL << (RParser::COMMENT - 64)))) != 0)) {
            setState(173);
            expr(0);
          }
          setState(180);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        break;
      }

      case RParser::BRC_CLOSE: {
        enterOuterAlt(_localctx, 2);

        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormlistContext ------------------------------------------------------------------

RParser::FormlistContext::FormlistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RParser::FormContext *> RParser::FormlistContext::form() {
  return getRuleContexts<RParser::FormContext>();
}

RParser::FormContext* RParser::FormlistContext::form(size_t i) {
  return getRuleContext<RParser::FormContext>(i);
}

std::vector<tree::TerminalNode *> RParser::FormlistContext::COMMA() {
  return getTokens(RParser::COMMA);
}

tree::TerminalNode* RParser::FormlistContext::COMMA(size_t i) {
  return getToken(RParser::COMMA, i);
}


size_t RParser::FormlistContext::getRuleIndex() const {
  return RParser::RuleFormlist;
}

void RParser::FormlistContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFormlist(this);
}

void RParser::FormlistContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFormlist(this);
}


antlrcpp::Any RParser::FormlistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitFormlist(this);
  else
    return visitor->visitChildren(this);
}

RParser::FormlistContext* RParser::formlist() {
  FormlistContext *_localctx = _tracker.createInstance<FormlistContext>(_ctx, getState());
  enterRule(_localctx, 8, RParser::RuleFormlist);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(184);
    form();
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RParser::COMMA) {
      setState(185);
      match(RParser::COMMA);
      setState(186);
      form();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormContext ------------------------------------------------------------------

RParser::FormContext::FormContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::FormContext::ID() {
  return getToken(RParser::ID, 0);
}

tree::TerminalNode* RParser::FormContext::EQ_CHAR() {
  return getToken(RParser::EQ_CHAR, 0);
}

RParser::ExprContext* RParser::FormContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::FormContext::TRIPLE_DOT() {
  return getToken(RParser::TRIPLE_DOT, 0);
}

tree::TerminalNode* RParser::FormContext::PERIOD() {
  return getToken(RParser::PERIOD, 0);
}


size_t RParser::FormContext::getRuleIndex() const {
  return RParser::RuleForm;
}

void RParser::FormContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForm(this);
}

void RParser::FormContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForm(this);
}


antlrcpp::Any RParser::FormContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitForm(this);
  else
    return visitor->visitChildren(this);
}

RParser::FormContext* RParser::form() {
  FormContext *_localctx = _tracker.createInstance<FormContext>(_ctx, getState());
  enterRule(_localctx, 10, RParser::RuleForm);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(198);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(192);
      match(RParser::ID);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(193);
      match(RParser::ID);
      setState(194);
      match(RParser::EQ_CHAR);
      setState(195);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(196);
      match(RParser::TRIPLE_DOT);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(197);
      match(RParser::PERIOD);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SublistContext ------------------------------------------------------------------

RParser::SublistContext::SublistContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<RParser::SubContext *> RParser::SublistContext::sub() {
  return getRuleContexts<RParser::SubContext>();
}

RParser::SubContext* RParser::SublistContext::sub(size_t i) {
  return getRuleContext<RParser::SubContext>(i);
}

std::vector<tree::TerminalNode *> RParser::SublistContext::COMMA() {
  return getTokens(RParser::COMMA);
}

tree::TerminalNode* RParser::SublistContext::COMMA(size_t i) {
  return getToken(RParser::COMMA, i);
}


size_t RParser::SublistContext::getRuleIndex() const {
  return RParser::RuleSublist;
}

void RParser::SublistContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSublist(this);
}

void RParser::SublistContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSublist(this);
}


antlrcpp::Any RParser::SublistContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitSublist(this);
  else
    return visitor->visitChildren(this);
}

RParser::SublistContext* RParser::sublist() {
  SublistContext *_localctx = _tracker.createInstance<SublistContext>(_ctx, getState());
  enterRule(_localctx, 12, RParser::RuleSublist);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(200);
    sub();
    setState(205);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == RParser::COMMA) {
      setState(201);
      match(RParser::COMMA);
      setState(202);
      sub();
      setState(207);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SubContext ------------------------------------------------------------------

RParser::SubContext::SubContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::SubContext::ID() {
  return getToken(RParser::ID, 0);
}

tree::TerminalNode* RParser::SubContext::EQ_CHAR() {
  return getToken(RParser::EQ_CHAR, 0);
}

RParser::ExprContext* RParser::SubContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::SubContext::STRING() {
  return getToken(RParser::STRING, 0);
}

tree::TerminalNode* RParser::SubContext::NULLT() {
  return getToken(RParser::NULLT, 0);
}

tree::TerminalNode* RParser::SubContext::TRIPLE_DOT() {
  return getToken(RParser::TRIPLE_DOT, 0);
}

tree::TerminalNode* RParser::SubContext::PERIOD() {
  return getToken(RParser::PERIOD, 0);
}


size_t RParser::SubContext::getRuleIndex() const {
  return RParser::RuleSub;
}

void RParser::SubContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSub(this);
}

void RParser::SubContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSub(this);
}


antlrcpp::Any RParser::SubContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitSub(this);
  else
    return visitor->visitChildren(this);
}

RParser::SubContext* RParser::sub() {
  SubContext *_localctx = _tracker.createInstance<SubContext>(_ctx, getState());
  enterRule(_localctx, 14, RParser::RuleSub);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(227);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(208);
      match(RParser::ID);
      setState(209);
      match(RParser::EQ_CHAR);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(210);
      match(RParser::ID);
      setState(211);
      match(RParser::EQ_CHAR);
      setState(212);
      expr(0);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(213);
      match(RParser::STRING);
      setState(214);
      match(RParser::EQ_CHAR);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(215);
      match(RParser::STRING);
      setState(216);
      match(RParser::EQ_CHAR);
      setState(217);
      expr(0);
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(218);
      match(RParser::NULLT);
      setState(219);
      match(RParser::EQ_CHAR);
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(220);
      match(RParser::NULLT);
      setState(221);
      match(RParser::EQ_CHAR);
      setState(222);
      expr(0);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(223);
      match(RParser::TRIPLE_DOT);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(224);
      match(RParser::PERIOD);
      break;
    }

    case 9: {
      enterOuterAlt(_localctx, 9);
      setState(225);
      expr(0);
      break;
    }

    case 10: {
      enterOuterAlt(_localctx, 10);

      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionCallContext ------------------------------------------------------------------

RParser::FunctionCallContext::FunctionCallContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RParser::ExprContext* RParser::FunctionCallContext::expr() {
  return getRuleContext<RParser::ExprContext>(0);
}

tree::TerminalNode* RParser::FunctionCallContext::PAR_OPEN() {
  return getToken(RParser::PAR_OPEN, 0);
}

RParser::SublistContext* RParser::FunctionCallContext::sublist() {
  return getRuleContext<RParser::SublistContext>(0);
}

tree::TerminalNode* RParser::FunctionCallContext::PAR_CLOSE() {
  return getToken(RParser::PAR_CLOSE, 0);
}


size_t RParser::FunctionCallContext::getRuleIndex() const {
  return RParser::RuleFunctionCall;
}

void RParser::FunctionCallContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionCall(this);
}

void RParser::FunctionCallContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionCall(this);
}


antlrcpp::Any RParser::FunctionCallContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitFunctionCall(this);
  else
    return visitor->visitChildren(this);
}

RParser::FunctionCallContext* RParser::functionCall() {
  FunctionCallContext *_localctx = _tracker.createInstance<FunctionCallContext>(_ctx, getState());
  enterRule(_localctx, 16, RParser::RuleFunctionCall);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(229);
    expr(0);
    setState(230);
    match(RParser::PAR_OPEN);
    setState(231);
    sublist();
    setState(232);
    match(RParser::PAR_CLOSE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RcomparisonContext ------------------------------------------------------------------

RParser::RcomparisonContext::RcomparisonContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::RcomparisonContext::LT() {
  return getToken(RParser::LT, 0);
}

tree::TerminalNode* RParser::RcomparisonContext::LTE() {
  return getToken(RParser::LTE, 0);
}

tree::TerminalNode* RParser::RcomparisonContext::GT() {
  return getToken(RParser::GT, 0);
}

tree::TerminalNode* RParser::RcomparisonContext::GTE() {
  return getToken(RParser::GTE, 0);
}

tree::TerminalNode* RParser::RcomparisonContext::EQUAL_OP() {
  return getToken(RParser::EQUAL_OP, 0);
}

tree::TerminalNode* RParser::RcomparisonContext::NOTEQUAL_OP() {
  return getToken(RParser::NOTEQUAL_OP, 0);
}


size_t RParser::RcomparisonContext::getRuleIndex() const {
  return RParser::RuleRcomparison;
}

void RParser::RcomparisonContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRcomparison(this);
}

void RParser::RcomparisonContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRcomparison(this);
}


antlrcpp::Any RParser::RcomparisonContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitRcomparison(this);
  else
    return visitor->visitChildren(this);
}

RParser::RcomparisonContext* RParser::rcomparison() {
  RcomparisonContext *_localctx = _tracker.createInstance<RcomparisonContext>(_ctx, getState());
  enterRule(_localctx, 18, RParser::RuleRcomparison);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(234);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RParser::LT)
      | (1ULL << RParser::LTE)
      | (1ULL << RParser::GT)
      | (1ULL << RParser::GTE)
      | (1ULL << RParser::EQUAL_OP)
      | (1ULL << RParser::NOTEQUAL_OP))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssignOpContext ------------------------------------------------------------------

RParser::AssignOpContext::AssignOpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::AssignOpContext::ASSIGN() {
  return getToken(RParser::ASSIGN, 0);
}

tree::TerminalNode* RParser::AssignOpContext::DASSIGN() {
  return getToken(RParser::DASSIGN, 0);
}

tree::TerminalNode* RParser::AssignOpContext::RASSIGN() {
  return getToken(RParser::RASSIGN, 0);
}

tree::TerminalNode* RParser::AssignOpContext::DRASSIGN() {
  return getToken(RParser::DRASSIGN, 0);
}

tree::TerminalNode* RParser::AssignOpContext::EQ_CHAR() {
  return getToken(RParser::EQ_CHAR, 0);
}


size_t RParser::AssignOpContext::getRuleIndex() const {
  return RParser::RuleAssignOp;
}

void RParser::AssignOpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssignOp(this);
}

void RParser::AssignOpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssignOp(this);
}


antlrcpp::Any RParser::AssignOpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitAssignOp(this);
  else
    return visitor->visitChildren(this);
}

RParser::AssignOpContext* RParser::assignOp() {
  AssignOpContext *_localctx = _tracker.createInstance<AssignOpContext>(_ctx, getState());
  enterRule(_localctx, 20, RParser::RuleAssignOp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(236);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RParser::ASSIGN)
      | (1ULL << RParser::DASSIGN)
      | (1ULL << RParser::RASSIGN)
      | (1ULL << RParser::DRASSIGN)
      | (1ULL << RParser::EQ_CHAR))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NumberContext ------------------------------------------------------------------

RParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::NumberContext::INT() {
  return getToken(RParser::INT, 0);
}

tree::TerminalNode* RParser::NumberContext::FLOAT() {
  return getToken(RParser::FLOAT, 0);
}

tree::TerminalNode* RParser::NumberContext::COMPLEX() {
  return getToken(RParser::COMPLEX, 0);
}


size_t RParser::NumberContext::getRuleIndex() const {
  return RParser::RuleNumber;
}

void RParser::NumberContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNumber(this);
}

void RParser::NumberContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNumber(this);
}


antlrcpp::Any RParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

RParser::NumberContext* RParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 22, RParser::RuleNumber);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(238);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RParser::INT)
      | (1ULL << RParser::FLOAT)
      | (1ULL << RParser::COMPLEX))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RcommentContext ------------------------------------------------------------------

RParser::RcommentContext::RcommentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RParser::RcommentContext::COMMENT() {
  return getToken(RParser::COMMENT, 0);
}


size_t RParser::RcommentContext::getRuleIndex() const {
  return RParser::RuleRcomment;
}

void RParser::RcommentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRcomment(this);
}

void RParser::RcommentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRcomment(this);
}


antlrcpp::Any RParser::RcommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitRcomment(this);
  else
    return visitor->visitChildren(this);
}

RParser::RcommentContext* RParser::rcomment() {
  RcommentContext *_localctx = _tracker.createInstance<RcommentContext>(_ctx, getState());
  enterRule(_localctx, 24, RParser::RuleRcomment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(240);
    match(RParser::COMMENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- KeywordContext ------------------------------------------------------------------

RParser::KeywordContext::KeywordContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t RParser::KeywordContext::getRuleIndex() const {
  return RParser::RuleKeyword;
}

void RParser::KeywordContext::copyFrom(KeywordContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ForKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::ForKeywordContext::FOR() {
  return getToken(RParser::FOR, 0);
}

RParser::ForKeywordContext::ForKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::ForKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterForKeyword(this);
}
void RParser::ForKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitForKeyword(this);
}

antlrcpp::Any RParser::ForKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitForKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NullKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::NullKeywordContext::NULLT() {
  return getToken(RParser::NULLT, 0);
}

RParser::NullKeywordContext::NullKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::NullKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNullKeyword(this);
}
void RParser::NullKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNullKeyword(this);
}

antlrcpp::Any RParser::NullKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNullKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfKeyWordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::IfKeyWordContext::IF() {
  return getToken(RParser::IF, 0);
}

RParser::IfKeyWordContext::IfKeyWordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::IfKeyWordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIfKeyWord(this);
}
void RParser::IfKeyWordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIfKeyWord(this);
}

antlrcpp::Any RParser::IfKeyWordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitIfKeyWord(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::InKeywordContext::IN() {
  return getToken(RParser::IN, 0);
}

RParser::InKeywordContext::InKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::InKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInKeyword(this);
}
void RParser::InKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInKeyword(this);
}

antlrcpp::Any RParser::InKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitInKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InfKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::InfKeywordContext::INF() {
  return getToken(RParser::INF, 0);
}

RParser::InfKeywordContext::InfKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::InfKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInfKeyword(this);
}
void RParser::InfKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInfKeyword(this);
}

antlrcpp::Any RParser::InfKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitInfKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NanKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::NanKeywordContext::NaN() {
  return getToken(RParser::NaN, 0);
}

RParser::NanKeywordContext::NanKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::NanKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNanKeyword(this);
}
void RParser::NanKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNanKeyword(this);
}

antlrcpp::Any RParser::NanKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNanKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BreakKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::BreakKeywordContext::BREAK() {
  return getToken(RParser::BREAK, 0);
}

RParser::BreakKeywordContext::BreakKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::BreakKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBreakKeyword(this);
}
void RParser::BreakKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBreakKeyword(this);
}

antlrcpp::Any RParser::BreakKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitBreakKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- WhileKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::WhileKeywordContext::WHILE() {
  return getToken(RParser::WHILE, 0);
}

RParser::WhileKeywordContext::WhileKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::WhileKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterWhileKeyword(this);
}
void RParser::WhileKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitWhileKeyword(this);
}

antlrcpp::Any RParser::WhileKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitWhileKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ElseKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::ElseKeywordContext::ELSE() {
  return getToken(RParser::ELSE, 0);
}

RParser::ElseKeywordContext::ElseKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::ElseKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElseKeyword(this);
}
void RParser::ElseKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElseKeyword(this);
}

antlrcpp::Any RParser::ElseKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitElseKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FalseKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::FalseKeywordContext::FALSE() {
  return getToken(RParser::FALSE, 0);
}

RParser::FalseKeywordContext::FalseKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::FalseKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFalseKeyword(this);
}
void RParser::FalseKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFalseKeyword(this);
}

antlrcpp::Any RParser::FalseKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitFalseKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- NaKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::NaKeywordContext::NA() {
  return getToken(RParser::NA, 0);
}

RParser::NaKeywordContext::NaKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::NaKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNaKeyword(this);
}
void RParser::NaKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNaKeyword(this);
}

antlrcpp::Any RParser::NaKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitNaKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- RepeatKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::RepeatKeywordContext::REPEAT() {
  return getToken(RParser::REPEAT, 0);
}

RParser::RepeatKeywordContext::RepeatKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::RepeatKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRepeatKeyword(this);
}
void RParser::RepeatKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRepeatKeyword(this);
}

antlrcpp::Any RParser::RepeatKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitRepeatKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FunctionKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::FunctionKeywordContext::FUNCTION() {
  return getToken(RParser::FUNCTION, 0);
}

RParser::FunctionKeywordContext::FunctionKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::FunctionKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunctionKeyword(this);
}
void RParser::FunctionKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunctionKeyword(this);
}

antlrcpp::Any RParser::FunctionKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitFunctionKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- TrueKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::TrueKeywordContext::TRUE() {
  return getToken(RParser::TRUE, 0);
}

RParser::TrueKeywordContext::TrueKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::TrueKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTrueKeyword(this);
}
void RParser::TrueKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTrueKeyword(this);
}

antlrcpp::Any RParser::TrueKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitTrueKeyword(this);
  else
    return visitor->visitChildren(this);
}
//----------------- HexKeywordContext ------------------------------------------------------------------

tree::TerminalNode* RParser::HexKeywordContext::NEXT() {
  return getToken(RParser::NEXT, 0);
}

RParser::HexKeywordContext::HexKeywordContext(KeywordContext *ctx) { copyFrom(ctx); }

void RParser::HexKeywordContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterHexKeyword(this);
}
void RParser::HexKeywordContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitHexKeyword(this);
}

antlrcpp::Any RParser::HexKeywordContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RVisitor*>(visitor))
    return parserVisitor->visitHexKeyword(this);
  else
    return visitor->visitChildren(this);
}
RParser::KeywordContext* RParser::keyword() {
  KeywordContext *_localctx = _tracker.createInstance<KeywordContext>(_ctx, getState());
  enterRule(_localctx, 26, RParser::RuleKeyword);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(257);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RParser::IF: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::IfKeyWordContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(242);
        match(RParser::IF);
        break;
      }

      case RParser::ELSE: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::ElseKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(243);
        match(RParser::ELSE);
        break;
      }

      case RParser::REPEAT: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::RepeatKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 3);
        setState(244);
        match(RParser::REPEAT);
        break;
      }

      case RParser::WHILE: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::WhileKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 4);
        setState(245);
        match(RParser::WHILE);
        break;
      }

      case RParser::FOR: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::ForKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 5);
        setState(246);
        match(RParser::FOR);
        break;
      }

      case RParser::BREAK: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::BreakKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 6);
        setState(247);
        match(RParser::BREAK);
        break;
      }

      case RParser::IN: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::InKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 7);
        setState(248);
        match(RParser::IN);
        break;
      }

      case RParser::NEXT: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::HexKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 8);
        setState(249);
        match(RParser::NEXT);
        break;
      }

      case RParser::FUNCTION: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::FunctionKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 9);
        setState(250);
        match(RParser::FUNCTION);
        break;
      }

      case RParser::TRUE: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::TrueKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 10);
        setState(251);
        match(RParser::TRUE);
        break;
      }

      case RParser::FALSE: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::FalseKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 11);
        setState(252);
        match(RParser::FALSE);
        break;
      }

      case RParser::NA: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::NaKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 12);
        setState(253);
        match(RParser::NA);
        break;
      }

      case RParser::NaN: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::NanKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 13);
        setState(254);
        match(RParser::NaN);
        break;
      }

      case RParser::INF: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::InfKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 14);
        setState(255);
        match(RParser::INF);
        break;
      }

      case RParser::NULLT: {
        _localctx = dynamic_cast<KeywordContext *>(_tracker.createInstance<RParser::NullKeywordContext>(_localctx));
        enterOuterAlt(_localctx, 15);
        setState(256);
        match(RParser::NULLT);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool RParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 2: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool RParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 31);
    case 1: return precpred(_ctx, 30);
    case 2: return precpred(_ctx, 29);
    case 3: return precpred(_ctx, 27);
    case 4: return precpred(_ctx, 26);
    case 5: return precpred(_ctx, 25);
    case 6: return precpred(_ctx, 24);
    case 7: return precpred(_ctx, 23);
    case 8: return precpred(_ctx, 20);
    case 9: return precpred(_ctx, 19);
    case 10: return precpred(_ctx, 17);
    case 11: return precpred(_ctx, 16);
    case 12: return precpred(_ctx, 33);
    case 13: return precpred(_ctx, 32);
    case 14: return precpred(_ctx, 14);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> RParser::_decisionToDFA;
atn::PredictionContextCache RParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN RParser::_atn;
std::vector<uint16_t> RParser::_serializedATN;

std::vector<std::string> RParser::_ruleNames = {
  "prog", "expr_or_assign", "expr", "exprlist", "formlist", "form", "sublist", 
  "sub", "functionCall", "rcomparison", "assignOp", "number", "rcomment", 
  "keyword"
};

std::vector<std::string> RParser::_literalNames = {
  "", "", "", "", "", "", "", "'if'", "'else'", "'repeat'", "'while'", "'for'", 
  "'in'", "'break'", "'next'", "'function'", "", "'('", "')'", "'['", "']'", 
  "'[['", "']]'", "'{'", "'}'", "'?'", "'+'", "'-'", "'*'", "'/'", "'<'", 
  "'<='", "'>'", "'>='", "'=='", "'!='", "'!'", "'%'", "'#'", "'|'", "'||'", 
  "'&'", "'&&'", "':='", "'<-'", "'<<-'", "'->'", "'->>'", "'='", "':::'", 
  "'::'", "':'", "'$'", "'@'", "'~'", "'_'", "'.'", "'''", "'\"'", "'...'", 
  "','", "'`'", "';'", "'^'", "'NULL'", "'NaN'", "'TRUE'", "'FALSE'", "'NA'", 
  "'Inf'"
};

std::vector<std::string> RParser::_symbolicNames = {
  "", "HEX", "INT", "FLOAT", "COMPLEX", "STRING", "ID", "IF", "ELSE", "REPEAT", 
  "WHILE", "FOR", "IN", "BREAK", "NEXT", "FUNCTION", "AND_OP", "PAR_OPEN", 
  "PAR_CLOSE", "BRA_OPEN", "BRA_CLOSE", "DBRA_OPEN", "DBRA_CLOSE", "BRC_OPEN", 
  "BRC_CLOSE", "QMARK", "PLUS", "MINUS", "ASTERICK", "SLASH", "LT", "LTE", 
  "GT", "GTE", "EQUAL_OP", "NOTEQUAL_OP", "NOT", "PERCENT", "HASH", "BAR", 
  "DBAR", "AMP", "DAMP", "COLEQ", "ASSIGN", "DASSIGN", "RASSIGN", "DRASSIGN", 
  "EQ_CHAR", "TRIPLE_COLON", "DOUBLE_COLON", "COLON", "DOLLAR_CHAR", "AT_CHAR", 
  "TILDE", "USCORE", "PERIOD", "SQUOTE", "DQUOTE", "TRIPLE_DOT", "COMMA", 
  "BACKTICK", "SEMI", "CARAT", "NULLT", "NaN", "TRUE", "FALSE", "NA", "INF", 
  "USER_OP", "COMMENT", "NL", "WS"
};

dfa::Vocabulary RParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> RParser::_tokenNames;

RParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x4b, 0x106, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x21, 0xa, 0x2, 0xc, 
    0x2, 0xe, 0x2, 0x24, 0xb, 0x2, 0x3, 0x2, 0x7, 0x2, 0x27, 0xa, 0x2, 0xc, 
    0x2, 0xe, 0x2, 0x2a, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x33, 0xa, 0x3, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x40, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x71, 0xa, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0xa9, 0xa, 0x4, 0xc, 0x4, 
    0xe, 0x4, 0xac, 0xb, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0xb1, 
    0xa, 0x5, 0x7, 0x5, 0xb3, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0xb6, 0xb, 0x5, 
    0x3, 0x5, 0x5, 0x5, 0xb9, 0xa, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 
    0x6, 0xbe, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 0xc1, 0xb, 0x6, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0xc9, 0xa, 0x7, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0xce, 0xa, 0x8, 0xc, 0x8, 0xe, 
    0x8, 0xd1, 0xb, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x5, 0x9, 0xe6, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 
    0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 
    0xf, 0x3, 0xf, 0x3, 0xf, 0x5, 0xf, 0x104, 0xa, 0xf, 0x3, 0xf, 0x2, 0x3, 
    0x6, 0x10, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
    0x18, 0x1a, 0x1c, 0x2, 0xb, 0x4, 0x2, 0x40, 0x40, 0x4a, 0x4a, 0x3, 0x2, 
    0x1c, 0x1d, 0x3, 0x2, 0x33, 0x34, 0x3, 0x2, 0x36, 0x37, 0x3, 0x2, 0x1e, 
    0x1f, 0x3, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x20, 0x25, 0x3, 0x2, 0x2e, 0x32, 
    0x3, 0x2, 0x4, 0x6, 0x2, 0x13b, 0x2, 0x28, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x32, 0x3, 0x2, 0x2, 0x2, 0x6, 0x70, 0x3, 0x2, 0x2, 0x2, 0x8, 0xb8, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0xba, 0x3, 0x2, 0x2, 0x2, 0xc, 0xc8, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0xca, 0x3, 0x2, 0x2, 0x2, 0x10, 0xe5, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0xe7, 0x3, 0x2, 0x2, 0x2, 0x14, 0xec, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0xee, 0x3, 0x2, 0x2, 0x2, 0x18, 0xf0, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0xf2, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x103, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x22, 
    0x5, 0x4, 0x3, 0x2, 0x1f, 0x21, 0x9, 0x2, 0x2, 0x2, 0x20, 0x1f, 0x3, 
    0x2, 0x2, 0x2, 0x21, 0x24, 0x3, 0x2, 0x2, 0x2, 0x22, 0x20, 0x3, 0x2, 
    0x2, 0x2, 0x22, 0x23, 0x3, 0x2, 0x2, 0x2, 0x23, 0x27, 0x3, 0x2, 0x2, 
    0x2, 0x24, 0x22, 0x3, 0x2, 0x2, 0x2, 0x25, 0x27, 0x7, 0x4a, 0x2, 0x2, 
    0x26, 0x1e, 0x3, 0x2, 0x2, 0x2, 0x26, 0x25, 0x3, 0x2, 0x2, 0x2, 0x27, 
    0x2a, 0x3, 0x2, 0x2, 0x2, 0x28, 0x26, 0x3, 0x2, 0x2, 0x2, 0x28, 0x29, 
    0x3, 0x2, 0x2, 0x2, 0x29, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x28, 0x3, 
    0x2, 0x2, 0x2, 0x2b, 0x2c, 0x7, 0x2, 0x2, 0x3, 0x2c, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2d, 0x2e, 0x5, 0x6, 0x4, 0x2, 0x2e, 0x2f, 0x5, 0x16, 0xc, 
    0x2, 0x2f, 0x30, 0x5, 0x4, 0x3, 0x2, 0x30, 0x33, 0x3, 0x2, 0x2, 0x2, 
    0x31, 0x33, 0x5, 0x6, 0x4, 0x2, 0x32, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x32, 
    0x31, 0x3, 0x2, 0x2, 0x2, 0x33, 0x5, 0x3, 0x2, 0x2, 0x2, 0x34, 0x35, 
    0x8, 0x4, 0x1, 0x2, 0x35, 0x36, 0x9, 0x3, 0x2, 0x2, 0x36, 0x71, 0x5, 
    0x6, 0x4, 0x1e, 0x37, 0x71, 0x5, 0x1a, 0xe, 0x2, 0x38, 0x39, 0x7, 0x26, 
    0x2, 0x2, 0x39, 0x71, 0x5, 0x6, 0x4, 0x17, 0x3a, 0x3b, 0x7, 0x38, 0x2, 
    0x2, 0x3b, 0x71, 0x5, 0x6, 0x4, 0x14, 0x3c, 0x3d, 0x7, 0x11, 0x2, 0x2, 
    0x3d, 0x3f, 0x7, 0x13, 0x2, 0x2, 0x3e, 0x40, 0x5, 0xa, 0x6, 0x2, 0x3f, 
    0x3e, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 0x3, 0x2, 0x2, 0x2, 0x40, 0x41, 
    0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x7, 0x14, 0x2, 0x2, 0x42, 0x71, 0x5, 
    0x6, 0x4, 0x11, 0x43, 0x44, 0x7, 0x19, 0x2, 0x2, 0x44, 0x45, 0x5, 0x8, 
    0x5, 0x2, 0x45, 0x46, 0x7, 0x1a, 0x2, 0x2, 0x46, 0x71, 0x3, 0x2, 0x2, 
    0x2, 0x47, 0x48, 0x7, 0x9, 0x2, 0x2, 0x48, 0x49, 0x7, 0x13, 0x2, 0x2, 
    0x49, 0x4a, 0x5, 0x6, 0x4, 0x2, 0x4a, 0x4b, 0x7, 0x14, 0x2, 0x2, 0x4b, 
    0x4c, 0x5, 0x6, 0x4, 0xe, 0x4c, 0x71, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 
    0x7, 0x9, 0x2, 0x2, 0x4e, 0x4f, 0x7, 0x13, 0x2, 0x2, 0x4f, 0x50, 0x5, 
    0x6, 0x4, 0x2, 0x50, 0x51, 0x7, 0x14, 0x2, 0x2, 0x51, 0x52, 0x5, 0x6, 
    0x4, 0x2, 0x52, 0x53, 0x7, 0xa, 0x2, 0x2, 0x53, 0x54, 0x5, 0x6, 0x4, 
    0xd, 0x54, 0x71, 0x3, 0x2, 0x2, 0x2, 0x55, 0x56, 0x7, 0xd, 0x2, 0x2, 
    0x56, 0x57, 0x7, 0x13, 0x2, 0x2, 0x57, 0x58, 0x7, 0x8, 0x2, 0x2, 0x58, 
    0x59, 0x7, 0xe, 0x2, 0x2, 0x59, 0x5a, 0x5, 0x6, 0x4, 0x2, 0x5a, 0x5b, 
    0x7, 0x14, 0x2, 0x2, 0x5b, 0x5c, 0x5, 0x6, 0x4, 0xc, 0x5c, 0x71, 0x3, 
    0x2, 0x2, 0x2, 0x5d, 0x5e, 0x7, 0xc, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x13, 
    0x2, 0x2, 0x5f, 0x60, 0x5, 0x6, 0x4, 0x2, 0x60, 0x61, 0x7, 0x14, 0x2, 
    0x2, 0x61, 0x62, 0x5, 0x6, 0x4, 0xb, 0x62, 0x71, 0x3, 0x2, 0x2, 0x2, 
    0x63, 0x64, 0x7, 0xb, 0x2, 0x2, 0x64, 0x71, 0x5, 0x6, 0x4, 0xa, 0x65, 
    0x66, 0x7, 0x1b, 0x2, 0x2, 0x66, 0x71, 0x5, 0x6, 0x4, 0x9, 0x67, 0x71, 
    0x5, 0x1c, 0xf, 0x2, 0x68, 0x69, 0x7, 0x13, 0x2, 0x2, 0x69, 0x6a, 0x5, 
    0x6, 0x4, 0x2, 0x6a, 0x6b, 0x7, 0x14, 0x2, 0x2, 0x6b, 0x71, 0x3, 0x2, 
    0x2, 0x2, 0x6c, 0x71, 0x7, 0x8, 0x2, 0x2, 0x6d, 0x71, 0x7, 0x7, 0x2, 
    0x2, 0x6e, 0x71, 0x7, 0x3, 0x2, 0x2, 0x6f, 0x71, 0x5, 0x18, 0xd, 0x2, 
    0x70, 0x34, 0x3, 0x2, 0x2, 0x2, 0x70, 0x37, 0x3, 0x2, 0x2, 0x2, 0x70, 
    0x38, 0x3, 0x2, 0x2, 0x2, 0x70, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x70, 0x3c, 
    0x3, 0x2, 0x2, 0x2, 0x70, 0x43, 0x3, 0x2, 0x2, 0x2, 0x70, 0x47, 0x3, 
    0x2, 0x2, 0x2, 0x70, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x70, 0x55, 0x3, 0x2, 
    0x2, 0x2, 0x70, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x70, 0x63, 0x3, 0x2, 0x2, 
    0x2, 0x70, 0x65, 0x3, 0x2, 0x2, 0x2, 0x70, 0x67, 0x3, 0x2, 0x2, 0x2, 
    0x70, 0x68, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x70, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6f, 
    0x3, 0x2, 0x2, 0x2, 0x71, 0xaa, 0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0xc, 
    0x21, 0x2, 0x2, 0x73, 0x74, 0x9, 0x4, 0x2, 0x2, 0x74, 0xa9, 0x5, 0x6, 
    0x4, 0x22, 0x75, 0x76, 0xc, 0x20, 0x2, 0x2, 0x76, 0x77, 0x9, 0x5, 0x2, 
    0x2, 0x77, 0xa9, 0x5, 0x6, 0x4, 0x21, 0x78, 0x79, 0xc, 0x1f, 0x2, 0x2, 
    0x79, 0x7a, 0x7, 0x41, 0x2, 0x2, 0x7a, 0xa9, 0x5, 0x6, 0x4, 0x1f, 0x7b, 
    0x7c, 0xc, 0x1d, 0x2, 0x2, 0x7c, 0x7d, 0x7, 0x35, 0x2, 0x2, 0x7d, 0xa9, 
    0x5, 0x6, 0x4, 0x1e, 0x7e, 0x7f, 0xc, 0x1c, 0x2, 0x2, 0x7f, 0x80, 0x7, 
    0x48, 0x2, 0x2, 0x80, 0xa9, 0x5, 0x6, 0x4, 0x1d, 0x81, 0x82, 0xc, 0x1b, 
    0x2, 0x2, 0x82, 0x83, 0x9, 0x6, 0x2, 0x2, 0x83, 0xa9, 0x5, 0x6, 0x4, 
    0x1c, 0x84, 0x85, 0xc, 0x1a, 0x2, 0x2, 0x85, 0x86, 0x9, 0x3, 0x2, 0x2, 
    0x86, 0xa9, 0x5, 0x6, 0x4, 0x1b, 0x87, 0x88, 0xc, 0x19, 0x2, 0x2, 0x88, 
    0x89, 0x5, 0x14, 0xb, 0x2, 0x89, 0x8a, 0x5, 0x6, 0x4, 0x1a, 0x8a, 0xa9, 
    0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 0xc, 0x16, 0x2, 0x2, 0x8c, 0x8d, 0x7, 
    0x12, 0x2, 0x2, 0x8d, 0xa9, 0x5, 0x6, 0x4, 0x17, 0x8e, 0x8f, 0xc, 0x15, 
    0x2, 0x2, 0x8f, 0x90, 0x9, 0x7, 0x2, 0x2, 0x90, 0xa9, 0x5, 0x6, 0x4, 
    0x16, 0x91, 0x92, 0xc, 0x13, 0x2, 0x2, 0x92, 0x93, 0x7, 0x38, 0x2, 0x2, 
    0x93, 0xa9, 0x5, 0x6, 0x4, 0x14, 0x94, 0x95, 0xc, 0x12, 0x2, 0x2, 0x95, 
    0x96, 0x5, 0x16, 0xc, 0x2, 0x96, 0x97, 0x5, 0x6, 0x4, 0x13, 0x97, 0xa9, 
    0x3, 0x2, 0x2, 0x2, 0x98, 0x99, 0xc, 0x23, 0x2, 0x2, 0x99, 0x9a, 0x7, 
    0x17, 0x2, 0x2, 0x9a, 0x9b, 0x5, 0xe, 0x8, 0x2, 0x9b, 0x9c, 0x7, 0x16, 
    0x2, 0x2, 0x9c, 0x9d, 0x7, 0x16, 0x2, 0x2, 0x9d, 0xa9, 0x3, 0x2, 0x2, 
    0x2, 0x9e, 0x9f, 0xc, 0x22, 0x2, 0x2, 0x9f, 0xa0, 0x7, 0x15, 0x2, 0x2, 
    0xa0, 0xa1, 0x5, 0xe, 0x8, 0x2, 0xa1, 0xa2, 0x7, 0x16, 0x2, 0x2, 0xa2, 
    0xa9, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0xc, 0x10, 0x2, 0x2, 0xa4, 0xa5, 
    0x7, 0x13, 0x2, 0x2, 0xa5, 0xa6, 0x5, 0xe, 0x8, 0x2, 0xa6, 0xa7, 0x7, 
    0x14, 0x2, 0x2, 0xa7, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x72, 0x3, 0x2, 
    0x2, 0x2, 0xa8, 0x75, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x78, 0x3, 0x2, 0x2, 
    0x2, 0xa8, 0x7b, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x7e, 0x3, 0x2, 0x2, 0x2, 
    0xa8, 0x81, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x84, 0x3, 0x2, 0x2, 0x2, 0xa8, 
    0x87, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x8b, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x8e, 
    0x3, 0x2, 0x2, 0x2, 0xa8, 0x91, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x94, 0x3, 
    0x2, 0x2, 0x2, 0xa8, 0x98, 0x3, 0x2, 0x2, 0x2, 0xa8, 0x9e, 0x3, 0x2, 
    0x2, 0x2, 0xa8, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xac, 0x3, 0x2, 0x2, 
    0x2, 0xaa, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xab, 0x3, 0x2, 0x2, 0x2, 
    0xab, 0x7, 0x3, 0x2, 0x2, 0x2, 0xac, 0xaa, 0x3, 0x2, 0x2, 0x2, 0xad, 
    0xb4, 0x5, 0x6, 0x4, 0x2, 0xae, 0xb0, 0x9, 0x2, 0x2, 0x2, 0xaf, 0xb1, 
    0x5, 0x6, 0x4, 0x2, 0xb0, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xb1, 0x3, 
    0x2, 0x2, 0x2, 0xb1, 0xb3, 0x3, 0x2, 0x2, 0x2, 0xb2, 0xae, 0x3, 0x2, 
    0x2, 0x2, 0xb3, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb2, 0x3, 0x2, 0x2, 
    0x2, 0xb4, 0xb5, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xb9, 0x3, 0x2, 0x2, 0x2, 
    0xb6, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb7, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xb8, 
    0xad, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb9, 0x9, 
    0x3, 0x2, 0x2, 0x2, 0xba, 0xbf, 0x5, 0xc, 0x7, 0x2, 0xbb, 0xbc, 0x7, 
    0x3e, 0x2, 0x2, 0xbc, 0xbe, 0x5, 0xc, 0x7, 0x2, 0xbd, 0xbb, 0x3, 0x2, 
    0x2, 0x2, 0xbe, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xbd, 0x3, 0x2, 0x2, 
    0x2, 0xbf, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xb, 0x3, 0x2, 0x2, 0x2, 
    0xc1, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc9, 0x7, 0x8, 0x2, 0x2, 0xc3, 
    0xc4, 0x7, 0x8, 0x2, 0x2, 0xc4, 0xc5, 0x7, 0x32, 0x2, 0x2, 0xc5, 0xc9, 
    0x5, 0x6, 0x4, 0x2, 0xc6, 0xc9, 0x7, 0x3d, 0x2, 0x2, 0xc7, 0xc9, 0x7, 
    0x3a, 0x2, 0x2, 0xc8, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc3, 0x3, 0x2, 
    0x2, 0x2, 0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc7, 0x3, 0x2, 0x2, 
    0x2, 0xc9, 0xd, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcf, 0x5, 0x10, 0x9, 0x2, 
    0xcb, 0xcc, 0x7, 0x3e, 0x2, 0x2, 0xcc, 0xce, 0x5, 0x10, 0x9, 0x2, 0xcd, 
    0xcb, 0x3, 0x2, 0x2, 0x2, 0xce, 0xd1, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xcd, 
    0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0xd1, 0xcf, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x7, 0x8, 
    0x2, 0x2, 0xd3, 0xe6, 0x7, 0x32, 0x2, 0x2, 0xd4, 0xd5, 0x7, 0x8, 0x2, 
    0x2, 0xd5, 0xd6, 0x7, 0x32, 0x2, 0x2, 0xd6, 0xe6, 0x5, 0x6, 0x4, 0x2, 
    0xd7, 0xd8, 0x7, 0x7, 0x2, 0x2, 0xd8, 0xe6, 0x7, 0x32, 0x2, 0x2, 0xd9, 
    0xda, 0x7, 0x7, 0x2, 0x2, 0xda, 0xdb, 0x7, 0x32, 0x2, 0x2, 0xdb, 0xe6, 
    0x5, 0x6, 0x4, 0x2, 0xdc, 0xdd, 0x7, 0x42, 0x2, 0x2, 0xdd, 0xe6, 0x7, 
    0x32, 0x2, 0x2, 0xde, 0xdf, 0x7, 0x42, 0x2, 0x2, 0xdf, 0xe0, 0x7, 0x32, 
    0x2, 0x2, 0xe0, 0xe6, 0x5, 0x6, 0x4, 0x2, 0xe1, 0xe6, 0x7, 0x3d, 0x2, 
    0x2, 0xe2, 0xe6, 0x7, 0x3a, 0x2, 0x2, 0xe3, 0xe6, 0x5, 0x6, 0x4, 0x2, 
    0xe4, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xd2, 0x3, 0x2, 0x2, 0x2, 0xe5, 
    0xd4, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xd7, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xd9, 
    0x3, 0x2, 0x2, 0x2, 0xe5, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xde, 0x3, 
    0x2, 0x2, 0x2, 0xe5, 0xe1, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe2, 0x3, 0x2, 
    0x2, 0x2, 0xe5, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe4, 0x3, 0x2, 0x2, 
    0x2, 0xe6, 0x11, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x5, 0x6, 0x4, 0x2, 
    0xe8, 0xe9, 0x7, 0x13, 0x2, 0x2, 0xe9, 0xea, 0x5, 0xe, 0x8, 0x2, 0xea, 
    0xeb, 0x7, 0x14, 0x2, 0x2, 0xeb, 0x13, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 
    0x9, 0x8, 0x2, 0x2, 0xed, 0x15, 0x3, 0x2, 0x2, 0x2, 0xee, 0xef, 0x9, 
    0x9, 0x2, 0x2, 0xef, 0x17, 0x3, 0x2, 0x2, 0x2, 0xf0, 0xf1, 0x9, 0xa, 
    0x2, 0x2, 0xf1, 0x19, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf3, 0x7, 0x49, 0x2, 
    0x2, 0xf3, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xf4, 0x104, 0x7, 0x9, 0x2, 0x2, 
    0xf5, 0x104, 0x7, 0xa, 0x2, 0x2, 0xf6, 0x104, 0x7, 0xb, 0x2, 0x2, 0xf7, 
    0x104, 0x7, 0xc, 0x2, 0x2, 0xf8, 0x104, 0x7, 0xd, 0x2, 0x2, 0xf9, 0x104, 
    0x7, 0xf, 0x2, 0x2, 0xfa, 0x104, 0x7, 0xe, 0x2, 0x2, 0xfb, 0x104, 0x7, 
    0x10, 0x2, 0x2, 0xfc, 0x104, 0x7, 0x11, 0x2, 0x2, 0xfd, 0x104, 0x7, 
    0x44, 0x2, 0x2, 0xfe, 0x104, 0x7, 0x45, 0x2, 0x2, 0xff, 0x104, 0x7, 
    0x46, 0x2, 0x2, 0x100, 0x104, 0x7, 0x43, 0x2, 0x2, 0x101, 0x104, 0x7, 
    0x47, 0x2, 0x2, 0x102, 0x104, 0x7, 0x42, 0x2, 0x2, 0x103, 0xf4, 0x3, 
    0x2, 0x2, 0x2, 0x103, 0xf5, 0x3, 0x2, 0x2, 0x2, 0x103, 0xf6, 0x3, 0x2, 
    0x2, 0x2, 0x103, 0xf7, 0x3, 0x2, 0x2, 0x2, 0x103, 0xf8, 0x3, 0x2, 0x2, 
    0x2, 0x103, 0xf9, 0x3, 0x2, 0x2, 0x2, 0x103, 0xfa, 0x3, 0x2, 0x2, 0x2, 
    0x103, 0xfb, 0x3, 0x2, 0x2, 0x2, 0x103, 0xfc, 0x3, 0x2, 0x2, 0x2, 0x103, 
    0xfd, 0x3, 0x2, 0x2, 0x2, 0x103, 0xfe, 0x3, 0x2, 0x2, 0x2, 0x103, 0xff, 
    0x3, 0x2, 0x2, 0x2, 0x103, 0x100, 0x3, 0x2, 0x2, 0x2, 0x103, 0x101, 
    0x3, 0x2, 0x2, 0x2, 0x103, 0x102, 0x3, 0x2, 0x2, 0x2, 0x104, 0x1d, 0x3, 
    0x2, 0x2, 0x2, 0x12, 0x22, 0x26, 0x28, 0x32, 0x3f, 0x70, 0xa8, 0xaa, 
    0xb0, 0xb4, 0xb8, 0xbf, 0xc8, 0xcf, 0xe5, 0x103, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

RParser::Initializer RParser::_init;
