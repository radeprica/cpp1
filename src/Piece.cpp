#include "Piece.h"

Piece::Piece(unsigned int id, 
		PieceSideShape left, 
		PieceSideShape top, 
		PieceSideShape right, 
		PieceSideShape bottom) : 
    _id(id), _left(left), _top(top), _right(right), _bottom(bottom) 
	{ }

