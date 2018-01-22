#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <limits> 

#include "GroupPuzzlePieces.h"
#include "Puzzle2dPiece.h"
#include "Exceptions.h"
#include "GroupPuzzlePiecesIterator.h"


#define USAGE "Usage: <program> <input_file> <output_file> [-rotate] [-threads n]\n"

int main()
{
	try
	{
		std::list<Puzzle2dPiece> pieces = {{0, 1, -1, 1}, {0, 1, -1, 1}, {0, -1, -1, 1}};
    	auto groups = groupPuzzlePieces(pieces);
    
    	auto some_pieces = groups.get(0, std::numeric_limits<int>::min(), -1, 1);

		for(auto p : some_pieces)
		{
			std::cout << "{" << (p)->get_left_side_shape() << ", " << 
                         (p)->get_top_side_shape() << ", " << 
                          (p)->get_right_side_shape() << ", " << 
                           (p)->get_bottom_side_shape() << "}, " << std::endl;
		}

		groups.print_me();
		return 0;

	}
	catch (PuzzleException& e)
	{
		std::cout << "puzzle exception caught: " << e.get_cause() << std::endl;
		return -1;
	}
	catch (const std::exception& e)
	{
		std::cout << "std exception caught: " << e.what() << std::endl;
		return -1;
	}
	catch(...)
	{
		std::cout <<"Something went terribly wrong!" << std::endl;
		return -1;
	}
}

