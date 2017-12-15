#include "Puzzle.h"
#include "Piece.h"
#include "Exceptions.h"
#include "Logger.h"
#include "ConditionalPieceIterator.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <cmath>


#define SOLUTION_PIECE_IN_PLACE(place) (_solution.at(place).first)
#define SOLUTION_ROTATION_IN_PLACE(place) (_solution.at(place).second)

void Puzzle::initialize_from_file(const std::string& input_path)
{
    // Open input file
    std::ifstream input_file(input_path);
    if (input_file.fail())
    {
        throw PuzzleException(strerror(errno));
    }

    std::string line;

    // Parse first line
    std::getline(input_file, line);
    _num_of_pieces = _parse_num_piece_line(line);
    _puzzle_pieces = std::vector<PiecePtr>(_num_of_pieces);

    // Starting with all pieces missing
    for (unsigned int i = 1; i <= _num_of_pieces; i++)
    {
        _missing_ids.push_back(i);
    }

    // Parse pieces from file
    while (std::getline(input_file, line))
    {
        _parse_piece_line(line);
    }
	
	input_file.close();

    // 
    if(had_initialization_errors())
    {
        _log_initialization_errors();
        return;
    }
	
    // Compute all possible dimentions for the given num of pieces
    _find_possible_dimentions();

    //TODO: delete this
    //_piece_organizer->print_me();
    
    _solution.resize(_num_of_pieces);
    _is_initialized = true;
}

bool Puzzle::solve()
{
    static const std::string no_solution_str("Cannot solve puzzle: it seems that there is no proper solution");
    
    if(!_is_initialized)
    {
        return false;
    }
    if (_find_and_log_structure_errors())
    {
        return false;
    }

    for (std::pair<unsigned int, unsigned int> dim : _possible_dimentions)
    {
        if (Puzzle::_try_solve(0, dim.first, dim.second))
        {
            return true;
        }

        if (Puzzle::_try_solve(0, dim.second, dim.first))
        {
            return true;
        }
    }

    LOG << no_solution_str << std::endl;
    return false;
}

int Puzzle::_parse_num_piece_line(const std::string& line)
{
    static const std::string num_elements_str("NumElements");
    static const char delim = '=';

    std::string key;
    unsigned int value;

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
    return value;
}

void Puzzle::_parse_piece_line(const std::string& line)
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
    _piece_organizer->insert_piece(_puzzle_pieces[id-1]);

}

bool Puzzle::_try_solve(unsigned int k,unsigned int row_size, unsigned int column_size)
{
	unsigned int k_row =  static_cast<unsigned int>(k/row_size);
	unsigned int k_column = k%row_size;

    PieceSideShape required_left = any_shape;
    PieceSideShape required_top = any_shape;
    PieceSideShape required_right = any_shape;
    PieceSideShape required_bottom = any_shape;

    // we're building the solution from the top left, so there is always a demand on the left side of the piece we choose
    if (k_column == 0 || 
        SOLUTION_PIECE_IN_PLACE(k - 1)->get_right_side_shape(SOLUTION_ROTATION_IN_PLACE(k - 1)) == straight)
    {
        required_left = straight;
    }
    else if (SOLUTION_PIECE_IN_PLACE(k - 1)->get_right_side_shape(SOLUTION_ROTATION_IN_PLACE(k - 1)) == male)
    {
        required_left = female;
    }
    else
    {
        required_left = male;
    }

    // we're building the solution from the top left, so there is always a demand on the top side of the piece we choose
    if (k_row == 0 || 
    (k >= row_size && 
    SOLUTION_PIECE_IN_PLACE(k - row_size)->get_bottom_side_shape(SOLUTION_ROTATION_IN_PLACE(k - row_size)) == straight))
    {
        required_top = straight;
    }
    else if (k >= row_size && 
    SOLUTION_PIECE_IN_PLACE(k - row_size)->get_bottom_side_shape(SOLUTION_ROTATION_IN_PLACE(k - row_size)) == male)
    {
        required_top = female;
    }
    else
    {
        required_top = male;
    }

    // if we are at the right edge of the puzzle, demand stragiht right edge in the piece
    if (k_column == (row_size - 1))
    {
        required_right = straight;
    }

    // if we are at the bottom of the puzzle, demand stragiht bottom edge in the piece
    if (k_row == (column_size - 1))
    {
        required_bottom = straight;
    }

    ConditionalPieceIterator it(_piece_organizer, 
                                required_left,
                                required_top,
                                required_right,
                                required_bottom);

    // if we are at the last piece
    if (k == _num_of_pieces - 1)
    {
        RotatedPiece candidate = it.get_and_mark_unavailable_next();
        // if there is no matching piece, we failed
        if (candidate.first.get() == nullptr)
        {
            return false;
        }
        
        _solution[k] = candidate;
        // TODO: function print solution
        for (unsigned int i = 0; i < _num_of_pieces; i++)
        {
            if (i != 0 && i % row_size == 0)
            {
                LOG << std::endl;
            }
            if (_solution[i].second != no_rotation)
            {
                LOG << _solution[i].first->get_id() << '[' << _solution[i].second << "] ";
            }
            else
            {
                LOG << _solution[i].first->get_id() << " ";
            }
            
        }
        LOG << std::endl;
        return true;
    }

    for (RotatedPiece candidate = it.get_and_mark_unavailable_next(); 
        candidate.first.get() != nullptr; 
        candidate = it.get_and_mark_unavailable_next())
    {
        _solution[k] = candidate;
        // if we can solve with this piece in this location, we continue
        if (_try_solve(k + 1, row_size, column_size))
        {
            return true;
        }
        // if we can't solve with this piece in this location, we put it back in the pool and try the next possibility
        it.mark_available(candidate);
        // std::cout << "back" << std::endl;
    }
    
    // if no piece is good, we failed..
    return false;
}

