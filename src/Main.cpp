#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "Logger.h"
#include "Piece.h"
#include "Puzzle.h"
#include "Exceptions.h"

enum arguments
{
	no_arguments = 0,
	arg_input_file,
	arg_output_file,
	arg_rotate,
	max_arguments,
};

#define USAGE "Usage: <program> <input_file> <output_file> [-rotate]\n"

int main(int argc, char* argv[])
{
	try
	{
		const std::string rotate_str("-rotate");

		std::string input_file_path, output_file_path;
		bool rotate = false;

		if (argc > max_arguments || argc < arg_rotate)
		{
			printf("lala " USAGE);
			return -1;
		}

		if (argc == arg_rotate)
		{
			input_file_path = std::string(argv[arg_input_file]);
			output_file_path = std::string(argv[arg_output_file]);
		}
		else
		{
			if (rotate_str.compare(argv[arg_input_file]) != 0)
			{
				input_file_path = std::string(argv[arg_input_file]);
				if (rotate_str.compare(argv[arg_output_file]) != 0)
				{
					output_file_path = std::string(argv[arg_output_file]);
					if (rotate_str.compare(argv[arg_rotate]) != 0)
					{
						printf(USAGE);
						return -1;
					}
				}
				else
				{
					output_file_path = std::string(argv[arg_output_file + 1]);
				}
				
			}
			else
			{
				input_file_path = std::string(argv[arg_input_file + 1]);
				output_file_path = std::string(argv[arg_output_file + 1]);
			}
			rotate = true;

		}

		Logger::set_logger(output_file_path);

		Puzzle puzzle(rotate, 4);
		puzzle.initialize_from_file(input_file_path);

		if(puzzle.had_initialization_errors())
		{
			return -1;
		}
		if (!puzzle.solve())
		{
			return -1;
		}
		return 0;
	}
	catch (PuzzleException& e)
	{
		if(Logger::is_set())
		{
			LOG << "puzzle exception caught: " << e.get_cause() << '\n';
		}
		else
		{
			std::cout << "puzzle exception caught: " << e.get_cause() << '\n';
		}
		return -1;
	}
	catch (const std::exception& e)
	{
		if(Logger::is_set())
		{
			LOG << "std exception caught: " << e.what() << '\n';
		}
		else
		{
			LOG << "std exception caught: " << e.what() << '\n';
		}
		return -1;
	}
	catch(...)
	{
		printf("Something went terribly wrong!\n");
		return -1;
	}
}