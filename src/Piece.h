#ifndef PIECE_H
#define PIECE_H

#include <memory>

enum RotationDegrees
{
    no_rotation = 0,
    rotate_once = 90,
    rotate_twice = 180,
    rotate_thrice = 270,
};

enum PieceSideShape
{
    any_shape = -2,
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
    PieceSideShape get_left_side_shape(RotationDegrees deg = no_rotation);
    PieceSideShape get_top_side_shape(RotationDegrees deg = no_rotation);
    PieceSideShape get_right_side_shape(RotationDegrees deg = no_rotation);
    PieceSideShape get_bottom_side_shape(RotationDegrees deg = no_rotation);
	bool is_tl_corner();
	bool is_tr_corner();
	bool is_bl_corner();
	bool is_br_corner();
    bool is_available() { return _is_available; }
    void set_is_available(bool available) { _is_available = available; }

	static bool is_possible_edges_match(PieceSideShape p1, PieceSideShape p2);

private:
	const unsigned int _id;
    const PieceSideShape _left;
    const PieceSideShape _top;
    const PieceSideShape _right;
    const PieceSideShape _bottom;

    bool _is_available;

    friend class PieceOrganizer;
};

typedef std::shared_ptr<Piece> PiecePtr;

#endif //PIECE_H