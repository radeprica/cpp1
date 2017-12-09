#include <iostream>

#include "PieceOrganizer.h"

void PieceOrganizer::insert_piece(const PiecePtr& piece)
{
    _organized_pieces[SHAPE_TO_PLACE(piece->_left)]
    [SHAPE_TO_PLACE(piece->_top)]
    [SHAPE_TO_PLACE(piece->_right)]
    [SHAPE_TO_PLACE(piece->_bottom)].push_back(RotatedPiece(piece, no_rotation));

    if (piece->_left == piece->_top && piece->_top == piece->_right && piece->_right == piece->_bottom)
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