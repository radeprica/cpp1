#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class PuzzleException : public std::exception
{
public:
// TODO: get exception to get args and format them
    PuzzleException(const std::string& cause, ...);
    ~PuzzleException() { }

    const std::string get_cause() { return _cause; }

private:
    std::string _cause;
};

#endif //EXCEPTIONS_H