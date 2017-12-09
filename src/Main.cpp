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
	input_file,
	output_file,
	max_arguments,
};

#define USAGE "Usage: <program> <input_file> <output_file>\n"

int main(int argc, char* argv[])
{
	try
	{
		if (argc != arguments::max_arguments)
		{
			printf(USAGE);
			return -1;
		}
		
		Logger::set_logger(argv[arguments::output_file]);

		Puzzle puzzle;
		puzzle.initialize_from_file(argv[1]);

		if(puzzle.had_initialization_errors())
		{
			return -1;
		}
		if (!puzzle.solve())
		{
			return -1;
		}
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
	}
	catch(...)
	{
		printf("Something went terribly wrong!\n");
	}
	return 0;
}