#ifndef PIECE_H
#define PIECE_H

class Piece
{

public:
	int id;
	int left;
	int top;
	int right;
	int bottom;
	Piece(int id, int left, int top, int right, int bottom);
};

#endif