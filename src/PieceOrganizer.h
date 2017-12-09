#ifndef PIECE_ORGANIZER_H
#define PIECE_ORGANIZER_H

#include <array>
#include <vector>
#include <list>

#include "Piece.h"

#define SHAPE_TO_PLACE(shape) ((shape) + 1)

using namespace std;

typedef pair<PiecePtr, RotationDegrees> RotatedPiece;

class PieceOrganizer
{
public:
    PieceOrganizer(bool is_rotate) : _is_rotate(is_rotate) { };
    ~PieceOrganizer() { };

    void insert_piece(const PiecePtr& piece);
    unsigned int get_piece_amount_by_conditions(PieceSideShape left_cond, 
                                                PieceSideShape top_cond, 
                                                PieceSideShape right_cond, 
                                                PieceSideShape bottom_cond);

    void print_me();

private:
    bool _is_rotate;
    array<array<array<array<vector<RotatedPiece>, 3>, 3>, 3>, 3> _organized_pieces;

    friend class ConditionalPieceIterator;

};

typedef std::shared_ptr<PieceOrganizer> PieceOrganizerPtr;

#endif //PIECE_ORGANIZER_H