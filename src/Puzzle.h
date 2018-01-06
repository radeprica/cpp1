#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>
#include <list>
#include <string>
#include <set>
#include <atomic>
#include <thread>

#include "Piece.h"
#include "PieceOrganizer.h"
#include "ConditionalPieceIterator.h"

class Puzzle
{
public:
    Puzzle(bool is_rotate, unsigned int num_of_threads);
    ~Puzzle() { }

    void initialize_from_file(const std::string& input_path);
    bool had_initialization_errors();
    bool solve();
	
private:   
    // parsing and initialization functions
    int _parse_num_piece_line(const std::string& line);
    void _parse_piece_line(const std::string& line);
    void _initialize_private_members();
    void _find_possible_dimentions();

    // integrity verification and logging functions
    void _log_initialization_errors();
    bool _find_and_log_structure_errors();   
    bool _is_wrong_number_of_straight_edges();
    bool _is_sum_of_edges_not_zero();

    // solution functions
	void _thread_solve(unsigned int worker_id);
    bool _try_solve(unsigned int worker_id, unsigned int k, unsigned int row_size, unsigned int column_size);
    ConditionalPieceIteratorPtr _get_next_piece_conditional_iterator(unsigned int worker_id, 
                                            unsigned int k,
                                            unsigned int row_size, 
                                            unsigned int column_size);
    void _log_solution();

private:
    // common info
    bool _is_initialized = false;
    bool _is_rotate;
    unsigned int _num_of_threads;
    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::vector<std::pair<unsigned int, unsigned int>> _possible_dimentions;

    // structural integrity info
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<unsigned int, std::string>> _wrong_format_pieces;

    // woker related info
    std::vector<std::vector<RotatedPiece>> _solution_by_worker;
    std::vector<PieceOrganizerPtr> _piece_organizer_of_worker;
    std::vector<unsigned int> _cur_row_size_of_worker;
    std::vector<thread> _worker_threads;
    std::atomic<int> _next_in_line{-1};
    std::atomic<int> _worker_solved{-1};

private:
	Puzzle(const Puzzle&) = delete;
	Puzzle& operator=(const Puzzle&) = delete;
};

#endif //PUZZLE_H