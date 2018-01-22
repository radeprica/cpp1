#include <iostream>
#include <limits> 

#include "GroupPuzzlePieces.h"

groupPuzzlePieces::groupPuzzlePieces(std::list<Puzzle2dPiece> puzzle_pieces)
{
    static constexpr unsigned int max_list_index = 2 * K + 1;

    unsigned int l = 0, t = 0, r = 0, b = 0;

    for (l = 0; l < max_list_index; l++)
    {
        for (t = 0; t < max_list_index; t++)
        {
            for (r = 0; r < max_list_index; r++)
            {
                for (b = 0; b < max_list_index; b++)
                {
                    _grouped_pieces[l][t][r][b] = std::make_shared<Puzzle2dPieceList>(Puzzle2dPieceList());
                }
            }
        }
    }

    for (Puzzle2dPiece piece : puzzle_pieces)
    {
        _insert_piece(std::make_shared<Puzzle2dPiece>(piece));
    }
}

void groupPuzzlePieces::_insert_piece(const Puzzle2dPiecePtr piece)
{
    _grouped_pieces[SHAPE_TO_PLACE(piece->get_left_side_shape())]
    [SHAPE_TO_PLACE(piece->get_top_side_shape())]
    [SHAPE_TO_PLACE(piece->get_right_side_shape())]
    [SHAPE_TO_PLACE(piece->get_bottom_side_shape())]->push_back(piece);
}

GroupPuzzlePiecesIterator groupPuzzlePieces::get(int left_cond, 
                                            int top_cond, 
                                            int right_cond, 
                                            int bottom_cond)
{
    GroupPuzzlePiecesIterator matching_pieces;
    unsigned int l = 0, t = 0, r = 0, b = 0;
    
    unsigned int min_left = SHAPE_TO_PLACE(left_cond);
    unsigned int max_left = SHAPE_TO_PLACE(left_cond) + 1;      
    unsigned int min_top = SHAPE_TO_PLACE(top_cond);
    unsigned int max_top = SHAPE_TO_PLACE(top_cond) + 1;        
    unsigned int min_right = SHAPE_TO_PLACE(right_cond);
    unsigned int max_right = SHAPE_TO_PLACE(right_cond) + 1;
    unsigned int min_bottom = SHAPE_TO_PLACE(bottom_cond);
    unsigned int max_bottom = SHAPE_TO_PLACE(bottom_cond) + 1;        
    
    if (left_cond == std::numeric_limits<int>::min())
    {
        min_left = 0;
        max_left = 2 * K + 1;
    }
    
    if (top_cond == std::numeric_limits<int>::min())
    {
        min_top = 0;
        max_top = 2 * K + 1;
    }
    
    if (right_cond == std::numeric_limits<int>::min())
    {
        min_right = 0;
        max_right = 2 * K + 1;
    }
    
    if (bottom_cond == std::numeric_limits<int>::min())
    {
        min_bottom = 0;
        max_bottom = 2 * K + 1;
    }
    
    for (l = min_left; l < max_left; l++)
    {
        for (t = min_top; t < max_top; t++)
        {
            for (r = min_right; r < max_right; r++)
            {
                for (b = min_bottom; b < max_bottom; b++)
                {
                    if (_grouped_pieces[l][t][r][b]->empty())
                    {
                        continue;
                    }
                    matching_pieces.concat_list(_grouped_pieces[l][t][r][b]);
                }
            }
        }
    }

    return matching_pieces;
}

void groupPuzzlePieces::print_me()
{
    int l = 0, t = 0, r = 0, b = 0;

    for (l = 0; l < 2*K+1; l++)
    {
        for (t = 0; t < 2*K+1; t++)
        {
            for (r = 0; r < 2*K+1; r++)
            {
                for (b = 0; b < 2*K+1; b++)
                {
                    std::cout << "[" << l - K << ", " << t - K << ", " << r - K << ", " << b - K << "]: ";
                    for (auto p : *(_grouped_pieces[l][t][r][b]))
                    {
                        std::cout << "{" << p->get_left_side_shape() << ", " << 
                         p->get_top_side_shape() << ", " << 
                          p->get_right_side_shape() << ", " << 
                           p->get_bottom_side_shape() << "}, ";
                    }
                    std::cout << std::endl;
                }
            }
        }
    }
}