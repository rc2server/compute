#pragma once

#include "Chunk.hpp"
#include "antlr4-common.h"
#include "generated/Rc2RawParser.h"

class MarkdownChunkImpl: public MarkdownChunk {
public:
	MarkdownChunkImpl(Rc2RawParser::ChunkContext* ctx);
	void append(Rc2RawParser::MdownContext* ctx);
	void append(Chunk* childChunk);
	
	std::vector<std::shared_ptr<InlineChunk>> inlineChunks() const { return inlineChunks_; }
protected:
	std::vector<std::shared_ptr<InlineChunk>> inlineChunks_;
	void dummy() const {}
};

class InlineCodeChunk: public InlineChunk, public CodeChunk {
public:
		InlineCodeChunk(Rc2RawParser::ChunkContext* ctx);
};

class InlineEquationChunk: public InlineChunk, public EquationChunk {
public:
		InlineEquationChunk(Rc2RawParser::ChunkContext* ctx);
};

class CodeChunkImpl: public Chunk, public CodeChunk {
public:
	CodeChunkImpl(Rc2RawParser::ChunkContext* context);
};

class EquationChunkImpl: public Chunk, public EquationChunk {
public:
	EquationChunkImpl(Rc2RawParser::ChunkContext* context);
};
