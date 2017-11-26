#include "Puzzle.h"
#include "Piece.h"
#include "Exceptions.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <cmath>


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

    if(had_initialization_errors())
    {
        log_initialization_errors();
        return;
    }
	// compute all possible dimentions for the given num of pieces
    find_possible_dimentions();

    _is_initialized = true;
}

bool Puzzle::solve()
{
    static const std::string no_solution_str("Cannot solve puzzle: it seems that there is no proper solution");
    
    if(!_is_initialized)
    {
        return false;
    }
    if (find_and_log_structure_errors())
    {
        return false;
    }

	/* solver initial data */

	// compute 2 boolean matrices of legal right matches & legal top matches
	find_all_possible_right_and_top_matches();

	_permutation = std::vector<unsigned int>(_num_of_pieces);
	
    for (std::pair<unsigned int, unsigned int> dim : _possible_dimentions)
    {
		for (unsigned int i = 0; i < _num_of_pieces; i++)
		{ _permutation[i]=i;}
        if (Puzzle::try_solve(0, dim.first, dim.second))
        {
            return true;
        }

		for (unsigned int i = 0; i < _num_of_pieces; i++)
		{ _permutation[i]=i;}
        if (Puzzle::try_solve(0, dim.second, dim.first))
        {
            return true;
        }
    }
    LOG << no_solution_str << std::endl;
    return false;
}

bool Puzzle::try_solve(unsigned int k,unsigned int row_size, unsigned int column_size)
{
	unsigned int k_row =  static_cast<unsigned int>(k/row_size);
	unsigned int k_column = k%row_size;

	for (unsigned int i = k; i < _num_of_pieces; i++)
	{
		unsigned int candidate_p = _permutation[i];

		/* left misgeret */
		if (k_column == 0)
		{
			if (_puzzle_pieces[candidate_p]->get_left_side_shape() != straight)
				continue;
		}
		/* right misgeret */
		if (k_column == (row_size-1))
		{
			if (_puzzle_pieces[candidate_p]->get_right_side_shape() != straight)
				continue;
		}
		/* top misgeret */
		if (k_row == 0)
		{
			if (_puzzle_pieces[candidate_p]->get_top_side_shape() != straight)
				continue;
		}
		/* bottom misgeret */
		if (k_row == (column_size - 1))
		{
			if (_puzzle_pieces[candidate_p]->get_bottom_side_shape() != straight)
				continue;
		}
		/* already has some piece on top of him */
		if (k_row > 0)
		{
			unsigned int top_of_candidate = _permutation[k-row_size];
			if (!_possible_top_matches[candidate_p][top_of_candidate])
			{
				continue;
			}
		}
		/* already has some piece on left of him */
		if (k_column > 0)
		{
			unsigned int left_of_candidate = _permutation[k-1];
			if (!_possible_right_matches[left_of_candidate][candidate_p])
			{
				continue;
			}
		}

		unsigned int temp = _permutation[k];
		_permutation[k] = candidate_p;
		_permutation[i] = temp;

        if (k == _num_of_pieces - 1)
        {
            for (unsigned int i = 0; i < _permutation.size(); i++)
            {
                if (i % row_size == 0)
                {
                    LOG << std::endl;
                }
                LOG << _permutation[i]+1 << ' ';
            }
            LOG << std::endl;
            return true;
        }
		
		if (Puzzle::try_solve(k+1, row_size, column_size))
        {
            return true;
        }
	
		temp = _permutation[i];
		_permutation[i] = _permutation[k];
		_permutation[k] = temp;
	}
    return false;
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
        std::pair<unsigned int, std::string> bad_line_pair(id, line);
        _wrong_format_pieces.push_back(bad_line_pair);
        return;
    }
    if(left < -1 || left > 1 || 
        top < -1 || top > 1 || 
        right < -1 || right > 1 || 
        bottom < -1 || bottom > 1)
        {
            std::pair<unsigned int, std::string> bad_line_pair(id, line);
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
		for (std::list<std::pair<unsigned int, std::string>>::const_iterator i = _wrong_format_pieces.begin(); 
            i != _wrong_format_pieces.end(); 
            ++i)
    	{
			LOG << "Puzzle ID " << i->first << " has wrong data: ";
			LOG << i->second;
			LOG << std::endl;
		}
    }
}

