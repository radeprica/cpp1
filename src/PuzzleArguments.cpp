#include "PuzzleArguments.h"
#include "Exceptions.h"

#include <vector>

enum args
{
    args_in_file = 0,
    args_out_file,
    args_min,
    args_with_rotate,
    args_with_threads,
    args_max
};

PuzzleArguments::PuzzleArguments(int argc, char **argv)
{
    const std::string rotate_str("-rotate");
	const std::string thread_flag_str("-threads");
	bool rotate_argument_as_expected = false;
	bool thread_arguments_as_expected = false;
	std::vector<std::string> arguments;

	for (int i = 1; i < argc; i++)
	{
		arguments.push_back(std::string(argv[i]));
	}

	if (arguments.size() < args_min || arguments.size() > args_max)
	{
		throw ArgumentsException("Wrong number of arguments");
	}

	_threads = 4;
	_rotate = false;

	if (arguments.size() >= args_with_threads)
	{
		for (auto a = arguments.begin(); a != arguments.end(); a++)
		{
			if (thread_flag_str.compare(*a) == 0)
			{
				a = arguments.erase(a);
				_threads = (unsigned int)atoi((*a).c_str());
				if (_threads < 1)
				{
					throw ArgumentsException("Must have at least one thread");
				}
				arguments.erase(a);
				thread_arguments_as_expected = true;
				break;
			}
		}
		if (!thread_arguments_as_expected)
		{
			throw ArgumentsException("Expected -threads n");
		}
	}

	if (arguments.size() == args_with_rotate)
	{
		for (auto a = arguments.begin(); a != arguments.end(); a++)
		{
			if (rotate_str.compare(*a) == 0)
			{
				_rotate = true;
				arguments.erase(a);
				rotate_argument_as_expected = true;
				break;
			}
		}
		if (!rotate_argument_as_expected)
		{
			throw ArgumentsException("Expected -rotate");
		}
	}

	_input_file_path = arguments.at(args_in_file);
	_output_file_path = arguments.at(args_out_file);
}