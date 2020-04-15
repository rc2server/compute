
// Generated from parser/RFilter.g4 by ANTLR 4.8


#include "RFilterListener.h"
#include "RFilterVisitor.h"

#include "RFilter.h"


using namespace antlrcpp;
using namespace antlr4;

RFilter::RFilter(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

RFilter::~RFilter() {
  delete _interpreter;
}

std::string RFilter::getGrammarFileName() const {
  return "RFilter.g4";
}

const std::vector<std::string>& RFilter::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& RFilter::getVocabulary() const {
  return _vocabulary;
}


//----------------- StreamContext ------------------------------------------------------------------

RFilter::StreamContext::StreamContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RFilter::StreamContext::EOF() {
  return getToken(RFilter::EOF, 0);
}

std::vector<RFilter::ElemContext *> RFilter::StreamContext::elem() {
  return getRuleContexts<RFilter::ElemContext>();
}

RFilter::ElemContext* RFilter::StreamContext::elem(size_t i) {
  return getRuleContext<RFilter::ElemContext>(i);
}

std::vector<tree::TerminalNode *> RFilter::StreamContext::NL() {
  return getTokens(RFilter::NL);
}

tree::TerminalNode* RFilter::StreamContext::NL(size_t i) {
  return getToken(RFilter::NL, i);
}

std::vector<tree::TerminalNode *> RFilter::StreamContext::SEMI() {
  return getTokens(RFilter::SEMI);
}

tree::TerminalNode* RFilter::StreamContext::SEMI(size_t i) {
  return getToken(RFilter::SEMI, i);
}


size_t RFilter::StreamContext::getRuleIndex() const {
  return RFilter::RuleStream;
}

void RFilter::StreamContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStream(this);
}

void RFilter::StreamContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStream(this);
}


antlrcpp::Any RFilter::StreamContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitStream(this);
  else
    return visitor->visitChildren(this);
}

RFilter::StreamContext* RFilter::stream() {
  StreamContext *_localctx = _tracker.createInstance<StreamContext>(_ctx, getState());
  enterRule(_localctx, 0, RFilter::RuleStream);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(17);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RFilter::HEX)
      | (1ULL << RFilter::INT)
      | (1ULL << RFilter::FLOAT)
      | (1ULL << RFilter::COMPLEX)
      | (1ULL << RFilter::STRING)
      | (1ULL << RFilter::ID)
      | (1ULL << RFilter::IF)
      | (1ULL << RFilter::ELSE)
      | (1ULL << RFilter::REPEAT)
      | (1ULL << RFilter::WHILE)
      | (1ULL << RFilter::FOR)
      | (1ULL << RFilter::IN)
      | (1ULL << RFilter::BREAK)
      | (1ULL << RFilter::NEXT)
      | (1ULL << RFilter::FUNCTION)
      | (1ULL << RFilter::PAR_OPEN)
      | (1ULL << RFilter::BRA_OPEN)
      | (1ULL << RFilter::DBRA_OPEN)
      | (1ULL << RFilter::BRC_OPEN)
      | (1ULL << RFilter::QMARK)
      | (1ULL << RFilter::PLUS)
      | (1ULL << RFilter::MINUS)
      | (1ULL << RFilter::ASTERICK)
      | (1ULL << RFilter::SLASH)
      | (1ULL << RFilter::LT)
      | (1ULL << RFilter::LTE)
      | (1ULL << RFilter::GT)
      | (1ULL << RFilter::GTE)
      | (1ULL << RFilter::EQUAL_OP)
      | (1ULL << RFilter::NOTEQUAL_OP)
      | (1ULL << RFilter::NOT)
      | (1ULL << RFilter::BAR)
      | (1ULL << RFilter::DBAR)
      | (1ULL << RFilter::AMP)
      | (1ULL << RFilter::DAMP)
      | (1ULL << RFilter::ASSIGN)
      | (1ULL << RFilter::RASSIGN)
      | (1ULL << RFilter::EQ_CHAR)
      | (1ULL << RFilter::TRIPLE_COLON)
      | (1ULL << RFilter::DOUBLE_COLON)
      | (1ULL << RFilter::COLON)
      | (1ULL << RFilter::DOLLAR_CHAR)
      | (1ULL << RFilter::AT_CHAR)
      | (1ULL << RFilter::TILDE)
      | (1ULL << RFilter::TRIPLE_DOT)
      | (1ULL << RFilter::COMMA)
      | (1ULL << RFilter::SEMI)
      | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
      | (1ULL << (RFilter::NaN - 64))
      | (1ULL << (RFilter::TRUE - 64))
      | (1ULL << (RFilter::FALSE - 64))
      | (1ULL << (RFilter::NA - 64))
      | (1ULL << (RFilter::INF - 64))
      | (1ULL << (RFilter::USER_OP - 64))
      | (1ULL << (RFilter::COMMENT - 64))
      | (1ULL << (RFilter::NL - 64)))) != 0)) {
      setState(15);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case RFilter::HEX:
        case RFilter::INT:
        case RFilter::FLOAT:
        case RFilter::COMPLEX:
        case RFilter::STRING:
        case RFilter::ID:
        case RFilter::IF:
        case RFilter::ELSE:
        case RFilter::REPEAT:
        case RFilter::WHILE:
        case RFilter::FOR:
        case RFilter::IN:
        case RFilter::BREAK:
        case RFilter::NEXT:
        case RFilter::FUNCTION:
        case RFilter::PAR_OPEN:
        case RFilter::BRA_OPEN:
        case RFilter::DBRA_OPEN:
        case RFilter::BRC_OPEN:
        case RFilter::QMARK:
        case RFilter::PLUS:
        case RFilter::MINUS:
        case RFilter::ASTERICK:
        case RFilter::SLASH:
        case RFilter::LT:
        case RFilter::LTE:
        case RFilter::GT:
        case RFilter::GTE:
        case RFilter::EQUAL_OP:
        case RFilter::NOTEQUAL_OP:
        case RFilter::NOT:
        case RFilter::BAR:
        case RFilter::DBAR:
        case RFilter::AMP:
        case RFilter::DAMP:
        case RFilter::ASSIGN:
        case RFilter::RASSIGN:
        case RFilter::EQ_CHAR:
        case RFilter::TRIPLE_COLON:
        case RFilter::DOUBLE_COLON:
        case RFilter::COLON:
        case RFilter::DOLLAR_CHAR:
        case RFilter::AT_CHAR:
        case RFilter::TILDE:
        case RFilter::TRIPLE_DOT:
        case RFilter::COMMA:
        case RFilter::CARAT:
        case RFilter::NULLT:
        case RFilter::NaN:
        case RFilter::TRUE:
        case RFilter::FALSE:
        case RFilter::NA:
        case RFilter::INF:
        case RFilter::USER_OP:
        case RFilter::COMMENT: {
          setState(12);
          elem();
          break;
        }

        case RFilter::NL: {
          setState(13);
          match(RFilter::NL);
          break;
        }

        case RFilter::SEMI: {
          setState(14);
          match(RFilter::SEMI);
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(19);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(20);
    match(RFilter::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- EatContext ------------------------------------------------------------------

RFilter::EatContext::EatContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> RFilter::EatContext::NL() {
  return getTokens(RFilter::NL);
}

tree::TerminalNode* RFilter::EatContext::NL(size_t i) {
  return getToken(RFilter::NL, i);
}


size_t RFilter::EatContext::getRuleIndex() const {
  return RFilter::RuleEat;
}

void RFilter::EatContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEat(this);
}

void RFilter::EatContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEat(this);
}


antlrcpp::Any RFilter::EatContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitEat(this);
  else
    return visitor->visitChildren(this);
}

