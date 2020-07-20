#pragma once

#include "Chunk.hpp"
#include "antlr4-common.h"
#include "generated/Rc2RawParser.h"

using std::vector;

class MarkdownChunkImpl: public MarkdownChunk {
public:
	MarkdownChunkImpl(Rc2RawParser::ChunkContext* ctx);
	void append(Rc2RawParser::MdownContext* ctx);
	void append(Chunk* childChunk);
	
//	vector<InlineChunk*> inlineChunks() const;
	virtual string cname() const { return "MarkdownChunkImpl"; }
protected:
//	std::vector<std::unique_ptr<InlineChunk>> inlineChunks_;
	void dummy() const {}
};

class InlineCodeChunk: public InlineChunk, public CodeChunk {
public:
		InlineCodeChunk(Rc2RawParser::ChunkContext* ctx);
		InlineCodeChunk(const InlineCodeChunk& rhs) : InlineChunk(rhs) {}
		virtual string cname() const { return "InlineCodeChunk"; }
};

class InlineEquationChunk: public InlineChunk, public EquationChunk {
public:
		InlineEquationChunk(Rc2RawParser::ChunkContext* ctx);
		InlineEquationChunk(const InlineEquationChunk& rhs) : InlineChunk(rhs) {}
		virtual string cname() const { return "InlineEquationChunk"; }
};

class CodeChunkImpl: public Chunk, public CodeChunk {
public:
	CodeChunkImpl(Rc2RawParser::ChunkContext* context);
	virtual string innerContent() const { return code_; }
};

class EquationChunkImpl: public Chunk, public EquationChunk {
public:
	EquationChunkImpl(Rc2RawParser::ChunkContext* context);
};
