#include "Puzzle.h"
#include "Piece.h"
#include "Exceptions.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>


void Puzzle::initialize_from_file(const std::string& input_path)
{
    static const std::string num_elements_str("NumElements");
    static const char delim = '=';

    std::ifstream input_file(input_path);
    if (input_file.fail())
    {
        throw PuzzleException(strerror(errno));
    }

    std::string line, key;
    unsigned int value;
    
    // Parse first line
    std::getline(input_file, line);
    std::istringstream iss(line); 
    std::getline(iss, key, delim);
    if (key.compare(0, num_elements_str.length(), num_elements_str) != 0)
    {
        throw PuzzleException("Wrong input format in first line");
    }
    if (!(iss >> value) || !iss.eof()) 
    {
        throw PuzzleException("Wrong input format in first line");
    }

    _num_of_pieces = value;
    _puzzle_pieces = std::vector<PiecePtr>(_num_of_pieces);

    // Starting with all pieces missing
    for (unsigned int i = 1; i <= _num_of_pieces; i++)
    {
        _missing_ids.push_back(i);
    }

    // Parse next lines
    while (std::getline(input_file, line))
    {
        parse_piece_line(line);
    }

    input_file.close();
}

void Puzzle::parse_piece_line(const std::string& line)
{
    unsigned int id;
    int left, top, right, bottom;
    std::istringstream iss(line);
    if (iss.eof())
    {
        return;
    }
    if (!(iss >> id))
    {
        // TODO: add line number to exception message
        throw PuzzleException("Wrong input format in line: %s", line.c_str());
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
    _puzzle_pieces[id-1] = PiecePtr(new Piece(id,
                                        (PieceSideShape)left, 
                                        (PieceSideShape)top, 
                                        (PieceSideShape)right, 
                                        (PieceSideShape)bottom));

}

bool Puzzle::had_initialization_errors()
{
    return (!_missing_ids.empty() ||
            ! _wrong_format_pieces.empty() ||
            !_wrong_id_pieces.empty());
}

void Puzzle::log_initialization_errors()
{
    if (!_missing_ids.empty())
    {
        LOG << "Missing puzzle element(s) with the following IDs: ";
        std::list<unsigned int>::const_iterator i = _missing_ids.begin();
        LOG << *i;
        i++;
        while(i != _missing_ids.end())
        {
            LOG << ", " << *i ;
            i++;
        }
        LOG << std::endl;
    }
    
    if (!_wrong_id_pieces.empty())
    {
		LOG << "Puzzle of size " << _num_of_pieces << " cannot have the following IDs: ";
        std::list<int>::const_iterator i = _wrong_id_pieces.begin();
		LOG << *i;
        i++;
        while(i != _wrong_id_pieces.end())
        {
            LOG << ", " << *i ;
            i++;
        }
		LOG << std::endl;
    }

    if (!_wrong_format_pieces.empty())
    {
		for (std::list<std::pair<int, std::string>>::const_iterator i = _wrong_format_pieces.begin(); 
            i != _wrong_format_pieces.end(); 
            ++i)
    	{
			LOG << "Puzzle ID " << i->first << " has wrong data: ";
			LOG << i->second;
			LOG << std::endl;
		}
    }
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

void Puzzle::find_corners_candidates()
{
	for (const PiecePtr p : _puzzle_pieces)
	{
		if (p->get_top_side_shape() == straight)
		{
			if (p->get_left_side_shape() == straight)
			{
				_tl_corner_candids.insert(p->get_id());
			}
			if (p->get_right_side_shape() == straight)
			{
				_tr_corner_candids.insert(p->get_id());
			}
		}

		if (p->get_bottom_side_shape() == straight)
		{
			if (p->get_left_side_shape() == straight)
			{
				_bl_corner_candids.insert(p->get_id());
			}
			if (p->get_right_side_shape() == straight)
			{
				_br_corner_candids.insert(p->get_id());
			}
		}
		
	}
}