RFilter::EatContext* RFilter::eat() {
  EatContext *_localctx = _tracker.createInstance<EatContext>(_ctx, getState());
  enterRule(_localctx, 2, RFilter::RuleEat);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(24); 
    _errHandler->sync(this);
    alt = 1;
    do {
      switch (alt) {
        case 1: {
              setState(22);
              dynamic_cast<EatContext *>(_localctx)->nlToken = match(RFilter::NL);
               (dynamic_cast<WritableToken*>(dynamic_cast<EatContext *>(_localctx)->nlToken))->setChannel(Token::HIDDEN_CHANNEL);
              break;
            }

      default:
        throw NoViableAltException(this);
      }
      setState(26); 
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 2, _ctx);
    } while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ElemContext ------------------------------------------------------------------

RFilter::ElemContext::ElemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

RFilter::OpContext* RFilter::ElemContext::op() {
  return getRuleContext<RFilter::OpContext>(0);
}

std::vector<RFilter::EatContext *> RFilter::ElemContext::eat() {
  return getRuleContexts<RFilter::EatContext>();
}

RFilter::EatContext* RFilter::ElemContext::eat(size_t i) {
  return getRuleContext<RFilter::EatContext>(i);
}

RFilter::AtomContext* RFilter::ElemContext::atom() {
  return getRuleContext<RFilter::AtomContext>(0);
}

RFilter::CommentContext* RFilter::ElemContext::comment() {
  return getRuleContext<RFilter::CommentContext>(0);
}

tree::TerminalNode* RFilter::ElemContext::BRC_OPEN() {
  return getToken(RFilter::BRC_OPEN, 0);
}

tree::TerminalNode* RFilter::ElemContext::BRC_CLOSE() {
  return getToken(RFilter::BRC_CLOSE, 0);
}

std::vector<RFilter::ElemContext *> RFilter::ElemContext::elem() {
  return getRuleContexts<RFilter::ElemContext>();
}

RFilter::ElemContext* RFilter::ElemContext::elem(size_t i) {
  return getRuleContext<RFilter::ElemContext>(i);
}

std::vector<tree::TerminalNode *> RFilter::ElemContext::NL() {
  return getTokens(RFilter::NL);
}

tree::TerminalNode* RFilter::ElemContext::NL(size_t i) {
  return getToken(RFilter::NL, i);
}

std::vector<tree::TerminalNode *> RFilter::ElemContext::SEMI() {
  return getTokens(RFilter::SEMI);
}

tree::TerminalNode* RFilter::ElemContext::SEMI(size_t i) {
  return getToken(RFilter::SEMI, i);
}

tree::TerminalNode* RFilter::ElemContext::PAR_OPEN() {
  return getToken(RFilter::PAR_OPEN, 0);
}

tree::TerminalNode* RFilter::ElemContext::PAR_CLOSE() {
  return getToken(RFilter::PAR_CLOSE, 0);
}

tree::TerminalNode* RFilter::ElemContext::BRA_OPEN() {
  return getToken(RFilter::BRA_OPEN, 0);
}

std::vector<tree::TerminalNode *> RFilter::ElemContext::BRA_CLOSE() {
  return getTokens(RFilter::BRA_CLOSE);
}

tree::TerminalNode* RFilter::ElemContext::BRA_CLOSE(size_t i) {
  return getToken(RFilter::BRA_CLOSE, i);
}

tree::TerminalNode* RFilter::ElemContext::DBRA_OPEN() {
  return getToken(RFilter::DBRA_OPEN, 0);
}

tree::TerminalNode* RFilter::ElemContext::FUNCTION() {
  return getToken(RFilter::FUNCTION, 0);
}

tree::TerminalNode* RFilter::ElemContext::FOR() {
  return getToken(RFilter::FOR, 0);
}

tree::TerminalNode* RFilter::ElemContext::WHILE() {
  return getToken(RFilter::WHILE, 0);
}

tree::TerminalNode* RFilter::ElemContext::IF() {
  return getToken(RFilter::IF, 0);
}

tree::TerminalNode* RFilter::ElemContext::ELSE() {
  return getToken(RFilter::ELSE, 0);
}


size_t RFilter::ElemContext::getRuleIndex() const {
  return RFilter::RuleElem;
}

void RFilter::ElemContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterElem(this);
}

void RFilter::ElemContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitElem(this);
}


antlrcpp::Any RFilter::ElemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitElem(this);
  else
    return visitor->visitChildren(this);
}

