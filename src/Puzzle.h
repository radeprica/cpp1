#include <vector>
#include <list>
#include <string>
#include <set>
#include "Piece.h"

class Puzzle
{
public:
    Puzzle() { }
    ~Puzzle() { }

    void initialize_from_file(const std::string& input_path);
    bool had_initialization_errors();
    void log_initialization_errors();
    bool find_and_log_structure_errors();

    //TODO: remove this
    void print_pieces();
	std::vector<int> permutation;
private:
	void bla(int k, int row_size, int column_size);
    void parse_piece_line(const std::string& line);
	void find_corners_candidates();
    void find_possible_dimentions();
    bool is_wrong_number_of_straight_edges();
    bool is_sum_of_edges_not_zero();
	void find_all_possible_right_and_top_matches();
	
private:
    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<int, std::string>> _wrong_format_pieces;

    std::list<std::pair<unsigned int, unsigned int>> _possible_dimentions;

	std::set<unsigned int> _tl_corner_candids;
	std::set<unsigned int> _tr_corner_candids;
	std::set<unsigned int> _bl_corner_candids;
	std::set<unsigned int> _br_corner_candids;

	std::vector<std::vector<bool>> _possible_right_matches;
	std::vector<std::vector<bool>> _possible_top_matches;

private:
/* Puzzle(const Puzzle&) = delete;
   Puzzle& operator=(const Puzzle&) = delete; */
};