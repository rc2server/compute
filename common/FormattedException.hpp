#pragma once

#include <exception>
#include <stdarg.h>
#include <string>

#define MAX_ERROR_LEN 512

class FormattedException : public std::exception {
	public:
		FormattedException(const char *format, ...);
		virtual ~FormattedException() throw() {};
		virtual const char *what() const throw() { return _emsg; };
		
	private:
		char _emsg[MAX_ERROR_LEN];
};

/**
 * @brief An exception that has a message string associated with it.
 * 
 */
class GenericException : public std::exception {
public:
	GenericException(std::string message);
	virtual ~GenericException() throw() {};
	virtual const char *what() const throw() { return _msg.c_str(); };
private:
	std::string _msg;
	
};
