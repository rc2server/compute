#pragma once

#include <exception>
#include <stdarg.h>

#define MAX_ERROR_LEN 256

class FormattedException : public std::exception {
	public:
		FormattedException(const char *format, ...);
		virtual ~FormattedException() throw() {};
		virtual const char *what() const throw() { return _emsg; };
		
	private:
		char _emsg[MAX_ERROR_LEN];
};
