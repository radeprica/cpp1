#ifndef PIECE_H
#define PIECE_H

#include <memory>

enum PieceSideShape
{
    female = -1,
    straight = 0,
    male = 1,
};

class Piece
{
public:
    Piece(unsigned int id, PieceSideShape left, PieceSideShape top, PieceSideShape right, PieceSideShape bottom);
    ~Piece() { };

    unsigned int get_id() { return _id; }
    PieceSideShape get_left_side_shape() { return _left; }
    PieceSideShape get_top_side_shape() { return _top; }
    PieceSideShape get_right_side_shape() { return _right; }
    PieceSideShape get_bottom_side_shape() { return _bottom; }

private:
	const unsigned int _id;
    const PieceSideShape _left;
    const PieceSideShape _top;
    const PieceSideShape _right;
    const PieceSideShape _bottom;
};

typedef std::shared_ptr<Piece> PiecePtr;

#endif //PIECE_H