
// Generated from parser/RFilter.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "RFilter.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by RFilter.
 */
class  RFilterVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by RFilter.
   */
    virtual antlrcpp::Any visitStream(RFilter::StreamContext *context) = 0;

    virtual antlrcpp::Any visitEat(RFilter::EatContext *context) = 0;

    virtual antlrcpp::Any visitElem(RFilter::ElemContext *context) = 0;

    virtual antlrcpp::Any visitComment(RFilter::CommentContext *context) = 0;

    virtual antlrcpp::Any visitAtom(RFilter::AtomContext *context) = 0;

    virtual antlrcpp::Any visitOp(RFilter::OpContext *context) = 0;


};

