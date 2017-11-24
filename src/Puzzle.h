#include <vector>
#include <list>
#include <string>
#include "Piece.h"

class Puzzle
{
public:
    Puzzle() { }
    ~Puzzle() { }

    void initialize_from_file(const std::string& input_path);
    const std::list<unsigned int> get_missing_pieces() { return _missing_ids; }
    const std::list<int> get_wrong_id_pieces() { return _wrong_id_pieces; }
    const std::list<std::pair<int, std::string>> get_wrong_format_pieces() { return _wrong_format_pieces; }
    int get_size() { return _num_of_pieces; }

    //TODO: remove this
    void print_pieces();

private:
    void parse_piece_line(const std::string& line);

private:
    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
    std::list<unsigned int> _missing_ids;
    std::list<int> _wrong_id_pieces;
    std::list<std::pair<int, std::string>> _wrong_format_pieces;
};