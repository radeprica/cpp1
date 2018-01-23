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

using namespace std;

int main()
{
	try
	{
		list<Puzzle2dPiece<5>> pieces = {{0, 3, 2, -5}, {0, -2, 2, -5}};
		auto groups = groupPuzzlePieces(pieces.begin(), pieces.end());
		// note that there is no & on the auto below (was in previous version)
		auto some_pieces = groups.get(0, std::numeric_limits<int>::min(), 2, -5);
		for(auto piece_ptr : some_pieces) {
			cout << "{" << piece_ptr->get_left_side_shape() << ", " << 
                         piece_ptr->get_top_side_shape() << ", " << 
                          piece_ptr->get_right_side_shape() << ", " << 
                           piece_ptr->get_bottom_side_shape() << "}, " << std::endl;
		}
		return 0;

	}
	catch (PuzzleException& e)
	{
		cout << "puzzle exception caught: " << e.get_cause() << endl;
		return -1;
	}
	catch (const std::exception& e)
	{
		cout << "std exception caught: " << e.what() << endl;
		return -1;
	}
	catch(...)
	{
		cout <<"Something went terribly wrong!" << endl;
		return -1;
	}
}

