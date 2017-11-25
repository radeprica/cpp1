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

int main(int argc, char* argv[])
{
	try
	{
		if (argc != arguments::max_arguments)
		{
			printf("Wrong usage\n");
			return -1;
		}
		
		Logger::set_logger(argv[arguments::output_file]);

		Puzzle puzzle;
		puzzle.initialize_from_file(argv[1]);
		puzzle.print_pieces();
		if(puzzle.had_initialization_errors())
		{
			puzzle.log_initialization_errors();
			return -1;
		}
		bool b = puzzle.is_wrong_number_of_straight_edges();
		if (b)
		{
			LOG << "Cannot solve puzzle: wrong number of straight edges" << std::endl ;
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