bool Puzzle::find_and_log_structure_errors()
{
    static const std::string num_edges_err_str("Cannot solve puzzle: wrong number of straight edges");
    static const std::string missing_corner_err_str("Cannot solve puzzle: missing corner element: ");
    static const std::string sum_edges_err_str("Cannot solve puzzle: sum of edges is not zero");

    bool ret = false;

    if (is_wrong_number_of_straight_edges())
    {
        LOG << num_edges_err_str << std::endl;
        ret = true;
    }

    find_corners_candidates();
    if (_tl_corner_candids.empty())
    {
        LOG << missing_corner_err_str << "TL" << std::endl;
        ret = true;
    }

    if (_tr_corner_candids.empty())
    {
        LOG << missing_corner_err_str << "TR" << std::endl;
        ret = true;
    }

    if (_bl_corner_candids.empty())
    {
        LOG << missing_corner_err_str << "BL" << std::endl;
        ret = true;
    }

    if (_br_corner_candids.empty())
    {
        LOG << missing_corner_err_str << "BR" << std::endl;
        ret = true;
    }

    if (is_sum_of_edges_not_zero())
    {
        LOG << sum_edges_err_str << std::endl;
        ret = true;
    }

    return ret;
}

void Puzzle::find_possible_dimentions()
{
    unsigned int i = 0;

    for (i = 1; i <= static_cast<unsigned int>(sqrt(_num_of_pieces)); i++)
    {
        if(_num_of_pieces % i == 0)
        {
            std::pair<unsigned int, unsigned int> dimenstions(i, _num_of_pieces / i);
			_possible_dimentions.push_front(dimenstions);
        }
    }
}

bool Puzzle::is_wrong_number_of_straight_edges()
{
    unsigned int num_left_straight = 0, num_top_straight = 0, num_right_straight = 0 , num_bottom_straight = 0;

    for (const PiecePtr p : _puzzle_pieces)
    {
        if (p->get_top_side_shape() == straight)
		{
            num_top_straight++;
        }
        if (p->get_left_side_shape() == straight)
        {
            num_left_straight++;
        }
        if (p->get_right_side_shape() == straight)
        {
            num_right_straight++;
        }
		if (p->get_bottom_side_shape() == straight)
		{
            num_bottom_straight++;
        }
    }

    for (const std::pair<unsigned int, unsigned int> dim : _possible_dimentions)
    {
        if (num_left_straight >= dim.first && num_right_straight >= dim.first &&
            num_top_straight >= dim.second && num_bottom_straight >= dim.second)
            {
                return false;
            }
        if (num_left_straight >= dim.second && num_right_straight >= dim.second &&
            num_top_straight >= dim.first && num_bottom_straight >= dim.first)
            {
                return false;
            }
    }
    return true;
}

bool Puzzle::is_sum_of_edges_not_zero()
{
    unsigned int sum = 0;

    for (const PiecePtr p : _puzzle_pieces)
    {
        sum += static_cast<int>(p->get_left_side_shape());
        sum += static_cast<int>(p->get_top_side_shape());
        sum += static_cast<int>(p->get_right_side_shape());
        sum += static_cast<int>(p->get_bottom_side_shape());
    }

    return (sum != 0);
}

void Puzzle::find_corners_candidates()
{
	for (const PiecePtr p : _puzzle_pieces)
	{
		if (p->is_tl_corner())
			_tl_corner_candids.insert(p->get_id());
		if (p->is_tr_corner())
			_tr_corner_candids.insert(p->get_id());
		if (p->is_bl_corner())
			_bl_corner_candids.insert(p->get_id());
		if (p->is_br_corner())
			_br_corner_candids.insert(p->get_id());
	}
}

/*
	compute 2 boolean matrices of legal right matches & legal top matches
	for all pairs of pieces,
	[x][y] = 1: piece id y is legal right puzzle connection to piece id x
	[x][y] = 0: else
	same logic for legal top
*/
void Puzzle::find_all_possible_right_and_top_matches()
{
	_possible_right_matches = std::vector<std::vector<bool>>(_num_of_pieces, std::vector<bool>(_num_of_pieces));
	_possible_top_matches = std::vector<std::vector<bool>>(_num_of_pieces, std::vector<bool>(_num_of_pieces));

	for (const PiecePtr p1 : _puzzle_pieces)
	{
		unsigned int p1_index = p1->get_id() - 1;
		PieceSideShape right = p1->get_right_side_shape();
		PieceSideShape top = p1->get_top_side_shape();

		for (const PiecePtr p2 : _puzzle_pieces)
		{
			unsigned int p2_index = p2->get_id() - 1;
			_possible_right_matches[p1_index][p2_index] = Piece::is_possible_edges_match(right, p2->get_left_side_shape());
			_possible_top_matches[p1_index][p2_index] = Piece::is_possible_edges_match(top, p2->get_bottom_side_shape());
		}
	}
}