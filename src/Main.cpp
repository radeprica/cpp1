#include <fstream>
#include <sstream>
#include <list>
#include "Piece.h"
#include "Solver.h"

int main(int argc, char* argv[])
{
	std::ifstream infile(argv[1]);
	std::string line;

	int num_of_pieces;

	std::getline(infile, line);
	std::istringstream iss(line);
	std::string key;
	int value;
	char delim = '=';
	if (!(iss >> key >> delim >> value)) {}
	num_of_pieces = value;

	std::list<Piece> pieces_lst;

	while (std::getline(infile, line))
	{
		std::istringstream iss(line);
		int id, left, top, right, bottom;
		if (!(iss >> id >> left >> top >> right >> bottom)) {}

		Piece p = Piece(id, left, top, right, bottom);
		pieces_lst.push_back(p);
	}
	
	Solver::sanity(pieces_lst, num_of_pieces);

	return 0;
}