#ifndef GROUPED_PIECE_LIST_H
#define GROUPED_PIECE_LIST_H

#include <iterator>
#include <list>

#include "Puzzle2dPiece.h"

template<int K>
class GroupPuzzlePiecesIterator
{
public:
    // using value_type = Puzzle2dPiece<K>;
    // using difference_type = std::ptrdiff_t;
    // using pointer = typename Puzzle2dPiece<K>::Puzzle2dPiecePtr*;
    // using reference = typename Puzzle2dPiece<K>::Puzzle2dPiecePtr&;
    // using iterator_category = std::forward_iterator_tag;

    GroupPuzzlePiecesIterator() { _lists = std::make_shared<typename std::list
                        <typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr>>(
                            typename std::list<typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr>()); };
    ~GroupPuzzlePiecesIterator() { };

    void concat_list(typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr puzzle_piece_list_ptr)
    {
        _lists->push_back(puzzle_piece_list_ptr);
    }

    GroupPuzzlePiecesIterator begin()
    {
        if (_lists->empty())
        {
            return *this;
        }
        _current_list = _lists->begin();
        _current_piece = _lists->front()->begin();
        return *this;
    }

    GroupPuzzlePiecesIterator end()
    {
        GroupPuzzlePiecesIterator<K> tmp = *this;
        tmp._current_list = _lists->end();
        tmp._current_piece = tmp._lists->back()->end();
        return tmp;
    }

    GroupPuzzlePiecesIterator& operator++()
    {
        _current_piece++;
        if (_current_piece != (*_current_list)->end())
        {
            return *this;
        }
        _current_list++;
        if (_current_list == _lists->end())
        {
            return *this;
        }
        _current_piece = (*_current_list)->begin();
        return *this;
    }

    GroupPuzzlePiecesIterator operator++(int)
    {
        GroupPuzzlePiecesIterator<K> tmp = *this;
        _current_piece++;
        if (_current_piece != (*_current_list)->end())
        {
            return tmp;
        }
        _current_list++;
        if (_current_list == _lists->end())
        {
            return tmp;
        }
        _current_piece = (*_current_list)->begin();
        return tmp;
    }

    typename Puzzle2dPiece<K>::Puzzle2dPiecePtr& operator*() const
    {

        return *_current_piece;
    }


    bool operator==(const GroupPuzzlePiecesIterator& other)
    {
        if (_lists->empty() && other._lists->empty())
        {
            return true;
        }
        return (*_lists ==  *(other._lists)) && (_current_list == other._current_list) && (_current_piece == other._current_piece);
    }

    bool operator!=(const GroupPuzzlePiecesIterator& other)
    {
        if (_lists->empty() && other._lists->empty())
        {
            return false;
        }
        return (*_lists !=  *(other._lists)) || (_current_list != other._current_list) || (_current_piece != other._current_piece);
    }

private:
    typename Puzzle2dPiece<K>::Puzzle2dPieceList::iterator _current_piece;
    typename std::list<typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr>::iterator _current_list;
    std::shared_ptr<std::list<typename Puzzle2dPiece<K>::Puzzle2dPieceListPtr>> _lists;

};


#endif //GROUPED_PIECE_LIST_H