RFilter::ElemContext* RFilter::elem() {
  ElemContext *_localctx = _tracker.createInstance<ElemContext>(_ctx, getState());
  enterRule(_localctx, 4, RFilter::RuleElem);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(143);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case RFilter::REPEAT:
      case RFilter::IN:
      case RFilter::QMARK:
      case RFilter::PLUS:
      case RFilter::MINUS:
      case RFilter::ASTERICK:
      case RFilter::SLASH:
      case RFilter::LT:
      case RFilter::LTE:
      case RFilter::GT:
      case RFilter::GTE:
      case RFilter::EQUAL_OP:
      case RFilter::NOTEQUAL_OP:
      case RFilter::NOT:
      case RFilter::BAR:
      case RFilter::DBAR:
      case RFilter::AMP:
      case RFilter::DAMP:
      case RFilter::ASSIGN:
      case RFilter::RASSIGN:
      case RFilter::EQ_CHAR:
      case RFilter::TRIPLE_COLON:
      case RFilter::DOUBLE_COLON:
      case RFilter::COLON:
      case RFilter::DOLLAR_CHAR:
      case RFilter::AT_CHAR:
      case RFilter::TILDE:
      case RFilter::TRIPLE_DOT:
      case RFilter::COMMA:
      case RFilter::CARAT:
      case RFilter::USER_OP: {
        enterOuterAlt(_localctx, 1);
        setState(28);
        op();
        setState(30);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
        case 1: {
          setState(29);
          eat();
          break;
        }

        }
        break;
      }

      case RFilter::HEX:
      case RFilter::INT:
      case RFilter::FLOAT:
      case RFilter::COMPLEX:
      case RFilter::STRING:
      case RFilter::ID:
      case RFilter::BREAK:
      case RFilter::NEXT:
      case RFilter::NULLT:
      case RFilter::NaN:
      case RFilter::TRUE:
      case RFilter::FALSE:
      case RFilter::NA:
      case RFilter::INF: {
        enterOuterAlt(_localctx, 2);
        setState(32);
        atom();
        break;
      }

      case RFilter::COMMENT: {
        enterOuterAlt(_localctx, 3);
        setState(33);
        comment();
        break;
      }

      case RFilter::BRC_OPEN: {
        enterOuterAlt(_localctx, 4);
        setState(34);
        match(RFilter::BRC_OPEN);
        setState(36);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          setState(35);
          eat();
          break;
        }

        }
        curlies += 1;
        setState(44);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::SEMI)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(42);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(39);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(40);
              match(RFilter::NL);
              break;
            }

            case RFilter::SEMI: {
              setState(41);
              match(RFilter::SEMI);
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(46);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        curlies -= 1;
        setState(48);
        match(RFilter::BRC_CLOSE);
        break;
      }

      case RFilter::PAR_OPEN: {
        enterOuterAlt(_localctx, 5);
        setState(49);
        match(RFilter::PAR_OPEN);
        setState(54);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(52);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(50);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(51);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(56);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(57);
        match(RFilter::PAR_CLOSE);
        break;
      }

      case RFilter::BRA_OPEN: {
        enterOuterAlt(_localctx, 6);
        setState(58);
        match(RFilter::BRA_OPEN);
        setState(63);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(61);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(59);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(60);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(65);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(66);
        match(RFilter::BRA_CLOSE);
        break;
      }

      case RFilter::DBRA_OPEN: {
        enterOuterAlt(_localctx, 7);
        setState(67);
        match(RFilter::DBRA_OPEN);
        setState(72);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(70);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(68);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(69);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(74);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(75);
        match(RFilter::BRA_CLOSE);
        setState(76);
        match(RFilter::BRA_CLOSE);
        break;
      }

      case RFilter::FUNCTION: {
        enterOuterAlt(_localctx, 8);
        setState(77);
        match(RFilter::FUNCTION);
        setState(79);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == RFilter::NL) {
          setState(78);
          eat();
        }
        setState(81);
        match(RFilter::PAR_OPEN);
        setState(86);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(84);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(82);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(83);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(88);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(89);
        match(RFilter::PAR_CLOSE);
        setState(91);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx)) {
        case 1: {
          setState(90);
          eat();
          break;
        }

        }
        break;
      }

      case RFilter::FOR: {
        enterOuterAlt(_localctx, 9);
        setState(93);
        match(RFilter::FOR);
        setState(95);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == RFilter::NL) {
          setState(94);
          eat();
        }
        setState(97);
        match(RFilter::PAR_OPEN);
        setState(102);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(100);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(98);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(99);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(104);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(105);
        match(RFilter::PAR_CLOSE);
        setState(107);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 20, _ctx)) {
        case 1: {
          setState(106);
          eat();
          break;
        }

        }
        break;
      }

      case RFilter::WHILE: {
        enterOuterAlt(_localctx, 10);
        setState(109);
        match(RFilter::WHILE);
        setState(111);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == RFilter::NL) {
          setState(110);
          eat();
        }
        setState(113);
        match(RFilter::PAR_OPEN);
        setState(118);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(116);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(114);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(115);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(120);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(121);
        match(RFilter::PAR_CLOSE);
        setState(123);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
        case 1: {
          setState(122);
          eat();
          break;
        }

        }
        break;
      }

      case RFilter::IF: {
        enterOuterAlt(_localctx, 11);
        setState(125);
        match(RFilter::IF);
        setState(127);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == RFilter::NL) {
          setState(126);
          eat();
        }
        setState(129);
        match(RFilter::PAR_OPEN);
        setState(134);
        _errHandler->sync(this);
        _la = _input->LA(1);
        while ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << RFilter::HEX)
          | (1ULL << RFilter::INT)
          | (1ULL << RFilter::FLOAT)
          | (1ULL << RFilter::COMPLEX)
          | (1ULL << RFilter::STRING)
          | (1ULL << RFilter::ID)
          | (1ULL << RFilter::IF)
          | (1ULL << RFilter::ELSE)
          | (1ULL << RFilter::REPEAT)
          | (1ULL << RFilter::WHILE)
          | (1ULL << RFilter::FOR)
          | (1ULL << RFilter::IN)
          | (1ULL << RFilter::BREAK)
          | (1ULL << RFilter::NEXT)
          | (1ULL << RFilter::FUNCTION)
          | (1ULL << RFilter::PAR_OPEN)
          | (1ULL << RFilter::BRA_OPEN)
          | (1ULL << RFilter::DBRA_OPEN)
          | (1ULL << RFilter::BRC_OPEN)
          | (1ULL << RFilter::QMARK)
          | (1ULL << RFilter::PLUS)
          | (1ULL << RFilter::MINUS)
          | (1ULL << RFilter::ASTERICK)
          | (1ULL << RFilter::SLASH)
          | (1ULL << RFilter::LT)
          | (1ULL << RFilter::LTE)
          | (1ULL << RFilter::GT)
          | (1ULL << RFilter::GTE)
          | (1ULL << RFilter::EQUAL_OP)
          | (1ULL << RFilter::NOTEQUAL_OP)
          | (1ULL << RFilter::NOT)
          | (1ULL << RFilter::BAR)
          | (1ULL << RFilter::DBAR)
          | (1ULL << RFilter::AMP)
          | (1ULL << RFilter::DAMP)
          | (1ULL << RFilter::ASSIGN)
          | (1ULL << RFilter::RASSIGN)
          | (1ULL << RFilter::EQ_CHAR)
          | (1ULL << RFilter::TRIPLE_COLON)
          | (1ULL << RFilter::DOUBLE_COLON)
          | (1ULL << RFilter::COLON)
          | (1ULL << RFilter::DOLLAR_CHAR)
          | (1ULL << RFilter::AT_CHAR)
          | (1ULL << RFilter::TILDE)
          | (1ULL << RFilter::TRIPLE_DOT)
          | (1ULL << RFilter::COMMA)
          | (1ULL << RFilter::CARAT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
          ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
          | (1ULL << (RFilter::NaN - 64))
          | (1ULL << (RFilter::TRUE - 64))
          | (1ULL << (RFilter::FALSE - 64))
          | (1ULL << (RFilter::NA - 64))
          | (1ULL << (RFilter::INF - 64))
          | (1ULL << (RFilter::USER_OP - 64))
          | (1ULL << (RFilter::COMMENT - 64))
          | (1ULL << (RFilter::NL - 64)))) != 0)) {
          setState(132);
          _errHandler->sync(this);
          switch (_input->LA(1)) {
            case RFilter::HEX:
            case RFilter::INT:
            case RFilter::FLOAT:
            case RFilter::COMPLEX:
            case RFilter::STRING:
            case RFilter::ID:
            case RFilter::IF:
            case RFilter::ELSE:
            case RFilter::REPEAT:
            case RFilter::WHILE:
            case RFilter::FOR:
            case RFilter::IN:
            case RFilter::BREAK:
            case RFilter::NEXT:
            case RFilter::FUNCTION:
            case RFilter::PAR_OPEN:
            case RFilter::BRA_OPEN:
            case RFilter::DBRA_OPEN:
            case RFilter::BRC_OPEN:
            case RFilter::QMARK:
            case RFilter::PLUS:
            case RFilter::MINUS:
            case RFilter::ASTERICK:
            case RFilter::SLASH:
            case RFilter::LT:
            case RFilter::LTE:
            case RFilter::GT:
            case RFilter::GTE:
            case RFilter::EQUAL_OP:
            case RFilter::NOTEQUAL_OP:
            case RFilter::NOT:
            case RFilter::BAR:
            case RFilter::DBAR:
            case RFilter::AMP:
            case RFilter::DAMP:
            case RFilter::ASSIGN:
            case RFilter::RASSIGN:
            case RFilter::EQ_CHAR:
            case RFilter::TRIPLE_COLON:
            case RFilter::DOUBLE_COLON:
            case RFilter::COLON:
            case RFilter::DOLLAR_CHAR:
            case RFilter::AT_CHAR:
            case RFilter::TILDE:
            case RFilter::TRIPLE_DOT:
            case RFilter::COMMA:
            case RFilter::CARAT:
            case RFilter::NULLT:
            case RFilter::NaN:
            case RFilter::TRUE:
            case RFilter::FALSE:
            case RFilter::NA:
            case RFilter::INF:
            case RFilter::USER_OP:
            case RFilter::COMMENT: {
              setState(130);
              elem();
              break;
            }

            case RFilter::NL: {
              setState(131);
              eat();
              break;
            }

          default:
            throw NoViableAltException(this);
          }
          setState(136);
          _errHandler->sync(this);
          _la = _input->LA(1);
        }
        setState(137);
        match(RFilter::PAR_CLOSE);
        setState(139);
        _errHandler->sync(this);

        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 28, _ctx)) {
        case 1: {
          setState(138);
          eat();
          break;
        }

        }
        break;
      }

      case RFilter::ELSE: {
        enterOuterAlt(_localctx, 12);
        setState(141);
        match(RFilter::ELSE);

                // ``inside a compound expression, a newline before else is discarded,
                // whereas at the outermost level, the newline terminates the if
                // construction and a subsequent else causes a syntax error.''
                /*
                Works here
                    if (1==0) { print(1) } else { print(2) }

                and correctly gets error here:

                    if (1==0) { print(1) }
                    else { print(2) }

                this works too:

                    if (1==0) {
                      if (2==0) print(1)
                      else print(2)
                    }
                */
        		auto tok = dynamic_cast<WritableToken*>(_input->LT(-2));
        		if (curlies > 0 && tok != nullptr && tok->getType() == NL)
        			tok->setChannel(Token::HIDDEN_CHANNEL);
                
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

//----------------- CommentContext ------------------------------------------------------------------

RFilter::CommentContext::CommentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RFilter::CommentContext::COMMENT() {
  return getToken(RFilter::COMMENT, 0);
}


size_t RFilter::CommentContext::getRuleIndex() const {
  return RFilter::RuleComment;
}

void RFilter::CommentContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComment(this);
}

