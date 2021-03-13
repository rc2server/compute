#pragma once

#include <vector>
#include <string>
#include <memory>
#include <common/ZeroInitializedStruct.hpp>
#include "PrivateChunks.hpp"
#include "generated/Rc2RawParserBaseListener.h"

using std::string;
using std::unique_ptr;
using std::vector;
using std::reference_wrapper;

enum ListenerErrorType { invalidChunkAtRoot };

struct ListenerError {
	ListenerError(ListenerErrorType et, int lineNum, int charIdx, string desc)
		: type(et), lineNumber(lineNum), charIndex(charIdx), description(desc)
		{}

	ListenerErrorType type;
	int lineNumber;
	int charIndex;
	string description;
};

struct ErrorReporter {
	std::vector<ListenerError> errors;
};

class Rc2ParserListener: public Rc2RawParserBaseListener, public ZeroInitializedClass {
public:
	
	Rc2ParserListener(ErrorReporter* reporter);
		
	vector<Chunk*> chunks() const;
	
	void enterChunk(Rc2RawParser::ChunkContext * ctx);
	void exitChunk(Rc2RawParser::ChunkContext * ctx);
	
protected:
	ErrorReporter* errorReporter_;
	vector<unique_ptr<Chunk>> chunks_;
	Rc2RawParser::ChunkContext* curContext_;
	Chunk *curChunk_;
	MarkdownChunkImpl *curMarkdownChunk_;
	bool skippedWhiteSpaceContent;
};

