
// Generated from parser/RFilter.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  RFilter : public antlr4::Parser {
public:
  enum {
    HEX = 1, INT = 2, FLOAT = 3, COMPLEX = 4, STRING = 5, ID = 6, IF = 7, 
    ELSE = 8, REPEAT = 9, WHILE = 10, FOR = 11, IN = 12, BREAK = 13, NEXT = 14, 
    FUNCTION = 15, AND_OP = 16, PAR_OPEN = 17, PAR_CLOSE = 18, BRA_OPEN = 19, 
    BRA_CLOSE = 20, DBRA_OPEN = 21, DBRA_CLOSE = 22, BRC_OPEN = 23, BRC_CLOSE = 24, 
    QMARK = 25, PLUS = 26, MINUS = 27, ASTERICK = 28, SLASH = 29, LT = 30, 
    LTE = 31, GT = 32, GTE = 33, EQUAL_OP = 34, NOTEQUAL_OP = 35, NOT = 36, 
    PERCENT = 37, HASH = 38, BAR = 39, DBAR = 40, AMP = 41, DAMP = 42, COLEQ = 43, 
    ASSIGN = 44, DASSIGN = 45, RASSIGN = 46, DRASSIGN = 47, EQ_CHAR = 48, 
    TRIPLE_COLON = 49, DOUBLE_COLON = 50, COLON = 51, DOLLAR_CHAR = 52, 
    AT_CHAR = 53, TILDE = 54, USCORE = 55, PERIOD = 56, SQUOTE = 57, DQUOTE = 58, 
    TRIPLE_DOT = 59, COMMA = 60, BACKTICK = 61, SEMI = 62, CARAT = 63, NULLT = 64, 
    NaN = 65, TRUE = 66, FALSE = 67, NA = 68, INF = 69, USER_OP = 70, COMMENT = 71, 
    NL = 72, WS = 73
  };

  enum {
    RuleStream = 0, RuleEat = 1, RuleElem = 2, RuleComment = 3, RuleAtom = 4, 
    RuleOp = 5
  };

  RFilter(antlr4::TokenStream *input);
  ~RFilter();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  int curlies = 0;


  class StreamContext;
  class EatContext;
  class ElemContext;
  class CommentContext;
  class AtomContext;
  class OpContext; 

  class  StreamContext : public antlr4::ParserRuleContext {
  public:
    StreamContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<ElemContext *> elem();
    ElemContext* elem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NL();
    antlr4::tree::TerminalNode* NL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StreamContext* stream();

  class  EatContext : public antlr4::ParserRuleContext {
  public:
    antlr4::Token *nlToken = nullptr;;
    EatContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NL();
    antlr4::tree::TerminalNode* NL(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  EatContext* eat();

  class  ElemContext : public antlr4::ParserRuleContext {
  public:
    ElemContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    OpContext *op();
    std::vector<EatContext *> eat();
    EatContext* eat(size_t i);
    AtomContext *atom();
    CommentContext *comment();
    antlr4::tree::TerminalNode *BRC_OPEN();
    antlr4::tree::TerminalNode *BRC_CLOSE();
    std::vector<ElemContext *> elem();
    ElemContext* elem(size_t i);
    std::vector<antlr4::tree::TerminalNode *> NL();
    antlr4::tree::TerminalNode* NL(size_t i);
    std::vector<antlr4::tree::TerminalNode *> SEMI();
    antlr4::tree::TerminalNode* SEMI(size_t i);
    antlr4::tree::TerminalNode *PAR_OPEN();
    antlr4::tree::TerminalNode *PAR_CLOSE();
    antlr4::tree::TerminalNode *BRA_OPEN();
    std::vector<antlr4::tree::TerminalNode *> BRA_CLOSE();
    antlr4::tree::TerminalNode* BRA_CLOSE(size_t i);
    antlr4::tree::TerminalNode *DBRA_OPEN();
    antlr4::tree::TerminalNode *FUNCTION();
    antlr4::tree::TerminalNode *FOR();
    antlr4::tree::TerminalNode *WHILE();
    antlr4::tree::TerminalNode *IF();
    antlr4::tree::TerminalNode *ELSE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ElemContext* elem();

  class  CommentContext : public antlr4::ParserRuleContext {
  public:
    CommentContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *COMMENT();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CommentContext* comment();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NEXT();
    antlr4::tree::TerminalNode *BREAK();
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *STRING();
    antlr4::tree::TerminalNode *HEX();
    antlr4::tree::TerminalNode *INT();
    antlr4::tree::TerminalNode *FLOAT();
    antlr4::tree::TerminalNode *COMPLEX();
    antlr4::tree::TerminalNode *NULLT();
    antlr4::tree::TerminalNode *NA();
    antlr4::tree::TerminalNode *INF();
    antlr4::tree::TerminalNode *NaN();
    antlr4::tree::TerminalNode *TRUE();
    antlr4::tree::TerminalNode *FALSE();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AtomContext* atom();

  class  OpContext : public antlr4::ParserRuleContext {
  public:
    OpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *PLUS();
    antlr4::tree::TerminalNode *MINUS();
    antlr4::tree::TerminalNode *ASTERICK();
    antlr4::tree::TerminalNode *SLASH();
    antlr4::tree::TerminalNode *CARAT();
    antlr4::tree::TerminalNode *LT();
    antlr4::tree::TerminalNode *LTE();
    antlr4::tree::TerminalNode *GTE();
    antlr4::tree::TerminalNode *GT();
    antlr4::tree::TerminalNode *EQUAL_OP();
    antlr4::tree::TerminalNode *NOTEQUAL_OP();
    antlr4::tree::TerminalNode *AMP();
    antlr4::tree::TerminalNode *DAMP();
    antlr4::tree::TerminalNode *USER_OP();
    antlr4::tree::TerminalNode *REPEAT();
    antlr4::tree::TerminalNode *IN();
    antlr4::tree::TerminalNode *QMARK();
    antlr4::tree::TerminalNode *NOT();
    antlr4::tree::TerminalNode *EQ_CHAR();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *TILDE();
    antlr4::tree::TerminalNode *DOLLAR_CHAR();
    antlr4::tree::TerminalNode *AT_CHAR();
    antlr4::tree::TerminalNode *ASSIGN();
    antlr4::tree::TerminalNode *RASSIGN();
    antlr4::tree::TerminalNode *DOUBLE_COLON();
    antlr4::tree::TerminalNode *TRIPLE_COLON();
    antlr4::tree::TerminalNode *COMMA();
    antlr4::tree::TerminalNode *TRIPLE_DOT();
    antlr4::tree::TerminalNode *DBAR();
    antlr4::tree::TerminalNode *BAR();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpContext* op();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

