#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>
#include "Chunk.hpp"
#include <common/ZeroInitializedStruct.hpp>

using std::string;
using std::ostream;

struct NSRange {
	int location;
	int length;
	
	NSRange() : location(0), length(0) {};
	NSRange(int loc, int len) : location(loc), length(len) {};
};
ostream& operator<<(ostream& os, const NSRange range);

// ChunkType enum
enum ChunkType { markdown, code, equation, inlineEquation, inlineCode };
ostream& operator<<(ostream& os, const ChunkType type);

class InlineChunk; //forward declaration

class Chunk : public ZeroInitializedClass {
public:
	Chunk(ChunkType type, string content, int startLine, int startChar, int endChar, NSRange innerRange)
		: type_(type), content_(content), startLine_(startLine), 
		startCharIndex_(startChar), endCharIndex_(endChar), innerRange_(innerRange)
		{};
	Chunk(const Chunk& other) = default;
	ChunkType type() const { return type_; }
	string content() const { return content_; }
	virtual string innerContent() const { return content_; }
	int startLine() const { return startLine_; }
	int startCharIndex() const { return startCharIndex_; }
	int endCharIndex() const { return endCharIndex_; }
	NSRange range() const { return NSRange(startCharIndex_, endCharIndex_ - startCharIndex_ + 1); }
	NSRange innerRange() const { return innerRange_; }
			bool isInline() const { return type_ == inlineCode || type_ == inlineEquation; }
			bool isCode() const { return type_ == code || type_ == inlineCode; }
				
	string description() const {
		std::ostringstream st;
		st << type_ << " " << range();
		return st.str();
		return "";
	}
	
	
	/**
	 * @brief returns a unique identifier that should be the same on any platform
	 * 
	 * @return string
	 */
	string chunkIdentifier() const;

	// subclasses can override and additional content to the end of the string that will be hashed
	virtual string hashInput();	
	
	virtual string cname() const { return "Chunk"; }
protected:
	Chunk();
	
	ChunkType 	type_;
	string		content_;
	int			startLine_;
	int			startCharIndex_;
	int			endCharIndex_;
	NSRange		innerRange_;
	string		identifier_;
};

class InlineChunk: public Chunk {
public:
	InlineChunk(const InlineChunk& other) : Chunk(other) {}
protected:
	InlineChunk() {};
};

class CodeChunk {
public:
	string arguments() const { return arguments_; }
	string code() const { return code_; }
protected:
	string arguments_;
	string code_;
};

class EquationChunk {
public:
	string code() const { return code_; }
protected:
	string code_;
};

class MarkdownChunk: public Chunk {
public:
	MarkdownChunk();
	MarkdownChunk(const MarkdownChunk& other);
	size_t inlineChunkCount() const { return inlineChunks_.size(); }
	std::vector<InlineChunk*> inlineChunks() const;
	virtual void append(Chunk *chunk) {};
	virtual string cname() const { return "MarkdownChunk"; }
protected:
	virtual string hashInput();
	std::vector<std::unique_ptr<InlineChunk> > inlineChunks_;
};
