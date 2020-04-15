
// Generated from parser/RFilter.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RFilterVisitor.h"


/**
 * This class provides an empty implementation of RFilterVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  RFilterBaseVisitor : public RFilterVisitor {
public:

  virtual antlrcpp::Any visitStream(RFilter::StreamContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEat(RFilter::EatContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitElem(RFilter::ElemContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitComment(RFilter::CommentContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAtom(RFilter::AtomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOp(RFilter::OpContext *ctx) override {
    return visitChildren(ctx);
  }


};