void RFilter::CommentContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComment(this);
}


antlrcpp::Any RFilter::CommentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitComment(this);
  else
    return visitor->visitChildren(this);
}

RFilter::CommentContext* RFilter::comment() {
  CommentContext *_localctx = _tracker.createInstance<CommentContext>(_ctx, getState());
  enterRule(_localctx, 6, RFilter::RuleComment);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(145);
    match(RFilter::COMMENT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

RFilter::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RFilter::AtomContext::NEXT() {
  return getToken(RFilter::NEXT, 0);
}

tree::TerminalNode* RFilter::AtomContext::BREAK() {
  return getToken(RFilter::BREAK, 0);
}

tree::TerminalNode* RFilter::AtomContext::ID() {
  return getToken(RFilter::ID, 0);
}

tree::TerminalNode* RFilter::AtomContext::STRING() {
  return getToken(RFilter::STRING, 0);
}

tree::TerminalNode* RFilter::AtomContext::HEX() {
  return getToken(RFilter::HEX, 0);
}

tree::TerminalNode* RFilter::AtomContext::INT() {
  return getToken(RFilter::INT, 0);
}

tree::TerminalNode* RFilter::AtomContext::FLOAT() {
  return getToken(RFilter::FLOAT, 0);
}

tree::TerminalNode* RFilter::AtomContext::COMPLEX() {
  return getToken(RFilter::COMPLEX, 0);
}

tree::TerminalNode* RFilter::AtomContext::NULLT() {
  return getToken(RFilter::NULLT, 0);
}

tree::TerminalNode* RFilter::AtomContext::NA() {
  return getToken(RFilter::NA, 0);
}

tree::TerminalNode* RFilter::AtomContext::INF() {
  return getToken(RFilter::INF, 0);
}

tree::TerminalNode* RFilter::AtomContext::NaN() {
  return getToken(RFilter::NaN, 0);
}

tree::TerminalNode* RFilter::AtomContext::TRUE() {
  return getToken(RFilter::TRUE, 0);
}

tree::TerminalNode* RFilter::AtomContext::FALSE() {
  return getToken(RFilter::FALSE, 0);
}


size_t RFilter::AtomContext::getRuleIndex() const {
  return RFilter::RuleAtom;
}

void RFilter::AtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom(this);
}

void RFilter::AtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom(this);
}


