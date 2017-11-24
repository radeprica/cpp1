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
    static const std::string num_elements_str("NumElements");
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
        char delim = '=';
        std::getline(iss, key, delim);
        if (key.compare(0, num_elements_str.length(), num_elements_str) != 0)
        {
            throw PuzzleException("Wrong input format in first line");
        }
        if (!(iss >> value)) 
        {
            throw PuzzleException("Wrong input format in first line");
        }
    
        _num_of_pieces = value;
        _puzzle_pieces = std::vector<PiecePtr>(_num_of_pieces);
        for (unsigned int i = 1; i <= _num_of_pieces; i++)
        {
            _missing_ids.push_back(i);
        }

        while (std::getline(input_file, line))
        {
            parse_piece_line(line);
        }

        input_file.close();
    }
    catch (PuzzleException& e)
    {
        std::cerr << "puzzle exception caught: " << e.get_cause() << '\n';
        input_file.close();
    }
}

void Puzzle::parse_piece_line(const std::string& line)
{
    unsigned int id;
    int left, top, right, bottom;
    std::istringstream iss(line);
    if (!(iss >> id))
    {
        // TODO: add line number to exception message
        throw PuzzleException("Wrong input format in line");
    }
    if (id < 1 || id > _num_of_pieces)
    {
        _wrong_id_pieces.push_back(id);
        return;
    }

    _missing_ids.remove(id);

    if (!(iss >> left >> top >> right >> bottom) || !iss.eof())
    {
        std::pair<int, std::string> bad_line_pair(id, line);
        _wrong_format_pieces.push_back(bad_line_pair);
        return;
    }
    if(left < -1 || left > 1 || 
        top < -1 || top > 1 || 
        right < -1 || right > 1 || 
        bottom < -1 || bottom > 1)
        {
            std::pair<int, std::string> bad_line_pair(id, line);
           _wrong_format_pieces.push_back(bad_line_pair); 
        }
    _puzzle_pieces.push_back(PiecePtr(new Piece(id,
                                        (PieceSideShape)left, 
                                        (PieceSideShape)top, 
                                        (PieceSideShape)right, 
                                        (PieceSideShape)bottom)));

}

void Puzzle::print_pieces()
{
    for (const PiecePtr p : _puzzle_pieces)
	{	
        if(p.get() == NULL)
        {
            continue;
        }
		std::cout << "id: " << p->get_id() << 
        " left: " << p->get_left_side_shape() << 
        " top: " << p->get_top_side_shape() << 
        " right: " << p->get_right_side_shape() << 
        " bottom: " << p-> get_bottom_side_shape() << std::endl;
	}
}