#include <iostream>

#include "ConditionalPieceIterator.h"
#include "PieceOrganizer.h"

#define ORGANIZER_IN_CUR_LOCATION (_organizer->_organized_pieces[*_cur_left][*_cur_top][*_cur_right][*_cur_bottom])

ConditionalPieceIterator::ConditionalPieceIterator(const PieceOrganizerPtr organizer, 
                                                    PieceSideShape left, 
                                                    PieceSideShape top, 
                                                    PieceSideShape right, 
                                                    PieceSideShape bottom) : 
_left(left), _top(top), _right(right), _bottom(bottom), _organizer(organizer)
{
    _possible_lefts = (left != any_shape) ? list<int>({SHAPE_TO_PLACE(left)}) : list<int>({0, 1, 2});
    _possible_tops = (top != any_shape) ? list<int>({SHAPE_TO_PLACE(top)}) : list<int>({0, 1, 2});
    _possible_rights = (right != any_shape) ? list<int>({SHAPE_TO_PLACE(right)}) : list<int>({0, 1, 2});
    _possible_bottoms = (bottom != any_shape) ? list<int>({SHAPE_TO_PLACE(bottom)}) : list<int>({0, 1, 2});

    _cur_left = _possible_lefts.begin();
    _cur_top = _possible_tops.begin();
    _cur_right = _possible_rights.begin();
    _cur_bottom = _possible_bottoms.begin();
}

const RotatedPiece ConditionalPieceIterator::get_and_mark_unavailable_next()
{
    for( ; _cur_left != _possible_lefts.end(); _cur_left++)
    {
        for( ; _cur_top != _possible_tops.end(); _cur_top++)
        {
            for ( ; _cur_right != _possible_rights.end(); _cur_right++)
            {
                for ( ; _cur_bottom != _possible_bottoms.end(); _cur_bottom++)
                {
                    if (ORGANIZER_IN_CUR_LOCATION.empty())
                    {
                        continue;
                    }
                    // get an appropriate piece
                    for (unsigned int i = 0; 
                    i < ORGANIZER_IN_CUR_LOCATION.size() ;
                    i++)
                    { 
                        RotatedPiece cur_piece = ORGANIZER_IN_CUR_LOCATION[i];
                        if (cur_piece.first->is_available())
                        {
                            // std::cout << '[' << *_cur_left << "][" << *_cur_top << "][" << *_cur_right<< "][" << *_cur_bottom << "]: ";
                            cur_piece.first->set_is_available(false);
                            _cur_bottom++;
                            // std::cout << cur_piece.first->get_id() << '(' << cur_piece.second << ')' << std::endl;
                            return cur_piece; 
                        }
                    }
                }
                
                _cur_bottom = _possible_bottoms.begin();
            }
            _cur_right = _possible_rights.begin();
        }
        _cur_top = _possible_tops.begin();
    }

    return std::make_pair(PiecePtr(nullptr), no_rotation);
}

void ConditionalPieceIterator::mark_available(RotatedPiece rotated_piece)
{
    rotated_piece.first->set_is_available(true);
}