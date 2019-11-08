#include "FormattedException.hpp"
#include <cstdio>

FormattedException::FormattedException(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(_emsg, MAX_ERROR_LEN, format, args);
	va_end(args);
}

FormattedException::FormattedException(int code,  const char *format, ...)
	: GenericException("", code)
{
	va_list args;
	va_start(args, format);
	vsnprintf(_emsg, MAX_ERROR_LEN, format, args);
	va_end(args);
}

GenericException::GenericException(std::string message, int code)
	:_msg(message), _code(code)
{
}
