#include <errno.h>
#include <cstring>

#include "Logger.h"
#include "Exceptions.h"

std::shared_ptr<std::ofstream> Logger::_logger_stream;



void Logger::set_logger(const std::string output_path)
{
    _logger_stream.reset(new std::ofstream(output_path));
    if (_logger_stream->fail())
    {
        _logger_stream.reset();
        throw PuzzleException("Failed opening output file %s", strerror(errno));
    }
}