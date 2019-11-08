#pragma once

#include <exception>
#include <stdarg.h>
#include <string>

#define MAX_ERROR_LEN 512


/**
 * @brief An exception that has a message string associated with it.
 * 
 */
class GenericException : public std::exception {
public:
	GenericException(std::string message = "", int code = 0);
	virtual ~GenericException() throw() {};
	virtual const char *what() const throw() { return _msg.c_str(); };
	int code() const { return _code; };
protected:
	std::string _msg;
	int _code;
	
};

class FormattedException : public GenericException {
	public:
		FormattedException(const char *format, ...);
		FormattedException(int code, const char *format, ...);
		virtual ~FormattedException() throw() {};
		virtual const char *what() const throw() { return _emsg; };
		/** @brief allow chaining with initializer */
		FormattedException& code(int code) { _code = code; return *this; };
		
	private:
		char _emsg[MAX_ERROR_LEN];
};
