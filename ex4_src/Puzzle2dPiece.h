#ifndef PUZZLE_2DPIECE_H
#define PUZZLE_2DPIECE_H

#include <memory>

class Puzzle2dPiece
{
public:
    Puzzle2dPiece(int left, int top, int right, int bottom) : _left(left), _top(top), _right(right), _bottom(bottom) { };
    ~Puzzle2dPiece() { };

    int get_left_side_shape() { return _left; };
    int get_top_side_shape() { return _top; };
    int get_right_side_shape() { return _right; };
    int get_bottom_side_shape() { return _bottom; };

private:
    const int _left;
    const int _top;
    const int _right;
    const int _bottom;
};

typedef std::shared_ptr<Puzzle2dPiece> Puzzle2dPiecePtr;
typedef std::list<Puzzle2dPiecePtr> Puzzle2dPieceList;
typedef std::shared_ptr<std::list<Puzzle2dPiecePtr>> Puzzle2dPieceListPtr;

#endif //PUZZLE_2DPIECE_H