antlrcpp::Any RFilter::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

RFilter::AtomContext* RFilter::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 8, RFilter::RuleAtom);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(147);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << RFilter::HEX)
      | (1ULL << RFilter::INT)
      | (1ULL << RFilter::FLOAT)
      | (1ULL << RFilter::COMPLEX)
      | (1ULL << RFilter::STRING)
      | (1ULL << RFilter::ID)
      | (1ULL << RFilter::BREAK)
      | (1ULL << RFilter::NEXT))) != 0) || ((((_la - 64) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 64)) & ((1ULL << (RFilter::NULLT - 64))
      | (1ULL << (RFilter::NaN - 64))
      | (1ULL << (RFilter::TRUE - 64))
      | (1ULL << (RFilter::FALSE - 64))
      | (1ULL << (RFilter::NA - 64))
      | (1ULL << (RFilter::INF - 64)))) != 0))) {
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

//----------------- OpContext ------------------------------------------------------------------

RFilter::OpContext::OpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* RFilter::OpContext::PLUS() {
  return getToken(RFilter::PLUS, 0);
}

tree::TerminalNode* RFilter::OpContext::MINUS() {
  return getToken(RFilter::MINUS, 0);
}

tree::TerminalNode* RFilter::OpContext::ASTERICK() {
  return getToken(RFilter::ASTERICK, 0);
}

tree::TerminalNode* RFilter::OpContext::SLASH() {
  return getToken(RFilter::SLASH, 0);
}

tree::TerminalNode* RFilter::OpContext::CARAT() {
  return getToken(RFilter::CARAT, 0);
}

tree::TerminalNode* RFilter::OpContext::LT() {
  return getToken(RFilter::LT, 0);
}

tree::TerminalNode* RFilter::OpContext::LTE() {
  return getToken(RFilter::LTE, 0);
}

tree::TerminalNode* RFilter::OpContext::GTE() {
  return getToken(RFilter::GTE, 0);
}

tree::TerminalNode* RFilter::OpContext::GT() {
  return getToken(RFilter::GT, 0);
}

tree::TerminalNode* RFilter::OpContext::EQUAL_OP() {
  return getToken(RFilter::EQUAL_OP, 0);
}

tree::TerminalNode* RFilter::OpContext::NOTEQUAL_OP() {
  return getToken(RFilter::NOTEQUAL_OP, 0);
}

tree::TerminalNode* RFilter::OpContext::AMP() {
  return getToken(RFilter::AMP, 0);
}

tree::TerminalNode* RFilter::OpContext::DAMP() {
  return getToken(RFilter::DAMP, 0);
}

tree::TerminalNode* RFilter::OpContext::USER_OP() {
  return getToken(RFilter::USER_OP, 0);
}

tree::TerminalNode* RFilter::OpContext::REPEAT() {
  return getToken(RFilter::REPEAT, 0);
}

tree::TerminalNode* RFilter::OpContext::IN() {
  return getToken(RFilter::IN, 0);
}

tree::TerminalNode* RFilter::OpContext::QMARK() {
  return getToken(RFilter::QMARK, 0);
}

tree::TerminalNode* RFilter::OpContext::NOT() {
  return getToken(RFilter::NOT, 0);
}

tree::TerminalNode* RFilter::OpContext::EQ_CHAR() {
  return getToken(RFilter::EQ_CHAR, 0);
}

tree::TerminalNode* RFilter::OpContext::COLON() {
  return getToken(RFilter::COLON, 0);
}

tree::TerminalNode* RFilter::OpContext::TILDE() {
  return getToken(RFilter::TILDE, 0);
}

tree::TerminalNode* RFilter::OpContext::DOLLAR_CHAR() {
  return getToken(RFilter::DOLLAR_CHAR, 0);
}

tree::TerminalNode* RFilter::OpContext::AT_CHAR() {
  return getToken(RFilter::AT_CHAR, 0);
}

tree::TerminalNode* RFilter::OpContext::ASSIGN() {
  return getToken(RFilter::ASSIGN, 0);
}

tree::TerminalNode* RFilter::OpContext::RASSIGN() {
  return getToken(RFilter::RASSIGN, 0);
}

tree::TerminalNode* RFilter::OpContext::DOUBLE_COLON() {
  return getToken(RFilter::DOUBLE_COLON, 0);
}

tree::TerminalNode* RFilter::OpContext::TRIPLE_COLON() {
  return getToken(RFilter::TRIPLE_COLON, 0);
}

tree::TerminalNode* RFilter::OpContext::COMMA() {
  return getToken(RFilter::COMMA, 0);
}

tree::TerminalNode* RFilter::OpContext::TRIPLE_DOT() {
  return getToken(RFilter::TRIPLE_DOT, 0);
}

tree::TerminalNode* RFilter::OpContext::DBAR() {
  return getToken(RFilter::DBAR, 0);
}

tree::TerminalNode* RFilter::OpContext::BAR() {
  return getToken(RFilter::BAR, 0);
}


size_t RFilter::OpContext::getRuleIndex() const {
  return RFilter::RuleOp;
}

void RFilter::OpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOp(this);
}

