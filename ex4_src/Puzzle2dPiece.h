#ifndef PUZZLE_2DPIECE_H
#define PUZZLE_2DPIECE_H

#include <memory>
#include <array>
#include <initializer_list>
#include <iostream>

#include "Exceptions.h"

template<int K>
class Puzzle2dPiece
{
public:
    Puzzle2dPiece(int left, int top, int right, int bottom) : _left(left), _top(top), _right(right), _bottom(bottom)
    { 
        if(left != std::numeric_limits<int>::min() && (left > k || left < -k))
        {
            throw PuzzleException("invalid type for left side of puzzle piece: got %d, expected value in range [%d, %d]", left, -k, k);
        }

        if(top != std::numeric_limits<int>::min() && (top > k || top < -k))
        {
            throw PuzzleException("invalid type for top side of puzzle piece: got %d, expected value in range [%d, %d]", top, -k, k);
        }

        if(right != std::numeric_limits<int>::min() && (right > k || right < -k))
        {
            throw PuzzleException("invalid type for right side of puzzle piece: got %d, expected value in range [%d, %d]", right, -k, k);
        }

        if(bottom != std::numeric_limits<int>::min() && (bottom > k || bottom < -k))
        {
            throw PuzzleException("invalid type for bottom side of puzzle piece: got %d, expected value in range [%d, %d]", bottom, -k, k);
        }
    };

    ~Puzzle2dPiece() { };

    int get_left_side_shape() const { return _left; };
    int get_top_side_shape() const { return _top; };
    int get_right_side_shape() const { return _right; };
    int get_bottom_side_shape() const { return _bottom; };

    static const int k = K;

typedef std::shared_ptr<Puzzle2dPiece<K>> Puzzle2dPiecePtr;
typedef std::list<Puzzle2dPiecePtr> Puzzle2dPieceList;
typedef std::shared_ptr<std::list<Puzzle2dPiecePtr>> Puzzle2dPieceListPtr;

private:
    const int _left;
    const int _top;
    const int _right;
    const int _bottom;
};

template<int K, template<int> class Puzzle2dPiece>
std::ostream& operator<<(std::ostream& os, const Puzzle2dPiece<K>& piece)  
{  
    os << "{" << piece.get_left_side_shape() << ", " << 
                        piece.get_top_side_shape() << ", " << 
                        piece.get_right_side_shape() << ", " << 
                        piece.get_bottom_side_shape() << "}";
    return os;  
} 

#endif //PUZZLE_2DPIECE_H