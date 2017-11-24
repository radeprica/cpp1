#include "Logger.h"

std::shared_ptr<std::ofstream> Logger::_logger_stream;



void Logger::set_logger(const std::string output_path)
{
    _logger_stream.reset(new std::ofstream(output_path));
}