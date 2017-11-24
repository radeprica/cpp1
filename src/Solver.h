#include<list>
#include "Piece.h"

#ifndef SOLVER_H
#define SOLVER_H

class Solver
{
public:
	static void sanity(std::list<Piece>& pieces, int num_of_pieces);
private:
	Solver() {};
};

#endif