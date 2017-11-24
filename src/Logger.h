#include <memory>
#include <fstream>
#include <string>

#define LOG (*Logger::get_stream().get())

class Logger
{
public:
    static void set_logger(const std::string output_path);
    static std::shared_ptr<std::ofstream> get_stream() { return _logger_stream; }

private:
    static std::shared_ptr<std::ofstream> _logger_stream;

private:
    Logger();
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};