#include "PrivateChunks.hpp"

MarkdownChunkImpl::MarkdownChunkImpl(Rc2RawParser::ChunkContext* ctx)
{
	antlr4::Token *token = ctx->getStart();
	if (token == nullptr) abort();
	
	content_ = ctx->getText();
	startLine_ = token->getLine();
	startCharIndex_ = token->getStartIndex();
	endCharIndex_ = token->getStopIndex();
	type_ = markdown;
}

// vector<InlineChunk*> 
// MarkdownChunkImpl::inlineChunks() const
// {
// 	vector<InlineChunk*> ichunks;
// 	for (auto ck = inlineChunks_.begin(); ck != inlineChunks_.end(); ++ck) {
// 		ichunks.push_back(ck->get());
// 	}
// 	return ichunks;
// }

void MarkdownChunkImpl::append(Rc2RawParser::MdownContext* ctx)
{
	if (ctx->stop == nullptr) abort();
	content_ += ctx->getText();
	endCharIndex_ = ctx->stop->getStopIndex();
}

void MarkdownChunkImpl::append(Chunk* childChunk)
{
	auto ichunk = dynamic_cast<InlineChunk*>(childChunk);
	if (ichunk == nullptr) abort();
	auto codeC = dynamic_cast<InlineCodeChunk*>(childChunk);
	auto eqC = dynamic_cast<InlineEquationChunk*>(childChunk);
	// FIXME: problem is a shared pointer can't be created for an InlineChunk because it is abstract. Does it need to be non-abstract? Likely by making constructor protected.
	if (codeC != nullptr) {
		inlineChunks_.push_back(std::make_unique<InlineCodeChunk>(*codeC));
	} else if (eqC != nullptr) {
		inlineChunks_.push_back(std::make_unique<InlineEquationChunk>(*eqC));
	}
}


CodeChunkImpl::CodeChunkImpl(Rc2RawParser::ChunkContext* context)
	: Chunk(), CodeChunk()
{
	auto ctx = context->code();
	if (ctx == nullptr) abort();
	if (ctx->CODE_START() == nullptr || ctx->CODE_ARG() == nullptr ||
		ctx->CODE_END() == nullptr || ctx->CODE() == nullptr)
	{
		abort();
	}
	auto start = ctx->CODE_START()->getSymbol();
	auto rawCode = ctx->CODE()->getSymbol();
	
	type_ = ChunkType::code;
	content_ = ctx->getText();
	arguments_ = ctx->CODE_ARG()->getSymbol()->getText();
	code_ = rawCode->getText();
	startLine_ = start->getLine();
	startCharIndex_ = start->getStartIndex();
	endCharIndex_ = ctx->CODE_END()->getSymbol()->getStopIndex();
	innerRange_ = NSRange(rawCode->getStartIndex(), rawCode->getStopIndex() - rawCode->getStartIndex() + 1);
}

EquationChunkImpl::EquationChunkImpl(Rc2RawParser::ChunkContext* context)
	: Chunk(), EquationChunk()
{
	auto ctx = context->equation();
	if (ctx == nullptr) abort();
	if (ctx->EQ_START() == nullptr || ctx->EQ_CODE() == nullptr || ctx->EQ_END() == nullptr) abort();
	auto start = ctx->EQ_START()->getSymbol();
	auto eqcode = ctx->EQ_CODE()->getSymbol();
	
	content_ = ctx->getText();
	startLine_ = start->getLine();
	startCharIndex_ = start->getStartIndex();
	endCharIndex_ = ctx->EQ_END()->getSymbol()->getStopIndex();
	code_ = eqcode->getText();
	innerRange_ = NSRange(eqcode->getStartIndex(), eqcode->getStopIndex() - eqcode->getStartIndex() + 1);	
}



InlineCodeChunk::InlineCodeChunk(Rc2RawParser::ChunkContext* context)
	: InlineChunk(), CodeChunk()
{
	auto ctx = context->inlineCode();
	if (ctx == nullptr || ctx->IC_START() == nullptr || 
		ctx->IC_CODE() == nullptr || ctx->IC_END() == nullptr) 
	{
		abort();
	}
	auto start = ctx->IC_START()->getSymbol();
	auto rawCode = ctx->IC_CODE()->getSymbol();
	type_ = inlineCode;
	content_ = context->getText();
	code_ = rawCode->getText();
	startLine_ = start->getLine();
	startCharIndex_ = start->getStartIndex();
	endCharIndex_ = ctx->IC_END()->getSymbol()->getStopIndex();
	innerRange_ = NSRange(rawCode->getStartIndex(), rawCode->getStopIndex() - rawCode->getStartIndex() + 1);
}

InlineEquationChunk::InlineEquationChunk(Rc2RawParser::ChunkContext* context)
	: InlineChunk(), EquationChunk()
{
	if (context == nullptr) abort();
	auto ictx = context->inlineEQ();
	if (ictx == nullptr || ictx->IEQ_START() == nullptr || ictx->IEQ_CODE() == nullptr || ictx->IEQ_END() == nullptr) abort();
	auto start = ictx->IEQ_START()->getSymbol();
	auto eqCode = ictx->IEQ_CODE()->getSymbol();
	
	type_ = inlineEquation;
	content_ = ictx->getText();
	startLine_ = start->getLine();
	startCharIndex_ = start->getStartIndex();
	endCharIndex_ = eqCode->getStopIndex();
	code_ = eqCode->getText();
	innerRange_ = NSRange(eqCode->getStartIndex(), eqCode->getStopIndex() - eqCode->getStartIndex() + 1);
}
