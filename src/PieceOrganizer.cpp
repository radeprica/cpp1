#include <iostream>

#include "PieceOrganizer.h"

void PieceOrganizer::insert_piece(const PiecePtr& piece)
{
    _organized_pieces[SHAPE_TO_PLACE(piece->_left)]
    [SHAPE_TO_PLACE(piece->_top)]
    [SHAPE_TO_PLACE(piece->_right)]
    [SHAPE_TO_PLACE(piece->_bottom)].push_back(RotatedPiece(piece, no_rotation));

    if (!_is_rotate ||
    (piece->_left == piece->_top && piece->_top == piece->_right && piece->_right == piece->_bottom))
    {
        return;
    }

    _organized_pieces[SHAPE_TO_PLACE(piece->_bottom)]
    [SHAPE_TO_PLACE(piece->_left)]
    [SHAPE_TO_PLACE(piece->_top)]
    [SHAPE_TO_PLACE(piece->_right)].push_back(RotatedPiece(piece, rotate_once));

    _organized_pieces[SHAPE_TO_PLACE(piece->_right)]
    [SHAPE_TO_PLACE(piece->_bottom)]
    [SHAPE_TO_PLACE(piece->_left)]
    [SHAPE_TO_PLACE(piece->_top)].push_back(RotatedPiece(piece, rotate_twice));

    _organized_pieces[SHAPE_TO_PLACE(piece->_top)]
    [SHAPE_TO_PLACE(piece->_right)]
    [SHAPE_TO_PLACE(piece->_bottom)]
    [SHAPE_TO_PLACE(piece->_left)].push_back(RotatedPiece(piece, rotate_thrice));

}

unsigned int PieceOrganizer::get_piece_amount_by_conditions(PieceSideShape left_cond, 
                                                PieceSideShape top_cond, 
                                                PieceSideShape right_cond, 
                                                PieceSideShape bottom_cond)
{
    list<int> possible_lefts = (left_cond != any_shape) ? list<int>({SHAPE_TO_PLACE(left_cond)}) : list<int>({0, 1, 2});
    list<int> possible_tops = (top_cond != any_shape) ? list<int>({SHAPE_TO_PLACE(top_cond)}) : list<int>({0, 1, 2});
    list<int> possible_rights = (right_cond != any_shape) ? list<int>({SHAPE_TO_PLACE(right_cond)}) : list<int>({0, 1, 2});
    list<int> possible_bottoms = (bottom_cond != any_shape) ? list<int>({SHAPE_TO_PLACE(bottom_cond)}) : list<int>({0, 1, 2});
    
    unsigned int total_size = 0;

    for(auto cur_left = possible_lefts.begin() ; cur_left != possible_lefts.end(); cur_left++)
    {
        for(auto cur_top = possible_tops.begin() ; cur_top != possible_tops.end(); cur_top++)
        {
            for (auto cur_right = possible_rights.begin() ; cur_right != possible_rights.end(); cur_right++)
            {
                for (auto cur_bottom = possible_bottoms.begin() ; cur_bottom != possible_bottoms.end(); cur_bottom++)
                {
                    total_size += _organized_pieces[*cur_left][*cur_top][*cur_right][*cur_bottom].size();
                }
            }
        }
    }
    return total_size;
}

void PieceOrganizer::print_me()
{
    vector<RotatedPiece> current_vec;
    int i = -1, j = -1, k = -1, s = -1;
    
    for (i = -1; i < 2; i++)
    {
        for (j = -1; j < 2; j++)
        {
            for (k = -1; k < 2; k++)
            {
                for (s = -1; s < 2; s++)
                {
                    current_vec = _organized_pieces[SHAPE_TO_PLACE(i)][SHAPE_TO_PLACE(j)][SHAPE_TO_PLACE(k)][SHAPE_TO_PLACE(s)];
                    if (current_vec.empty())
                    {
                        continue;
                    }
                    std::cout << '[' << i << ']' << '[' << j << ']' <<'[' << k << ']' << '[' << s << "]: ";
                    
                    for (unsigned int l = 0; l < current_vec.size(); l++)
                    {
                        std::cout << current_vec.at(l).first->get_id() << '(' << current_vec.at(l).second << ')';
                        if (l != current_vec.size())
                        {
                            std::cout << ", ";
                        }
                    }
                    std::cout << std::endl;
                }
            }
        }
    } 
}