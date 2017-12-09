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
    Puzzle() : _piece_organizer(new PieceOrganizer()) { }
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
	void _find_corners_candidates();
    void _find_possible_dimentions();
    bool _is_wrong_number_of_straight_edges();
    bool _is_sum_of_edges_not_zero();
	void _find_all_possible_right_and_top_matches();
	
private:
    bool _is_initialized = false;

    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<unsigned int, std::string>> _wrong_format_pieces;
    std::vector<unsigned int> _permutation;

    std::list<std::pair<unsigned int, unsigned int>> _possible_dimentions;

    std::vector<RotatedPiece> _solution;

    PieceOrganizerPtr _piece_organizer;
	std::set<unsigned int> _tl_corner_candids;
	std::set<unsigned int> _tr_corner_candids;
	std::set<unsigned int> _bl_corner_candids;
	std::set<unsigned int> _br_corner_candids;

	std::vector<std::vector<bool>> _possible_right_matches;
	std::vector<std::vector<bool>> _possible_top_matches;

private:
	Puzzle(const Puzzle&) = delete;
	Puzzle& operator=(const Puzzle&) = delete;
};

#endif //PUZZLE_H