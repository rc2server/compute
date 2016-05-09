#pragma once

#include <Rcpp.h>

namespace RC2 {
	typedef std::function<bool (const std::string&, Rcpp::RObject&)> ExecuteCallback;
	
};
