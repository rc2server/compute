#pragma once

#include <RInside.h>
#include <string>


class PreviewDelegate {
public:
	virtual void sendPreviewJson(string jsonStr) = 0;
	virtual void executePreviewCode(string code, SEXP& result, Rcpp::Environment* environment) = 0;
};
	
