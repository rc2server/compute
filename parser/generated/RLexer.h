
// Generated from Rc2Parser/R.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  RLexer : public antlr4::Lexer {
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

  RLexer(antlr4::CharStream *input);
  ~RLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

