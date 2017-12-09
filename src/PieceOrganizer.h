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
    PieceOrganizer() { };
    ~PieceOrganizer() { };

    void insert_piece(const PiecePtr& piece);
    void print_me();

private:
    array<array<array<array<vector<RotatedPiece>, 3>, 3>, 3>, 3> _organized_pieces;

    friend class ConditionalPieceIterator;

};

typedef std::shared_ptr<PieceOrganizer> PieceOrganizerPtr;

#endif //PIECE_ORGANIZER_H