
// Generated from ../../Rc2Parser/Rc2Lexer.g4 by ANTLR 4.8


#include "Rc2Lexer.h"


using namespace antlr4;


Rc2Lexer::Rc2Lexer(CharStream *input) : Lexer(input) {
  _interpreter = new atn::LexerATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

Rc2Lexer::~Rc2Lexer() {
  delete _interpreter;
}

std::string Rc2Lexer::getGrammarFileName() const {
  return "Rc2Lexer.g4";
}

const std::vector<std::string>& Rc2Lexer::getRuleNames() const {
  return _ruleNames;
}

const std::vector<std::string>& Rc2Lexer::getChannelNames() const {
  return _channelNames;
}

const std::vector<std::string>& Rc2Lexer::getModeNames() const {
  return _modeNames;
}

const std::vector<std::string>& Rc2Lexer::getTokenNames() const {
  return _tokenNames;
}

dfa::Vocabulary& Rc2Lexer::getVocabulary() const {
  return _vocabulary;
}

const std::vector<uint16_t> Rc2Lexer::getSerializedATN() const {
  return _serializedATN;
}

const atn::ATN& Rc2Lexer::getATN() const {
  return _atn;
}


#include "../LexerHelpers.cpp"


bool Rc2Lexer::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 0: return CODE_STARTSempred(dynamic_cast<antlr4::RuleContext *>(context), predicateIndex);
    case 2: return EQ_STARTSempred(dynamic_cast<antlr4::RuleContext *>(context), predicateIndex);
    case 3: return IEQ_STARTSempred(dynamic_cast<antlr4::RuleContext *>(context), predicateIndex);
    case 12: return CODE_ENDSempred(dynamic_cast<antlr4::RuleContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}


bool Rc2Lexer::CODE_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return getCharPositionInLine() == 0;

  default:
    break;
  }
  return true;
}

bool Rc2Lexer::EQ_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return  isEQStart() ;

  default:
    break;
  }
  return true;
}

bool Rc2Lexer::IEQ_STARTSempred(antlr4::RuleContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return  isInlineEqStart() ;

  default:
    break;
  }
  return true;
}

bool Rc2Lexer::CODE_ENDSempred(antlr4::RuleContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 3: return getCharPositionInLine() == 0;

  default:
    break;
  }
  return true;
}


// Static vars and initialization.
std::vector<dfa::DFA> Rc2Lexer::_decisionToDFA;
atn::PredictionContextCache Rc2Lexer::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN Rc2Lexer::_atn;
std::vector<uint16_t> Rc2Lexer::_serializedATN;

std::vector<std::string> Rc2Lexer::_ruleNames = {
  u8"CODE_START", u8"CODE_ARG", u8"EQ_START", u8"IEQ_START", u8"IC_START", 
  u8"MDOWN", u8"IC_END", u8"IC_CODE", u8"EQ_END", u8"EQ_CODE", u8"IEQ_CODE", 
  u8"IEQ_END", u8"CODE_END", u8"NL", u8"CODE", u8"NOT_BACKTICK", u8"ANY"
};

std::vector<std::string> Rc2Lexer::_channelNames = {
  "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
};

std::vector<std::string> Rc2Lexer::_modeNames = {
  u8"DEFAULT_MODE", u8"IN_ICODE", u8"IN_EQ", u8"IN_IN_EQ", u8"IN_CODE"
};

std::vector<std::string> Rc2Lexer::_literalNames = {
  "", "", "", "", "", u8"'`r '", "", u8"'`'", "", u8"'$$'", "", "", u8"'$'"
};

std::vector<std::string> Rc2Lexer::_symbolicNames = {
  "", u8"CODE_START", u8"CODE_ARG", u8"EQ_START", u8"IEQ_START", u8"IC_START", 
  u8"MDOWN", u8"IC_END", u8"IC_CODE", u8"EQ_END", u8"EQ_CODE", u8"IEQ_CODE", 
  u8"IEQ_END", u8"CODE_END", u8"NL", u8"CODE", u8"NOT_BACKTICK"
};

dfa::Vocabulary Rc2Lexer::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> Rc2Lexer::_tokenNames;

