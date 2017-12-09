#ifndef COND_PIECE_IT_H
#define COND_PIECE_IT_H

#include "PieceOrganizer.h"

class ConditionalPieceIterator
{
public:
    ConditionalPieceIterator(PieceOrganizerPtr organizer, PieceSideShape left, PieceSideShape top, PieceSideShape right, PieceSideShape bottom);
    ~ConditionalPieceIterator() { };

    const RotatedPiece get_and_mark_unavailable_next();
    void mark_available(RotatedPiece piece);

private:
    PieceSideShape _left;
    PieceSideShape _top;
    PieceSideShape _right;
    PieceSideShape _bottom;

    list<int> _possible_lefts;
    list<int> _possible_tops;
    list<int> _possible_rights;
    list<int> _possible_bottoms;

    list<int>::iterator _cur_left;
    list<int>::iterator _cur_top;
    list<int>::iterator _cur_right;
    list<int>::iterator _cur_bottom;

    PieceOrganizerPtr _organizer;
};

typedef std::shared_ptr<ConditionalPieceIterator> ConditionalPieceIteratorPtr;


#endif //COND_PIECE_IT_H