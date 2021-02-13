#include "Rc2ParserListener.hpp"
#include "generated/Rc2Lexer.h"
#include "PrivateChunks.hpp"
#include <iostream>

using std::make_unique;

Rc2ParserListener::Rc2ParserListener(ErrorReporter* reporter)
		: ZeroInitializedClass(), errorReporter_(reporter), curMarkdownChunk_(nullptr)
{
}

vector<Chunk*> 
Rc2ParserListener::chunks() const
{
	vector<Chunk*> chunks;
	for (auto ck = chunks_.begin(); ck != chunks_.end(); ++ck) {
		chunks.push_back((*ck).get());
	}
	return chunks;
}

void 
Rc2ParserListener::enterChunk(Rc2RawParser::ChunkContext* ctx)
{
	curContext_ = ctx;
	antlr4::Token *start = ctx->getStart();
	if (start == nullptr) return;
	Chunk* aChunk = nullptr;
	
	auto contents = ctx->getText();
	ctx->getText();
	if (std::all_of(contents.begin(), contents.end(), isspace)) {
		return;
	}
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
			auto icc = make_unique<InlineCodeChunk>(ctx);
			aChunk = icc.get();
			curMarkdownChunk_->append(icc.get());
			break;
		}
		case Rc2Lexer::IEQ_START: {
			auto iec = make_unique<InlineEquationChunk>(ctx);
			aChunk = iec.get();
			curMarkdownChunk_->append(iec.get());
			break;
		}
		case Rc2Lexer::CODE_START: {
			auto ck = make_unique<CodeChunkImpl>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			curMarkdownChunk_ = nullptr;
			break;
		}
		case Rc2Lexer::EQ_START: {
			auto ck = make_unique<EquationChunkImpl>(ctx);
			aChunk = ck.get();
			chunks_.push_back(std::move(ck));
			curMarkdownChunk_ = nullptr;
			break;
		// Add YAML case
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
	curChunk_ = aChunk;
}

void 
Rc2ParserListener::exitChunk(Rc2RawParser::ChunkContext* ctx)
{
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
