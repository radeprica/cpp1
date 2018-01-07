#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cstring>
#include <cmath>

#include "Puzzle.h"
#include "Piece.h"
#include "Exceptions.h"
#include "Logger.h"

#define WORKER_SOLUTION_PIECE_IN_PLACE(worker_id, place) (_solution_by_worker.at(worker_id).at(place).first)
#define WORKER_SOLUTION_ROTATION_IN_PLACE(worker_id, place) (_solution_by_worker.at(worker_id).at(place).second)
#define STOP_IF_SOLVED if (_worker_solved >= 0)    \
                        {                           \
                            return false;           \
                        }

Puzzle::Puzzle(bool is_rotate, unsigned int num_of_threads) : _is_rotate(is_rotate) 
{ 
    _num_of_threads = num_of_threads == 1 ? 1 : num_of_threads - 1; 
    
    // resize all vectors according to number of threads
    _solution_by_worker.resize(_num_of_threads);
    _piece_organizer_of_worker.resize(_num_of_threads);
    _cur_row_size_of_worker.resize(_num_of_threads);
}

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

    _initialize_private_members();

    // Parse pieces from file
    while (std::getline(input_file, line))
    {
        _parse_piece_line(line);
    }
	
	input_file.close();

    // check and log initialization errors
    if(had_initialization_errors())
    {
        _log_initialization_errors();
        return;
    }
	
    // Compute all possible dimentions for the given num of pieces
    _find_possible_dimentions();
  
    // init is done
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

    if (_num_of_threads == 1)
    {
        _thread_solve(0);
    }
    
    else
    {
        for (unsigned int i = 0; i < _num_of_threads; i++)
        {
            _worker_threads.push_back(std::thread(&Puzzle::_thread_solve, this, i));
        }
        for (unsigned int i = 0; i < _num_of_threads; i++)
        {
            _worker_threads[i].join();
        }
    }

    if (_worker_solved >= 0)
    {
        _log_solution();
        return true;
    }

    LOG << no_solution_str << std::endl;
    return false;
}

bool Puzzle::had_initialization_errors()
{
    return (!_missing_ids.empty() ||
            ! _wrong_format_pieces.empty() ||
            !_wrong_id_pieces.empty());
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

    // parse piece id
    if (!(iss >> id))
    {
        throw PuzzleException("Wrong input format in line: %s", line.c_str());
    }

    // verify that piece id is legal
    if (id < 1 || id > _num_of_pieces)
    {
        _wrong_id_pieces.push_back(id);
        return;
    }

    _missing_ids.remove(id);

    // parse piece information and verify line structure
    if (!(iss >> left >> top >> right >> bottom) || !iss.eof())
    {
        std::pair<unsigned int, std::string> bad_line_pair(id, line);
        _wrong_format_pieces.push_back(bad_line_pair);
        return;
    }

    // verify that edge shapes have legal values
    if(left < -1 || left > 1 || 
        top < -1 || top > 1 || 
        right < -1 || right > 1 || 
        bottom < -1 || bottom > 1)
    {
        std::pair<unsigned int, std::string> bad_line_pair(id, line);
    _wrong_format_pieces.push_back(bad_line_pair); 
    }

    else
    {
        // store piece information
        _puzzle_pieces[id-1] = PiecePtr(new Piece(id,
                                            (PieceSideShape)left, 
                                            (PieceSideShape)top, 
                                            (PieceSideShape)right, 
                                            (PieceSideShape)bottom));
        for (unsigned int worker_id=0; worker_id < _num_of_threads; worker_id++)
        {
            PiecePtr worker_current_piece(new Piece(*(_puzzle_pieces[id-1])));
            (_piece_organizer_of_worker[worker_id])->insert_piece(worker_current_piece);
        }
    }
    
}

void Puzzle::_initialize_private_members()
{
    _puzzle_pieces.resize(_num_of_pieces);
    
    // resize all vectors according to number of pieces
    for (unsigned worker_id = 0; worker_id < _num_of_threads; worker_id++)
    {
        _solution_by_worker[worker_id] = std::vector<RotatedPiece>(_num_of_pieces);
        _piece_organizer_of_worker[worker_id] = PieceOrganizerPtr(new PieceOrganizer(_is_rotate));
    }

    // Starting with all pieces missing
    for (unsigned int i = 1; i <= _num_of_pieces; i++)
    {
        _missing_ids.push_back(i);
    }
}