Rc2Lexer::Initializer::Initializer() {
  // This code could be in a static initializer lambda, but VS doesn't allow access to private class members from there.
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
    0x2, 0x12, 0x96, 0x8, 0x1, 0x8, 0x1, 0x8, 0x1, 0x8, 0x1, 0x8, 0x1, 0x4, 
    0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 0x4, 0x4, 0x5, 0x9, 
    0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 
    0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 
    0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 
    0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x7, 0x3, 0x33, 0xa, 0x3, 0xc, 0x3, 0xe, 0x3, 0x36, 0xb, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x7, 0x6, 0x7, 0x4e, 0xa, 0x7, 0xd, 0x7, 0xe, 0x7, 0x4f, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x6, 0x9, 0x57, 0xa, 0x9, 0xd, 
    0x9, 0xe, 0x9, 0x58, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x6, 0xb, 0x63, 0xa, 0xb, 0xd, 0xb, 0xe, 
    0xb, 0x64, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 0x6, 0xc, 0x6a, 0xa, 0xc, 0xd, 
    0xc, 0xe, 0xc, 0x6b, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x5, 0xe, 
    0x79, 0xa, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x5, 0xf, 0x7e, 0xa, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x6, 0x10, 0x84, 0xa, 0x10, 
    0xd, 0x10, 0xe, 0x10, 0x85, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x5, 0x12, 
    0x8b, 0xa, 0x12, 0x3, 0x12, 0x5, 0x12, 0x8e, 0xa, 0x12, 0x3, 0x12, 0x3, 
    0x12, 0x7, 0x12, 0x92, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x95, 0xb, 0x12, 
    0x4, 0x4f, 0x93, 0x2, 0x13, 0x7, 0x3, 0x9, 0x4, 0xb, 0x5, 0xd, 0x6, 
    0xf, 0x7, 0x11, 0x8, 0x13, 0x9, 0x15, 0xa, 0x17, 0xb, 0x19, 0xc, 0x1b, 
    0xd, 0x1d, 0xe, 0x1f, 0xf, 0x21, 0x10, 0x23, 0x11, 0x25, 0x12, 0x27, 
    0x2, 0x7, 0x2, 0x3, 0x4, 0x5, 0x6, 0x6, 0x3, 0x2, 0x7f, 0x7f, 0x3, 0x2, 
    0x62, 0x62, 0x3, 0x2, 0x26, 0x26, 0x4, 0x2, 0xb, 0xc, 0x22, 0x22, 0x2, 
    0x9d, 0x2, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2, 0x9, 0x3, 0x2, 0x2, 0x2, 0x2, 
    0xb, 0x3, 0x2, 0x2, 0x2, 0x2, 0xd, 0x3, 0x2, 0x2, 0x2, 0x2, 0xf, 0x3, 
    0x2, 0x2, 0x2, 0x2, 0x11, 0x3, 0x2, 0x2, 0x2, 0x3, 0x13, 0x3, 0x2, 0x2, 
    0x2, 0x3, 0x15, 0x3, 0x2, 0x2, 0x2, 0x4, 0x17, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x19, 0x3, 0x2, 0x2, 0x2, 0x5, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x5, 0x1d, 
    0x3, 0x2, 0x2, 0x2, 0x6, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x6, 0x21, 0x3, 0x2, 
    0x2, 0x2, 0x6, 0x23, 0x3, 0x2, 0x2, 0x2, 0x6, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x7, 0x29, 0x3, 0x2, 0x2, 0x2, 0x9, 0x2e, 0x3, 0x2, 0x2, 0x2, 0xb, 0x3b, 
    0x3, 0x2, 0x2, 0x2, 0xd, 0x41, 0x3, 0x2, 0x2, 0x2, 0xf, 0x46, 0x3, 0x2, 
    0x2, 0x2, 0x11, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x13, 0x51, 0x3, 0x2, 0x2, 
    0x2, 0x15, 0x56, 0x3, 0x2, 0x2, 0x2, 0x17, 0x5a, 0x3, 0x2, 0x2, 0x2, 
    0x19, 0x62, 0x3, 0x2, 0x2, 0x2, 0x1b, 0x69, 0x3, 0x2, 0x2, 0x2, 0x1d, 
    0x6d, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x71, 0x3, 0x2, 0x2, 0x2, 0x21, 0x7d, 
    0x3, 0x2, 0x2, 0x2, 0x23, 0x81, 0x3, 0x2, 0x2, 0x2, 0x25, 0x87, 0x3, 
    0x2, 0x2, 0x2, 0x27, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x29, 0x2a, 0x6, 0x2, 
    0x2, 0x2, 0x2a, 0x2b, 0x7, 0x62, 0x2, 0x2, 0x2b, 0x2c, 0x7, 0x62, 0x2, 
    0x2, 0x2c, 0x2d, 0x7, 0x62, 0x2, 0x2, 0x2d, 0x8, 0x3, 0x2, 0x2, 0x2, 
    0x2e, 0x2f, 0x7, 0x7d, 0x2, 0x2, 0x2f, 0x30, 0x7, 0x74, 0x2, 0x2, 0x30, 
    0x34, 0x3, 0x2, 0x2, 0x2, 0x31, 0x33, 0xa, 0x2, 0x2, 0x2, 0x32, 0x31, 
    0x3, 0x2, 0x2, 0x2, 0x33, 0x36, 0x3, 0x2, 0x2, 0x2, 0x34, 0x32, 0x3, 
    0x2, 0x2, 0x2, 0x34, 0x35, 0x3, 0x2, 0x2, 0x2, 0x35, 0x37, 0x3, 0x2, 
    0x2, 0x2, 0x36, 0x34, 0x3, 0x2, 0x2, 0x2, 0x37, 0x38, 0x7, 0x7f, 0x2, 
    0x2, 0x38, 0x39, 0x3, 0x2, 0x2, 0x2, 0x39, 0x3a, 0x8, 0x3, 0x2, 0x2, 
    0x3a, 0xa, 0x3, 0x2, 0x2, 0x2, 0x3b, 0x3c, 0x6, 0x4, 0x3, 0x2, 0x3c, 
    0x3d, 0x7, 0x26, 0x2, 0x2, 0x3d, 0x3e, 0x7, 0x26, 0x2, 0x2, 0x3e, 0x3f, 
    0x3, 0x2, 0x2, 0x2, 0x3f, 0x40, 0x8, 0x4, 0x3, 0x2, 0x40, 0xc, 0x3, 
    0x2, 0x2, 0x2, 0x41, 0x42, 0x7, 0x26, 0x2, 0x2, 0x42, 0x43, 0x6, 0x5, 
    0x4, 0x2, 0x43, 0x44, 0x3, 0x2, 0x2, 0x2, 0x44, 0x45, 0x8, 0x5, 0x4, 
    0x2, 0x45, 0xe, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x7, 0x62, 0x2, 0x2, 
    0x47, 0x48, 0x7, 0x74, 0x2, 0x2, 0x48, 0x49, 0x7, 0x22, 0x2, 0x2, 0x49, 
    0x4a, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x8, 0x6, 0x5, 0x2, 0x4b, 0x10, 
    0x3, 0x2, 0x2, 0x2, 0x4c, 0x4e, 0xb, 0x2, 0x2, 0x2, 0x4d, 0x4c, 0x3, 
    0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x3, 0x2, 
    0x2, 0x2, 0x4f, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x50, 0x12, 0x3, 0x2, 0x2, 
    0x2, 0x51, 0x52, 0x7, 0x62, 0x2, 0x2, 0x52, 0x53, 0x3, 0x2, 0x2, 0x2, 
    0x53, 0x54, 0x8, 0x8, 0x6, 0x2, 0x54, 0x14, 0x3, 0x2, 0x2, 0x2, 0x55, 
    0x57, 0xa, 0x3, 0x2, 0x2, 0x56, 0x55, 0x3, 0x2, 0x2, 0x2, 0x57, 0x58, 
    0x3, 0x2, 0x2, 0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x3, 
    0x2, 0x2, 0x2, 0x59, 0x16, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x26, 
    0x2, 0x2, 0x5b, 0x5c, 0x7, 0x26, 0x2, 0x2, 0x5c, 0x5d, 0x3, 0x2, 0x2, 
    0x2, 0x5d, 0x5e, 0x8, 0xa, 0x6, 0x2, 0x5e, 0x18, 0x3, 0x2, 0x2, 0x2, 
    0x5f, 0x60, 0x7, 0x26, 0x2, 0x2, 0x60, 0x63, 0xa, 0x4, 0x2, 0x2, 0x61, 
    0x63, 0xa, 0x4, 0x2, 0x2, 0x62, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x62, 0x61, 
    0x3, 0x2, 0x2, 0x2, 0x63, 0x64, 0x3, 0x2, 0x2, 0x2, 0x64, 0x62, 0x3, 
    0x2, 0x2, 0x2, 0x64, 0x65, 0x3, 0x2, 0x2, 0x2, 0x65, 0x1a, 0x3, 0x2, 
    0x2, 0x2, 0x66, 0x6a, 0xa, 0x4, 0x2, 0x2, 0x67, 0x68, 0x9, 0x5, 0x2, 
    0x2, 0x68, 0x6a, 0x7, 0x26, 0x2, 0x2, 0x69, 0x66, 0x3, 0x2, 0x2, 0x2, 
    0x69, 0x67, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6b, 
    0x69, 0x3, 0x2, 0x2, 0x2, 0x6b, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x1c, 
    0x3, 0x2, 0x2, 0x2, 0x6d, 0x6e, 0x7, 0x26, 0x2, 0x2, 0x6e, 0x6f, 0x3, 
    0x2, 0x2, 0x2, 0x6f, 0x70, 0x8, 0xd, 0x6, 0x2, 0x70, 0x1e, 0x3, 0x2, 
    0x2, 0x2, 0x71, 0x72, 0x6, 0xe, 0x5, 0x2, 0x72, 0x73, 0x7, 0x62, 0x2, 
    0x2, 0x73, 0x74, 0x7, 0x62, 0x2, 0x2, 0x74, 0x75, 0x7, 0x62, 0x2, 0x2, 
    0x75, 0x78, 0x3, 0x2, 0x2, 0x2, 0x76, 0x79, 0x5, 0x21, 0xf, 0x2, 0x77, 
    0x79, 0x7, 0x2, 0x2, 0x3, 0x78, 0x76, 0x3, 0x2, 0x2, 0x2, 0x78, 0x77, 
    0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x8, 
    0xe, 0x6, 0x2, 0x7b, 0x20, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7e, 0x7, 0xf, 
    0x2, 0x2, 0x7d, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x3, 0x2, 0x2, 
    0x2, 0x7e, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x80, 0x7, 0xc, 0x2, 0x2, 
    0x80, 0x22, 0x3, 0x2, 0x2, 0x2, 0x81, 0x83, 0x5, 0x21, 0xf, 0x2, 0x82, 
    0x84, 0x5, 0x27, 0x12, 0x2, 0x83, 0x82, 0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 
    0x3, 0x2, 0x2, 0x2, 0x85, 0x83, 0x3, 0x2, 0x2, 0x2, 0x85, 0x86, 0x3, 
    0x2, 0x2, 0x2, 0x86, 0x24, 0x3, 0x2, 0x2, 0x2, 0x87, 0x88, 0xa, 0x3, 
    0x2, 0x2, 0x88, 0x26, 0x3, 0x2, 0x2, 0x2, 0x89, 0x8b, 0x7, 0x62, 0x2, 
    0x2, 0x8a, 0x89, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x3, 0x2, 0x2, 0x2, 
    0x8b, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8e, 0x7, 0x62, 0x2, 0x2, 0x8d, 
    0x8c, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 
    0x3, 0x2, 0x2, 0x2, 0x8f, 0x93, 0x5, 0x25, 0x11, 0x2, 0x90, 0x92, 0xb, 
    0x2, 0x2, 0x2, 0x91, 0x90, 0x3, 0x2, 0x2, 0x2, 0x92, 0x95, 0x3, 0x2, 
    0x2, 0x2, 0x93, 0x94, 0x3, 0x2, 0x2, 0x2, 0x93, 0x91, 0x3, 0x2, 0x2, 
    0x2, 0x94, 0x28, 0x3, 0x2, 0x2, 0x2, 0x95, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x14, 0x2, 0x3, 0x4, 0x5, 0x6, 0x34, 0x4f, 0x58, 0x62, 0x64, 0x69, 0x6b, 
    0x78, 0x7d, 0x85, 0x8a, 0x8d, 0x93, 0x7, 0x7, 0x6, 0x2, 0x7, 0x4, 0x2, 
    0x7, 0x5, 0x2, 0x7, 0x3, 0x2, 0x6, 0x2, 0x2, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

Rc2Lexer::Initializer Rc2Lexer::_init;
