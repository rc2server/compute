#include "FormattedException.hpp"
#include <cstdio>

FormattedException::FormattedException(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(_emsg, MAX_ERROR_LEN, format, args);
	va_end(args);
}
