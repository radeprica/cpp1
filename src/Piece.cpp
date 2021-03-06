#include "Piece.h"

Piece::Piece(unsigned int id, 
		PieceSideShape left, 
		PieceSideShape top, 
		PieceSideShape right, 
		PieceSideShape bottom) : 
    _id(id), _left(left), _top(top), _right(right), _bottom(bottom), _is_available(true)
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


PieceSideShape Piece::get_left_side_shape(RotationDegrees deg)
{
	switch (deg)
	{
	case no_rotation:
		return _left;
	case rotate_once:
		return _bottom;
	case rotate_twice:
		return _right;
	default:
		return _top;
	}
}

PieceSideShape Piece::get_top_side_shape(RotationDegrees deg)
{
	switch (deg)
	{
	case no_rotation:
		return _top;
	case rotate_once:
		return _left;
	case rotate_twice:
		return _bottom;
	default:
		return _right;
	}
}

PieceSideShape Piece::get_right_side_shape(RotationDegrees deg)
{
	switch (deg)
	{

	case no_rotation:
		return _right;
	case rotate_once:
		return _top;
	case rotate_twice:
		return _left;
	default:
		return _bottom;
	}
}

PieceSideShape Piece::get_bottom_side_shape(RotationDegrees deg)
{
	switch (deg)
	{
	case no_rotation:
		return _bottom;
	case rotate_once:
		return _right;
	case rotate_twice:
		return _top;
	default:
		return _left;
	}
}