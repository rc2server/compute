
// Generated from parser/RFilter.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RFilter.h"


/**
 * This interface defines an abstract listener for a parse tree produced by RFilter.
 */
class  RFilterListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterStream(RFilter::StreamContext *ctx) = 0;
  virtual void exitStream(RFilter::StreamContext *ctx) = 0;

  virtual void enterEat(RFilter::EatContext *ctx) = 0;
  virtual void exitEat(RFilter::EatContext *ctx) = 0;

  virtual void enterElem(RFilter::ElemContext *ctx) = 0;
  virtual void exitElem(RFilter::ElemContext *ctx) = 0;

  virtual void enterComment(RFilter::CommentContext *ctx) = 0;
  virtual void exitComment(RFilter::CommentContext *ctx) = 0;

  virtual void enterAtom(RFilter::AtomContext *ctx) = 0;
  virtual void exitAtom(RFilter::AtomContext *ctx) = 0;

  virtual void enterOp(RFilter::OpContext *ctx) = 0;
  virtual void exitOp(RFilter::OpContext *ctx) = 0;


};

