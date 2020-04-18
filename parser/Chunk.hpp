#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <memory>

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

class Chunk {
public:
	Chunk(ChunkType type, string content, int startLine, int startChar, int endChar, NSRange innerRange)
		: type_(type), content_(content), startLine_(startLine), 
		startCharIndex_(startChar), endCharIndex_(endChar), innerRange_(innerRange)
		{};
		
	virtual ChunkType type() const { return type_; }
	virtual string content() const { return content_; }
	virtual int startLine() const { return startLine_; }
	virtual int startCharIndex() const { return startCharIndex_; }
	virtual int endCharIndex() const { return endCharIndex_; }
	virtual NSRange range() const { return NSRange(startCharIndex_, endCharIndex_ - startCharIndex_ + 1); }
	virtual NSRange innerRange() const { return innerRange_; }
			bool isInline() const { return type_ == inlineCode || type_ == inlineEquation; }
			bool isCode() const { return type_ == code || type_ == inlineCode; }
			
	virtual string description() const {
		std::ostringstream st;
		st << type_ << " " << range();
		return st.str();
		return "";
	}
	
protected:
	Chunk() {};
	
	ChunkType 	type_;
	string		content_;
	int			startLine_;
	int			startCharIndex_;
	int			endCharIndex_;
	NSRange		innerRange_;
};

class InlineChunk: public Chunk {
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
	std::vector<std::shared_ptr<InlineChunk>> inlineChunks() const { return inlineChunks_; }
protected:
	std::vector<std::shared_ptr<InlineChunk>> inlineChunks_;
	
	virtual void dummy() const = 0;
};
