#include <vector>
#include <string>
#include "Piece.h"

class Puzzle
{
public:
    Puzzle() { }
    ~Puzzle() { }

    void initialize_from_file(const std::string& input_path);
    std::vector<unsigned int> get_missing_pieces();

    //TODO: remove this
    void print_pieces();

private:
    unsigned int _num_of_pieces;
    std::vector<PiecePtr> _puzzle_pieces;
};