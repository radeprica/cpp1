#include "Puzzle.h"
#include "Piece.h"
#include "Exceptions.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>


void Puzzle::initialize_from_file(const std::string& input_path)
{
    std::ifstream input_file(input_path);
    if (input_file.fail())
    {
        throw PuzzleException(strerror(errno));
    }
    try
    {
        std::string line, key;
        std::getline(input_file, line);
        std::istringstream iss(line);
        unsigned int value;
        unsigned int id, left, top, right, bottom;
        char delim = '=';
        if (!(iss >> key >> delim >> value) || key.compare("NumElements")) 
        {
            throw PuzzleException("Wrong input format in first line");
        }
    
        _num_of_pieces = value;
        _puzzle_pieces = std::vector<PiecePtr>(_num_of_pieces);

        while (std::getline(input_file, line))
        {
            std::istringstream iss(line);
            if (!(iss >> id >> left >> top >> right >> bottom))
            {
                // TODO: add line number to exception message
                throw PuzzleException("Wrong input format in line");
            }

            if(id < 1 || id > _num_of_pieces)
            {
                // TODO: handle this the way it says in the worksheet
                throw PuzzleException("invalid id");
            }
            
            _puzzle_pieces[id-1] = PiecePtr(new Piece(id,
                                                (PieceSideShape)left, 
                                                (PieceSideShape)top, 
                                                (PieceSideShape)right, 
                                                (PieceSideShape)bottom));
        }
        input_file.close();
    }
    catch (PuzzleException& e)
    {
        std::cerr << "puzzle exception caught: " << e.get_cause() << '\n';
        input_file.close();
    }
}

void Puzzle::print_pieces()
{
    for (const PiecePtr p : _puzzle_pieces)
	{	
		std::cout << "id: " << p->get_id() << 
        " left: " << p->get_left_side_shape() << 
        " top: " << p->get_top_side_shape() << 
        " right: " << p->get_right_side_shape() << 
        " bottom: " << p-> get_bottom_side_shape() << std::endl;
	}
}