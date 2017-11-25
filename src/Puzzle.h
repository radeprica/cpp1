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

    //TODO: remove this
    void print_pieces();

private:
    void parse_piece_line(const std::string& line);
	void find_corners_candidates();

private:
    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<int, std::string>> _wrong_format_pieces;

	std::set<unsigned int> _tl_corner_candids;
	std::set<unsigned int> _tr_corner_candids;
	std::set<unsigned int> _bl_corner_candids;
	std::set<unsigned int> _br_corner_candids;

private:
    Puzzle(const Puzzle&) = delete;
    Puzzle& operator=(const Puzzle&) = delete;

};