void Puzzle::_find_possible_dimentions()
{
    unsigned int i = 0;

    for (i = 1; i <= static_cast<unsigned int>(sqrt(_num_of_pieces)); i++)
    {
        if(_num_of_pieces % i == 0)
        {
            std::pair<unsigned int, unsigned int> dimenstions(i, _num_of_pieces / i);
			_possible_dimentions.push_back(dimenstions);
            if ((_num_of_pieces / i) != i)
            {
                std::pair<unsigned int, unsigned int> dimenstions_reversed(_num_of_pieces / i, i);
                _possible_dimentions.push_back(dimenstions_reversed);
            }
        }
    }
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

    unsigned int tl_amount = _piece_organizer_of_worker[0]->get_piece_amount_by_conditions(straight, straight, any_shape, any_shape);
    unsigned int tr_amount = _piece_organizer_of_worker[0]->get_piece_amount_by_conditions(any_shape, straight, straight, any_shape);
    unsigned int bl_amount = _piece_organizer_of_worker[0]->get_piece_amount_by_conditions(straight, any_shape, any_shape, straight);
    unsigned int br_amount = _piece_organizer_of_worker[0]->get_piece_amount_by_conditions(any_shape, any_shape, straight, straight);
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

        if (_is_rotate)
        {
            unsigned int total_straights = num_left_straight + num_right_straight + num_top_straight + num_bottom_straight;
            if (total_straights >= (2*dim.first + 2*dim.second))
            {
                return false;
            }
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

void Puzzle::_thread_solve(unsigned int worker_id)
{
    int total_dims = _possible_dimentions.size();
    while (_worker_solved < 0)
    {
        int next_dim_in_line = ++_next_in_line;
        if (next_dim_in_line >= total_dims)
        {
            break;
        }
        
        // starting from more equal dimentions, because most puzzles are closer to a square shape then to a row shape
        std::pair<unsigned int, unsigned int> dim = _possible_dimentions.at(_possible_dimentions.size() - next_dim_in_line - 1);
        _cur_row_size_of_worker[worker_id] = dim.first;
        if (Puzzle::_try_solve(worker_id, 0, dim.first, dim.second))
        {
            _worker_solved = static_cast<int>(worker_id);
            break;
        }
    }
}

bool Puzzle::_try_solve(unsigned int worker_id, unsigned int k,unsigned int row_size, unsigned int column_size)
{
    STOP_IF_SOLVED;
    
    // get piece iterator matching the piece's location
	ConditionalPieceIteratorPtr it = _get_next_piece_conditional_iterator(worker_id, k, row_size, column_size);

    // if we are at the last piece
    if (k == _num_of_pieces - 1)
    {
        RotatedPiece candidate = it->get_and_mark_unavailable_next();
        // if there is no matching piece, we failed
        if (candidate.first.get() == nullptr)
        {
            return false;
        }
        
        _solution_by_worker[worker_id][k] = candidate;

        
        return true;
    }

    for (RotatedPiece candidate = it->get_and_mark_unavailable_next(); 
        candidate.first.get() != nullptr; 
        candidate = it->get_and_mark_unavailable_next())
    {
        STOP_IF_SOLVED;

        _solution_by_worker[worker_id][k] = candidate;
        // if we can solve with this piece in this location, we continue
        if (_try_solve(worker_id, k + 1, row_size, column_size))
        {
            return true;
        }
        // if we can't solve with this piece in this location, we put it back in the pool and try the next possibility
        it->mark_available(candidate);
    }
    
    // if no piece is good, we failed..
    return false;
}

ConditionalPieceIteratorPtr Puzzle::_get_next_piece_conditional_iterator(unsigned int worker_id, 
                                            unsigned int k,
                                            unsigned int row_size, 
                                            unsigned int column_size)
{
    unsigned int k_row =  static_cast<unsigned int>(k / row_size);
	unsigned int k_column = k % row_size;

    PieceSideShape required_left = any_shape;
    PieceSideShape required_top = any_shape;
    PieceSideShape required_right = any_shape;
    PieceSideShape required_bottom = any_shape;

    // we're building the solution from the top left, so there is always a demand on the left side of the piece we choose
    if (k_column == 0 || 
        WORKER_SOLUTION_PIECE_IN_PLACE(worker_id, k - 1)->get_right_side_shape(WORKER_SOLUTION_ROTATION_IN_PLACE(worker_id, k - 1)) == straight)
    {
        required_left = straight;
    }
    else if (WORKER_SOLUTION_PIECE_IN_PLACE(worker_id, k - 1)->get_right_side_shape(WORKER_SOLUTION_ROTATION_IN_PLACE(worker_id, k - 1)) == male)
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
    WORKER_SOLUTION_PIECE_IN_PLACE(worker_id, k - row_size)->get_bottom_side_shape(WORKER_SOLUTION_ROTATION_IN_PLACE(worker_id, k - row_size)) == straight))
    {
        required_top = straight;
    }
    else if (k >= row_size && 
    WORKER_SOLUTION_PIECE_IN_PLACE(worker_id, k - row_size)->get_bottom_side_shape(WORKER_SOLUTION_ROTATION_IN_PLACE(worker_id, k - row_size)) == male)
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

    ConditionalPieceIteratorPtr it = 
            std::make_shared<ConditionalPieceIterator>(_piece_organizer_of_worker[worker_id], 
                                                required_left,
                                                required_top,
                                                required_right,
                                                required_bottom);
    return it;
}

void Puzzle::_log_solution()
{
    if (_worker_solved < 0)
    {
        return;
    }

    unsigned int row_size = _cur_row_size_of_worker[_worker_solved];
    for (unsigned int i = 0; i < _num_of_pieces; i++)
    {
        if (i != 0 && i % row_size == 0)
        {
            LOG << std::endl;
        }
        if (_solution_by_worker[_worker_solved][i].second != no_rotation)
        {
            LOG << _solution_by_worker[_worker_solved][i].first->get_id() << '[' << _solution_by_worker[_worker_solved][i].second << "]";
        }
        else
        {
            LOG << _solution_by_worker[_worker_solved][i].first->get_id();
        }

        if (i % row_size != (row_size-1))
        {
            LOG << " ";
        }
    }
}