#include "Piece.h"

Piece::Piece(unsigned int id, 
		PieceSideShape left, 
		PieceSideShape top, 
		PieceSideShape right, 
		PieceSideShape bottom) : 
    _id(id), _left(left), _top(top), _right(right), _bottom(bottom) 
	{ }

	bool Piece::is_tl_corner()
	{
		if ((_top == straight) && (_left == straight))
			return true;
		return false;
	}

	bool Piece::is_tr_corner()
	{
		if ((_top == straight) && (_right == straight))
				return true;
		return false;
	}

	bool Piece::is_bl_corner()
	{
		if ((_bottom == straight) && (_left == straight))
			return true;
		return false;
	}

	bool Piece::is_br_corner()
	{
		if ((_bottom == straight) && (_right == straight))
			return true;
		return false;
	}

	bool Piece::is_possible_edges_match(PieceSideShape p1, PieceSideShape p2)
	{
		if (((p1 == straight) && (p2 == straight))  ||
			((p1 == male) && (p2 == female)) ||
			((p1 == female) && (p2 == male)))
		{
			return true;
		}
		return false;
	}
	