#ifndef GROUPED_PUZZLE_PIECES_H
#define GROUPED_PUZZLE_PIECES_H

#include <array>
#include <list>

#include "Puzzle2dPiece.h"
#include "GroupPuzzlePiecesIterator.h"

#define K (1)
#define SHAPE_TO_PLACE(shape) ((shape) + K)

class groupPuzzlePieces
{
public:
    groupPuzzlePieces(std::list<Puzzle2dPiece> puzzle_pieces);
    ~groupPuzzlePieces() { };

    GroupPuzzlePiecesIterator get(int left_cond, 
                            int top_cond, 
                            int right_cond, 
                            int bottom_cond);
    void print_me();

private:
    void _insert_piece(const Puzzle2dPiecePtr piece);

private:
    std::array<std::array<std::array<std::array<Puzzle2dPieceListPtr, 2 * K + 1>, 2 * K + 1>, 2 * K + 1>, 2 * K + 1> _grouped_pieces;
};

#endif //GROUPED_PUZZLE_PIECES_H