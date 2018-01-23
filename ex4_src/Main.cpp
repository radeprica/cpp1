#include "ex4headers.h"

using namespace std;

int main()
{
	try
	{
		list<Puzzle2dPiece<5>> pieces = {{0, 3, 2, -5}, {0, -2, 2, -5}};
		auto groups = groupPuzzlePieces(pieces.begin(), pieces.end());
		auto some_pieces = groups.get({0, std::numeric_limits<int>::min(), 2, -5});
		for(auto piece_ptr : some_pieces) {
			std::cout << *piece_ptr << std::endl; // will print both pieces!
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

