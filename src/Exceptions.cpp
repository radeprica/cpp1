#include <stdarg.h>

#include "Exceptions.h"

PuzzleException::PuzzleException(const std::string& cause, ...)
{
    int size_of_cause = -1;
    va_list args;
    va_start(args, cause);
    size_of_cause = vsnprintf (const_cast<char *>(_cause.data()), 0, cause.c_str(), args);
    va_end(args);
    va_start(args, cause);
    _cause.resize(size_of_cause + 1);
    vsnprintf (const_cast<char *>(_cause.data()), size_of_cause, cause.c_str(), args);
    va_end(args);
}