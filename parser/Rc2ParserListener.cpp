#include "Rc2ParserListener.hpp"
#include "generated/Rc2Lexer.h"
#include "PrivateChunks.hpp"
#include <iostream>

using std::make_unique;

vector<reference_wrapper<Chunk> > 
Rc2ParserListener::chunks() const
{
	vector<reference_wrapper<Chunk> > chunks;
	for (auto &ptr: chunks_) {
		chunks.push_back(std::ref(*ptr));
	}
	return chunks;
}

void 
Rc2ParserListener::enterChunk(Rc2RawParser::ChunkContext* ctx)
{
	curContext_ = ctx;
	antlr4::Token *start = ctx->getStart();
	if (start == nullptr) return;
	Chunk* aChunk;
	
	switch (start->getType()) {
		case Rc2Lexer::MDOWN:
			if (curMarkdownChunk_ == nullptr) {
				auto mdChunk = make_unique<MarkdownChunkImpl>(ctx);
				curMarkdownChunk_ = mdChunk.get();
				aChunk = mdChunk.get();
				chunks_.push_back(std::move(mdChunk));
			} else {
				auto mctx = ctx->mdown();
				curMarkdownChunk_->append(mctx);
				return;
			}
			break;
		case Rc2Lexer::IC_START: {
			auto ck = make_unique<InlineCodeChunk>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			break;
		}
		case Rc2Lexer::IEQ_START: {
			auto ck = make_unique<InlineEquationChunk>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			break;
		}
		case Rc2Lexer::CODE_START: {
			auto ck = make_unique<CodeChunkImpl>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			break;
		}
		case Rc2Lexer::EQ_START: {
			auto ck = make_unique<EquationChunkImpl>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			break;
		}
		default:
			std::cerr << "invalic chunk type: " << start->getType() - 1 << std::endl;
			abort();
	}
	if(aChunk->isInline() && curMarkdownChunk_ == nullptr) {
		std::cerr << "inline chunk found at top" << std::endl;
		auto err = ListenerError(invalidChunkAtRoot, aChunk->startLine(), aChunk->startCharIndex(), "inline chunk found at top level");
		errorReporter_->errors.push_back(err);
	}
	if(aChunk->isInline()) {
		curMarkdownChunk_->append(aChunk);
	}
	curChunk_ = aChunk;
}

void 
Rc2ParserListener::exitChunk(Rc2RawParser::ChunkContext* ctx)
{
	std::cout << "chunks count: " << chunks_.size() << std::endl;
	if (curChunk_ == nullptr) {
		curContext_ = nullptr;
		return;
	}
	if (curChunk_->isInline()) {
		curChunk_ = nullptr;
		return;
	}
	if (curChunk_->type() != markdown) {
		curMarkdownChunk_ = nullptr;
	}
	curContext_ = nullptr;
	curChunk_ = nullptr;
}
