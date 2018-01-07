#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

#include "PuzzleArguments.h"
#include "Logger.h"
#include "Piece.h"
#include "Puzzle.h"
#include "Exceptions.h"


#define USAGE "Usage: <program> <input_file> <output_file> [-rotate] [-threads n]\n"

int main(int argc, char* argv[])
{
	try
	{
		PuzzleArguments args(argc, argv);
		Logger::set_logger(args.get_output_file_path());

		Puzzle puzzle(args.get_rotate(), args.get_thread_number());
		puzzle.initialize_from_file(args.get_input_file_path());

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
	catch (ArgumentsException& e)
	{
		printf("Wrong Usage!: %s\n" USAGE, e.get_cause().c_str());
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