void RFilter::OpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<RFilterListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOp(this);
}


antlrcpp::Any RFilter::OpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<RFilterVisitor*>(visitor))
    return parserVisitor->visitOp(this);
  else
    return visitor->visitChildren(this);
}

RFilter::OpContext* RFilter::op() {
  OpContext *_localctx = _tracker.createInstance<OpContext>(_ctx, getState());
  enterRule(_localctx, 10, RFilter::RuleOp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(149);
    _la = _input->LA(1);
    if (!(((((_la - 9) & ~ 0x3fULL) == 0) &&
      ((1ULL << (_la - 9)) & ((1ULL << (RFilter::REPEAT - 9))
      | (1ULL << (RFilter::IN - 9))
      | (1ULL << (RFilter::QMARK - 9))
      | (1ULL << (RFilter::PLUS - 9))
      | (1ULL << (RFilter::MINUS - 9))
      | (1ULL << (RFilter::ASTERICK - 9))
      | (1ULL << (RFilter::SLASH - 9))
      | (1ULL << (RFilter::LT - 9))
      | (1ULL << (RFilter::LTE - 9))
      | (1ULL << (RFilter::GT - 9))
      | (1ULL << (RFilter::GTE - 9))
      | (1ULL << (RFilter::EQUAL_OP - 9))
      | (1ULL << (RFilter::NOTEQUAL_OP - 9))
      | (1ULL << (RFilter::NOT - 9))
      | (1ULL << (RFilter::BAR - 9))
      | (1ULL << (RFilter::DBAR - 9))
      | (1ULL << (RFilter::AMP - 9))
      | (1ULL << (RFilter::DAMP - 9))
      | (1ULL << (RFilter::ASSIGN - 9))
      | (1ULL << (RFilter::RASSIGN - 9))
      | (1ULL << (RFilter::EQ_CHAR - 9))
      | (1ULL << (RFilter::TRIPLE_COLON - 9))
      | (1ULL << (RFilter::DOUBLE_COLON - 9))
      | (1ULL << (RFilter::COLON - 9))
      | (1ULL << (RFilter::DOLLAR_CHAR - 9))
      | (1ULL << (RFilter::AT_CHAR - 9))
      | (1ULL << (RFilter::TILDE - 9))
      | (1ULL << (RFilter::TRIPLE_DOT - 9))
      | (1ULL << (RFilter::COMMA - 9))
      | (1ULL << (RFilter::CARAT - 9))
      | (1ULL << (RFilter::USER_OP - 9)))) != 0))) {
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

// Static vars and initialization.
std::vector<dfa::DFA> RFilter::_decisionToDFA;
atn::PredictionContextCache RFilter::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN RFilter::_atn;
std::vector<uint16_t> RFilter::_serializedATN;

std::vector<std::string> RFilter::_ruleNames = {
  "stream", "eat", "elem", "comment", "atom", "op"
};

std::vector<std::string> RFilter::_literalNames = {
  "", "", "", "", "", "", "", "'if'", "'else'", "'repeat'", "'while'", "'for'", 
  "'in'", "'break'", "'next'", "'function'", "", "'('", "')'", "'['", "']'", 
  "'[['", "']]'", "'{'", "'}'", "'?'", "'+'", "'-'", "'*'", "'/'", "'<'", 
  "'<='", "'>'", "'>='", "'=='", "'!='", "'!'", "'%'", "'#'", "'|'", "'||'", 
  "'&'", "'&&'", "':='", "'<-'", "'<<-'", "'->'", "'->>'", "'='", "':::'", 
  "'::'", "':'", "'$'", "'@'", "'~'", "'_'", "'.'", "'''", "'\"'", "'...'", 
  "','", "'`'", "';'", "'^'", "'NULL'", "'NaN'", "'TRUE'", "'FALSE'", "'NA'", 
  "'Inf'"
};

std::vector<std::string> RFilter::_symbolicNames = {
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

dfa::Vocabulary RFilter::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> RFilter::_tokenNames;

RFilter::Initializer::Initializer() {
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
    0x3, 0x4b, 0x9a, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x3, 
    0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x12, 0xa, 0x2, 0xc, 0x2, 0xe, 0x2, 
    0x15, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x6, 0x3, 0x1b, 
    0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x1c, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x21, 
    0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x27, 0xa, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x2d, 0xa, 0x4, 
    0xc, 0x4, 0xe, 0x4, 0x30, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x7, 0x4, 0x37, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x3a, 0xb, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x40, 0xa, 0x4, 
    0xc, 0x4, 0xe, 0x4, 0x43, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x7, 0x4, 0x49, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x4c, 0xb, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x52, 0xa, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x57, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x5a, 
    0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x5e, 0xa, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x5, 0x4, 0x62, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 
    0x67, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x6a, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x5, 0x4, 0x6e, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x72, 0xa, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x77, 0xa, 0x4, 0xc, 0x4, 0xe, 
    0x4, 0x7a, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x7e, 0xa, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x5, 0x4, 0x82, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x7, 0x4, 0x87, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x8a, 0xb, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x5, 0x4, 0x8e, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x92, 
    0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 
    0x3, 0x7, 0x2, 0x2, 0x8, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0x2, 0x4, 0x5, 
    0x2, 0x3, 0x8, 0xf, 0x10, 0x42, 0x47, 0xc, 0x2, 0xb, 0xb, 0xe, 0xe, 
    0x1b, 0x26, 0x29, 0x2c, 0x2e, 0x2e, 0x30, 0x30, 0x32, 0x38, 0x3d, 0x3e, 
    0x41, 0x41, 0x48, 0x48, 0x2, 0xbd, 0x2, 0x13, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x1a, 0x3, 0x2, 0x2, 0x2, 0x6, 0x91, 0x3, 0x2, 0x2, 0x2, 0x8, 0x93, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x95, 0x3, 0x2, 0x2, 0x2, 0xc, 0x97, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x12, 0x5, 0x6, 0x4, 0x2, 0xf, 0x12, 0x7, 0x4a, 0x2, 
    0x2, 0x10, 0x12, 0x7, 0x40, 0x2, 0x2, 0x11, 0xe, 0x3, 0x2, 0x2, 0x2, 
    0x11, 0xf, 0x3, 0x2, 0x2, 0x2, 0x11, 0x10, 0x3, 0x2, 0x2, 0x2, 0x12, 
    0x15, 0x3, 0x2, 0x2, 0x2, 0x13, 0x11, 0x3, 0x2, 0x2, 0x2, 0x13, 0x14, 
    0x3, 0x2, 0x2, 0x2, 0x14, 0x16, 0x3, 0x2, 0x2, 0x2, 0x15, 0x13, 0x3, 
    0x2, 0x2, 0x2, 0x16, 0x17, 0x7, 0x2, 0x2, 0x3, 0x17, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x18, 0x19, 0x7, 0x4a, 0x2, 0x2, 0x19, 0x1b, 0x8, 0x3, 0x1, 
    0x2, 0x1a, 0x18, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x1c, 0x3, 0x2, 0x2, 0x2, 
    0x1c, 0x1a, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x1d, 
    0x5, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x20, 0x5, 0xc, 0x7, 0x2, 0x1f, 0x21, 
    0x5, 0x4, 0x3, 0x2, 0x20, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x3, 
    0x2, 0x2, 0x2, 0x21, 0x92, 0x3, 0x2, 0x2, 0x2, 0x22, 0x92, 0x5, 0xa, 
    0x6, 0x2, 0x23, 0x92, 0x5, 0x8, 0x5, 0x2, 0x24, 0x26, 0x7, 0x19, 0x2, 
    0x2, 0x25, 0x27, 0x5, 0x4, 0x3, 0x2, 0x26, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x26, 0x27, 0x3, 0x2, 0x2, 0x2, 0x27, 0x28, 0x3, 0x2, 0x2, 0x2, 0x28, 
    0x2e, 0x8, 0x4, 0x1, 0x2, 0x29, 0x2d, 0x5, 0x6, 0x4, 0x2, 0x2a, 0x2d, 
    0x7, 0x4a, 0x2, 0x2, 0x2b, 0x2d, 0x7, 0x40, 0x2, 0x2, 0x2c, 0x29, 0x3, 
    0x2, 0x2, 0x2, 0x2c, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x2b, 0x3, 0x2, 
    0x2, 0x2, 0x2d, 0x30, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x2c, 0x3, 0x2, 0x2, 
    0x2, 0x2e, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x2f, 0x31, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x31, 0x32, 0x8, 0x4, 0x1, 0x2, 0x32, 
    0x92, 0x7, 0x1a, 0x2, 0x2, 0x33, 0x38, 0x7, 0x13, 0x2, 0x2, 0x34, 0x37, 
    0x5, 0x6, 0x4, 0x2, 0x35, 0x37, 0x5, 0x4, 0x3, 0x2, 0x36, 0x34, 0x3, 
    0x2, 0x2, 0x2, 0x36, 0x35, 0x3, 0x2, 0x2, 0x2, 0x37, 0x3a, 0x3, 0x2, 
    0x2, 0x2, 0x38, 0x36, 0x3, 0x2, 0x2, 0x2, 0x38, 0x39, 0x3, 0x2, 0x2, 
    0x2, 0x39, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x38, 0x3, 0x2, 0x2, 0x2, 
    0x3b, 0x92, 0x7, 0x14, 0x2, 0x2, 0x3c, 0x41, 0x7, 0x15, 0x2, 0x2, 0x3d, 
    0x40, 0x5, 0x6, 0x4, 0x2, 0x3e, 0x40, 0x5, 0x4, 0x3, 0x2, 0x3f, 0x3d, 
    0x3, 0x2, 0x2, 0x2, 0x3f, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x40, 0x43, 0x3, 
    0x2, 0x2, 0x2, 0x41, 0x3f, 0x3, 0x2, 0x2, 0x2, 0x41, 0x42, 0x3, 0x2, 
    0x2, 0x2, 0x42, 0x44, 0x3, 0x2, 0x2, 0x2, 0x43, 0x41, 0x3, 0x2, 0x2, 
    0x2, 0x44, 0x92, 0x7, 0x16, 0x2, 0x2, 0x45, 0x4a, 0x7, 0x17, 0x2, 0x2, 
    0x46, 0x49, 0x5, 0x6, 0x4, 0x2, 0x47, 0x49, 0x5, 0x4, 0x3, 0x2, 0x48, 
    0x46, 0x3, 0x2, 0x2, 0x2, 0x48, 0x47, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4c, 
    0x3, 0x2, 0x2, 0x2, 0x4a, 0x48, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x3, 
    0x2, 0x2, 0x2, 0x4b, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4a, 0x3, 0x2, 
    0x2, 0x2, 0x4d, 0x4e, 0x7, 0x16, 0x2, 0x2, 0x4e, 0x92, 0x7, 0x16, 0x2, 
    0x2, 0x4f, 0x51, 0x7, 0x11, 0x2, 0x2, 0x50, 0x52, 0x5, 0x4, 0x3, 0x2, 
    0x51, 0x50, 0x3, 0x2, 0x2, 0x2, 0x51, 0x52, 0x3, 0x2, 0x2, 0x2, 0x52, 
    0x53, 0x3, 0x2, 0x2, 0x2, 0x53, 0x58, 0x7, 0x13, 0x2, 0x2, 0x54, 0x57, 
    0x5, 0x6, 0x4, 0x2, 0x55, 0x57, 0x5, 0x4, 0x3, 0x2, 0x56, 0x54, 0x3, 
    0x2, 0x2, 0x2, 0x56, 0x55, 0x3, 0x2, 0x2, 0x2, 0x57, 0x5a, 0x3, 0x2, 
    0x2, 0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x3, 0x2, 0x2, 
    0x2, 0x59, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x58, 0x3, 0x2, 0x2, 0x2, 
    0x5b, 0x5d, 0x7, 0x14, 0x2, 0x2, 0x5c, 0x5e, 0x5, 0x4, 0x3, 0x2, 0x5d, 
    0x5c, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x92, 
    0x3, 0x2, 0x2, 0x2, 0x5f, 0x61, 0x7, 0xd, 0x2, 0x2, 0x60, 0x62, 0x5, 
    0x4, 0x3, 0x2, 0x61, 0x60, 0x3, 0x2, 0x2, 0x2, 0x61, 0x62, 0x3, 0x2, 
    0x2, 0x2, 0x62, 0x63, 0x3, 0x2, 0x2, 0x2, 0x63, 0x68, 0x7, 0x13, 0x2, 
    0x2, 0x64, 0x67, 0x5, 0x6, 0x4, 0x2, 0x65, 0x67, 0x5, 0x4, 0x3, 0x2, 
    0x66, 0x64, 0x3, 0x2, 0x2, 0x2, 0x66, 0x65, 0x3, 0x2, 0x2, 0x2, 0x67, 
    0x6a, 0x3, 0x2, 0x2, 0x2, 0x68, 0x66, 0x3, 0x2, 0x2, 0x2, 0x68, 0x69, 
    0x3, 0x2, 0x2, 0x2, 0x69, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x68, 0x3, 
    0x2, 0x2, 0x2, 0x6b, 0x6d, 0x7, 0x14, 0x2, 0x2, 0x6c, 0x6e, 0x5, 0x4, 
    0x3, 0x2, 0x6d, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x6e, 0x3, 0x2, 0x2, 
    0x2, 0x6e, 0x92, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x71, 0x7, 0xc, 0x2, 0x2, 
    0x70, 0x72, 0x5, 0x4, 0x3, 0x2, 0x71, 0x70, 0x3, 0x2, 0x2, 0x2, 0x71, 
    0x72, 0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0x3, 0x2, 0x2, 0x2, 0x73, 0x78, 
    0x7, 0x13, 0x2, 0x2, 0x74, 0x77, 0x5, 0x6, 0x4, 0x2, 0x75, 0x77, 0x5, 
    0x4, 0x3, 0x2, 0x76, 0x74, 0x3, 0x2, 0x2, 0x2, 0x76, 0x75, 0x3, 0x2, 
    0x2, 0x2, 0x77, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x78, 0x76, 0x3, 0x2, 0x2, 
    0x2, 0x78, 0x79, 0x3, 0x2, 0x2, 0x2, 0x79, 0x7b, 0x3, 0x2, 0x2, 0x2, 
    0x7a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x7d, 0x7, 0x14, 0x2, 0x2, 0x7c, 
    0x7e, 0x5, 0x4, 0x3, 0x2, 0x7d, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 
    0x3, 0x2, 0x2, 0x2, 0x7e, 0x92, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x81, 0x7, 
    0x9, 0x2, 0x2, 0x80, 0x82, 0x5, 0x4, 0x3, 0x2, 0x81, 0x80, 0x3, 0x2, 
    0x2, 0x2, 0x81, 0x82, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x3, 0x2, 0x2, 
    0x2, 0x83, 0x88, 0x7, 0x13, 0x2, 0x2, 0x84, 0x87, 0x5, 0x6, 0x4, 0x2, 
    0x85, 0x87, 0x5, 0x4, 0x3, 0x2, 0x86, 0x84, 0x3, 0x2, 0x2, 0x2, 0x86, 
    0x85, 0x3, 0x2, 0x2, 0x2, 0x87, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x88, 0x86, 
    0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8b, 0x3, 
    0x2, 0x2, 0x2, 0x8a, 0x88, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8d, 0x7, 0x14, 
    0x2, 0x2, 0x8c, 0x8e, 0x5, 0x4, 0x3, 0x2, 0x8d, 0x8c, 0x3, 0x2, 0x2, 
    0x2, 0x8d, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x92, 0x3, 0x2, 0x2, 0x2, 
    0x8f, 0x90, 0x7, 0xa, 0x2, 0x2, 0x90, 0x92, 0x8, 0x4, 0x1, 0x2, 0x91, 
    0x1e, 0x3, 0x2, 0x2, 0x2, 0x91, 0x22, 0x3, 0x2, 0x2, 0x2, 0x91, 0x23, 
    0x3, 0x2, 0x2, 0x2, 0x91, 0x24, 0x3, 0x2, 0x2, 0x2, 0x91, 0x33, 0x3, 
    0x2, 0x2, 0x2, 0x91, 0x3c, 0x3, 0x2, 0x2, 0x2, 0x91, 0x45, 0x3, 0x2, 
    0x2, 0x2, 0x91, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x91, 0x5f, 0x3, 0x2, 0x2, 
    0x2, 0x91, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x91, 0x7f, 0x3, 0x2, 0x2, 0x2, 
    0x91, 0x8f, 0x3, 0x2, 0x2, 0x2, 0x92, 0x7, 0x3, 0x2, 0x2, 0x2, 0x93, 
    0x94, 0x7, 0x49, 0x2, 0x2, 0x94, 0x9, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 
    0x9, 0x2, 0x2, 0x2, 0x96, 0xb, 0x3, 0x2, 0x2, 0x2, 0x97, 0x98, 0x9, 
    0x3, 0x2, 0x2, 0x98, 0xd, 0x3, 0x2, 0x2, 0x2, 0x20, 0x11, 0x13, 0x1c, 
    0x20, 0x26, 0x2c, 0x2e, 0x36, 0x38, 0x3f, 0x41, 0x48, 0x4a, 0x51, 0x56, 
    0x58, 0x5d, 0x61, 0x66, 0x68, 0x6d, 0x71, 0x76, 0x78, 0x7d, 0x81, 0x86, 
    0x88, 0x8d, 0x91, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

RFilter::Initializer RFilter::_init;
