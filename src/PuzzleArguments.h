#include <string>

class PuzzleArguments
{
public:
    PuzzleArguments(int argc, char **argv);
    ~PuzzleArguments() { };

    const std::string get_input_file_path() { return _input_file_path; }
    const std::string get_output_file_path() { return _output_file_path; }
    bool get_rotate() { return _rotate; }
    unsigned int get_thread_number() { return _threads; }

private:
    std::string _input_file_path;
	std::string _output_file_path;
	bool _rotate;
	unsigned int _threads;
};