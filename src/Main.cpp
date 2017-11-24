#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include "Piece.h"
#include "Puzzle.h"
#include "Exceptions.h"

enum arguments
{
	no_arguments = 0,
	input_file,
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
		Puzzle puzzle;
		puzzle.initialize_from_file(argv[1]);
		puzzle.print_pieces();
		std::list<unsigned int> missing_pieces = puzzle.get_missing_pieces();
		std::cout << "Missing puzzle element(s) with the following IDs: ";
		for (std::list<unsigned int>::const_iterator i = missing_pieces.begin(); i != missing_pieces.end(); ++i)
    	{
			// TODO: get the last element to not print ,
			std::cout << *i << ", ";
		}
		std::cout << std::endl;

		std::list<int> wrong_ids = puzzle.get_wrong_id_pieces();
		std::cout << "Puzzle of size " << puzzle.get_size() << " cannot have the following IDs: ";
		for (std::list<int>::const_iterator i = wrong_ids.begin(); i != wrong_ids.end(); ++i)
    	{
			std::cout << *i << ' ';
		}
		std::cout << std::endl;

		std::list<std::pair<int, std::string>> wrong_formats = puzzle.get_wrong_format_pieces();
		for (std::list<std::pair<int, std::string>>::const_iterator i = wrong_formats.begin(); i != wrong_formats.end(); ++i)
    	{
			std::cout << "Puzzle ID " << i->first << " has wrong data: ";
			std::cout << i->second;
			std::cout << std::endl;
		}
	}
	catch (PuzzleException& e)
	{
		std::cerr << "puzzle exception caught: " << e.get_cause() << '\n';
	}
	catch(...)
	{
		printf("Something went terribly wrong!\n");
	}
	return 0;
}