#ifndef GROUPED_PUZZLE_PIECES_H
#define GROUPED_PUZZLE_PIECES_H

#include <array>
#include <list>
#include <memory>

#include "Puzzle2dPiece.h"
#include "GroupPuzzlePiecesIterator.h"

template<int K>
class GroupPuzzlePieces
{
public:
    GroupPuzzlePieces(typename std::list<Puzzle2dPiece<K>>::iterator begin, typename std::list<Puzzle2dPiece<K>>::iterator end)
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
                        _grouped_pieces[l][t][r][b] = std::make_shared<typename Puzzle2dPiece<K>::Puzzle2dPieceList>(
                            typename Puzzle2dPiece<K>::Puzzle2dPieceList());
                    }
                }
            }
        }

        for (auto it = begin; it != end; it++)
        {
            _insert_piece(std::make_shared<Puzzle2dPiece<K>>(*it));
        }
    }

    ~GroupPuzzlePieces() { };

    // TODO: make get recieve a puzzle piece or other structure that can be made by initialization
    GroupPuzzlePiecesIterator<K> get(int left_cond, 
                            int top_cond, 
                            int right_cond, 
                            int bottom_cond)
    {
        GroupPuzzlePiecesIterator<K> matching_pieces;
        unsigned int l = 0, t = 0, r = 0, b = 0;
        
        unsigned int min_left = shape_to_place(left_cond);
        unsigned int max_left = shape_to_place(left_cond) + 1;      
        unsigned int min_top = shape_to_place(top_cond);
        unsigned int max_top = shape_to_place(top_cond) + 1;        
        unsigned int min_right = shape_to_place(right_cond);
        unsigned int max_right = shape_to_place(right_cond) + 1;
        unsigned int min_bottom = shape_to_place(bottom_cond);
        unsigned int max_bottom = shape_to_place(bottom_cond) + 1;        
        
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

                            
    void print_me()
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

private:
    int shape_to_place(int shape) { return shape + K; }
    void _insert_piece(const typename Puzzle2dPiece<K>::Puzzle2dPiecePtr piece)
    {
        _grouped_pieces[shape_to_place(piece->get_left_side_shape())]
        [shape_to_place(piece->get_top_side_shape())]
        [shape_to_place(piece->get_right_side_shape())]
        [shape_to_place(piece->get_bottom_side_shape())]->push_back(piece);
    }

private:
    std::array<std::array<std::array<std::array<typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr, 2 * K + 1>, 2 * K + 1>, 2 * K + 1>, 2 * K + 1> _grouped_pieces;
};

template<class Iterator, int T = std::iterator_traits<Iterator>::value_type::k>
GroupPuzzlePieces<T> groupPuzzlePieces(Iterator begin, Iterator end)
{
    return GroupPuzzlePieces<T>(begin, end);
}

#endif //GROUPED_PUZZLE_PIECES_H