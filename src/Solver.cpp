#include <iostream>
#include <list>
#include "Solver.h"
#include "Piece.h"

void Solver::sanity(std::list<Piece>& pieces, int num_of_pieces)
{
	std::cout << "input file declared num of pieces: " << num_of_pieces << std::endl;
	std::cout << "parsed num of pieces: " << pieces.size() << std::endl;

	for (const Piece& p : pieces)
	{	
		std::cout << "id: " << p.id << " left: " << p.left << " top: " << p.top << " right: " << p.right << " bottom: " << p.bottom << std::endl;
	}
}