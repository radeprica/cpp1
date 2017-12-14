#ifndef PUZZLE_H
#define PUZZLE_H

#include <vector>
#include <list>
#include <string>
#include <set>

#include "Piece.h"
#include "PieceOrganizer.h"

class Puzzle
{
public:
    Puzzle(bool is_rotate) : _piece_organizer(new PieceOrganizer(is_rotate)), _is_rotate(is_rotate) { }
    ~Puzzle() { }

    void initialize_from_file(const std::string& input_path);
    bool had_initialization_errors();
    bool solve();
	
private:
    int _parse_num_piece_line(const std::string& line);
    void _log_initialization_errors();
    bool _find_and_log_structure_errors();
	bool _try_solve(unsigned int k, unsigned int row_size, unsigned int column_size);
    void _parse_piece_line(const std::string& line);
    void _find_possible_dimentions();
    bool _is_wrong_number_of_straight_edges();
    bool _is_sum_of_edges_not_zero();
	
private:
    bool _is_initialized = false;

    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<unsigned int, std::string>> _wrong_format_pieces;

    std::list<std::pair<unsigned int, unsigned int>> _possible_dimentions;

    std::vector<RotatedPiece> _solution;
    PieceOrganizerPtr _piece_organizer;

private:
	Puzzle(const Puzzle&) = delete;
	Puzzle& operator=(const Puzzle&) = delete;
};

#endif //PUZZLE_H