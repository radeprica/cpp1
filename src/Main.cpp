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
		std::vector <unsigned int> missing_pieces = puzzle.get_missing_pieces();
		std::cout << "missing pieces: " << std::endl;
		for (std::vector<unsigned int>::const_iterator i = missing_pieces.begin(); i != missing_pieces.end(); ++i)
    	{
			std::cout << *i << ' ';
		}
		std::cout << std::endl;
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