bool Puzzle::had_initialization_errors()
{
    return (!_missing_ids.empty() ||
            ! _wrong_format_pieces.empty() ||
            !_wrong_id_pieces.empty());
}

void Puzzle::_log_initialization_errors()
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

bool Puzzle::_find_and_log_structure_errors()
{
    static const std::string num_edges_err_str("Cannot solve puzzle: wrong number of straight edges");
    static const std::string missing_corner_err_str("Cannot solve puzzle: missing corner element: ");
    static const std::string sum_edges_err_str("Cannot solve puzzle: sum of edges is not zero");

    bool ret = false;

    if (_is_wrong_number_of_straight_edges())
    {
        LOG << num_edges_err_str << std::endl;
        ret = true;
    }

    unsigned int tl_amount = _piece_organizer->get_piece_amount_by_conditions(straight, straight, any_shape, any_shape);
    unsigned int tr_amount = _piece_organizer->get_piece_amount_by_conditions(any_shape, straight, straight, any_shape);
    unsigned int bl_amount = _piece_organizer->get_piece_amount_by_conditions(straight, any_shape, any_shape, straight);
    unsigned int br_amount = _piece_organizer->get_piece_amount_by_conditions(any_shape, any_shape, straight, straight);
    unsigned int total_corners = tl_amount + tr_amount + bl_amount + br_amount;

    if (tl_amount == 0)
    {
        if (!_is_rotate || (total_corners < 4))
        {
            LOG << missing_corner_err_str << "TL" << std::endl;
            ret = true;
        }
    }

    if (tr_amount == 0)
    {
        if (!_is_rotate || (total_corners < 4))
        {
            LOG << missing_corner_err_str << "TR" << std::endl;
            ret = true;
        }
    }

    if (bl_amount == 0)
    {
        if (!_is_rotate || (total_corners < 4))
        {
            LOG << missing_corner_err_str << "BL" << std::endl;
            ret = true;
        }
    }

    if (br_amount == 0)
    {
        if (!_is_rotate || (total_corners < 4))
        {
            LOG << missing_corner_err_str << "BR" << std::endl;
            ret = true;
        }
    }

    if (_is_sum_of_edges_not_zero())
    {
        LOG << sum_edges_err_str << std::endl;
        ret = true;
    }

    return ret;
}

void Puzzle::_find_possible_dimentions()
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

bool Puzzle::_is_wrong_number_of_straight_edges()
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

        if (_is_rotate)
        {
            /* TODO: delete this comment FFS so "flip" is "reflect" and not 180 rotation?
                more speciefic calculations (aka taking each piece has more than 1 straight edge only once and checking all those combinations)
                has huge complexity
            */
            unsigned int total_straights = num_left_straight + num_right_straight + num_top_straight + num_bottom_straight;
            if (total_straights >= (2*dim.first + 2*dim.second))
            {
                return false;
            }

            /* more "general" check, more strict is similiar to solving/some NP problem */
            /*
            unsigned int bank_for_lefts = num_left_straight + num_top_straight + num_bottom_straight;
            unsigned int bank_for_rights = num_right_straight + num_top_straight + num_bottom_straight;
            unsigned int bank_for_tops = num_top_straight + num_left_straight + num_right_straight;
            unsigned int bank_for_bottoms = num_bottom_straight + num_left_straight + num_right_straight;

            unsigned int total_straights = num_left_straight + num_right_straight + num_top_straight + num_bottom_straight;

            if ((bank_for_lefts >= dim.first) && (bank_for_rights >= dim.first) && (bank_for_bottoms >= dim.second)
                && (bank_for_tops >= dim.second) && (total_straights >= (2*dim.first + 2*dim.second)))
            {
                return false;
            }
            if ((bank_for_lefts >= dim.second) && (bank_for_rights >= dim.second) && (bank_for_bottoms >= dim.first)
                && (bank_for_tops >= dim.first) && (total_straights >= (2*dim.first + 2*dim.second)))
            {
                return false;
            }
            */


            /*
            int left_needed = dim.first - num_left_straight;
            int right_needed = dim.first - num_right_straight;
            int top_needed = dim.second - num_top_straight;
            int bottom_needed = dim.second - num_bottom_straight;

            int top_bottom_bank = std::max(-1*top_needed, 0) + std::max(-1*bottom_needed, 0);
            int left_right_bank = std::max(-1*left_needed, 0) + std::max(-1*right_needed, 0);

            int left_right_needed = std::max(left_needed, 0) + std::max(right_needed, 0);
            int top_bottom_needed = std::max(top_needed, 0) + std::max(bottom_needed, 0);

            if ((top_bottom_bank - left_right_needed >= 0) && (left_right_bank - top_bottom_needed >= 0))
            {
                return false;
            }

            left_needed = dim.second - num_left_straight;
            right_needed = dim.second - num_right_straight;
            top_needed = dim.first - num_top_straight;
            bottom_needed = dim.first - num_bottom_straight;

            top_bottom_bank = std::max(-1*top_needed, 0) + std::max(-1*bottom_needed, 0);
            left_right_bank = std::max(-1*left_needed, 0) + std::max(-1*right_needed, 0);

            left_right_needed = std::max(left_needed, 0) + std::max(right_needed, 0);
            top_bottom_needed = std::max(top_needed, 0) + std::max(bottom_needed, 0);

            if ((top_bottom_bank - left_right_needed >= 0) && (left_right_bank - top_bottom_needed >= 0))
            {
                return false;
            }
            */
        }
    }
    
    return true;
}

bool Puzzle::_is_sum_of_edges_not_zero()
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