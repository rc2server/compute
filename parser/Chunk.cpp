#include "Chunk.hpp"
#include <memory>
#include <algorithm>
#include <sstream>
#include "../common/RC2Utils.hpp"
#include "PrivateChunks.hpp"
#include "RmdParser.hpp"

Chunk::Chunk()
{
}

string 
Chunk::chunkIdentifier() const {
	if(identifier_.length() < 10) {
		auto nonconst = const_cast<Chunk*>(this);
		string rawStr = nonconst->hashInput();
		nonconst->identifier_ = RC2::CalculateSHA256Hash(rawStr);
	}
	return identifier_;
}

string
Chunk::hashInput() {
	std::stringstream ss;
	ss << type_ << content_ << startLine_ << endCharIndex_ << startCharIndex_;
	return ss.str();
}


MarkdownChunk::MarkdownChunk()
	: inlineChunks_()
{
}

MarkdownChunk::MarkdownChunk(const MarkdownChunk& other)
	: Chunk(other)
{
	inlineChunks_.reserve(other.inlineChunks_.size());
	
	std::transform(
		std::begin(other.inlineChunks_),
		std::end(other.inlineChunks_),
		std::back_inserter(inlineChunks_),
		[](const std::unique_ptr<InlineChunk>& myptr) {
			if(typeid(&myptr) == typeid(InlineEquationChunk)) {
				InlineEquationChunk *aChunk = (InlineEquationChunk*)&myptr;
				return std::unique_ptr<InlineChunk>(myptr ? new InlineEquationChunk(*aChunk) : nullptr);
			} else if(typeid(&myptr) == typeid(InlineCodeChunk)) {
				InlineCodeChunk *aChunk = (InlineCodeChunk*)&myptr;
				return std::unique_ptr<InlineChunk>(myptr ? new InlineCodeChunk(*aChunk) : nullptr);
		}
			abort();
		});
	
}

std::vector<InlineChunk*> 
MarkdownChunk::inlineChunks() const
{
	std::vector<InlineChunk*> ichunks;
	for (auto ck = inlineChunks_.begin(); ck != inlineChunks_.end(); ++ck) {
		ichunks.push_back(ck->get());
	}
	return ichunks;
}

string
MarkdownChunk::hashInput() {
	string working;
	const auto &ichunks = inlineChunks_;
	for (auto ck = ichunks.begin(); ck != ichunks.end(); ++ck) {
		auto chunk = (*ck).get();
		if(typeid(chunk) == typeid(InlineCodeChunk))
			working += chunk->hashInput();
	}
	return working;
}

ostream& operator<<(ostream& os, const NSRange range) {
	os << "{loc=" << range.location << ",len=" << range.length << "}";
	return os;
}

ostream& operator<<(ostream& os, const ChunkType type)
{
	switch(type) {
		case markdown: 
			os << "markdown"; break;
		case code:
			os << "code"; break;
		case equation:
			os << "equation"; break;
		case inlineCode:
			os << "inlineCode"; break;
		case inlineEquation:
			os << "inlineEquation"; break;
	}
	return os;
}

