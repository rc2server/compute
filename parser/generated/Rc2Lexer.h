
// Generated from ../../Rc2Parser/Rc2Lexer.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  Rc2Lexer : public antlr4::Lexer {
public:
  enum {
    CODE_START = 1, CODE_ARG = 2, EQ_START = 3, IEQ_START = 4, IC_START = 5, 
    MDOWN = 6, IC_END = 7, IC_CODE = 8, EQ_END = 9, EQ_CODE = 10, IEQ_CODE = 11, 
    IEQ_END = 12, CODE_END = 13, NL = 14, CODE = 15, NOT_BACKTICK = 16
  };

  enum {
    IN_ICODE = 1, IN_EQ = 2, IN_IN_EQ = 3, IN_CODE = 4
  };

  Rc2Lexer(antlr4::CharStream *input);
  ~Rc2Lexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;

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


      #include "../LexerHelpers.hpp"


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.
  bool CODE_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool EQ_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool IEQ_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);
  bool CODE_ENDSempred(antlr4::RuleContext *_localctx, size_t predicateIndex);

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

