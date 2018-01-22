#include "GroupPuzzlePiecesIterator.h"

void GroupPuzzlePiecesIterator::concat_list(Puzzle2dPieceListPtr puzzle_piece_list_ptr)
{
    _lists->push_back(puzzle_piece_list_ptr);
}

GroupPuzzlePiecesIterator GroupPuzzlePiecesIterator::begin()
{
    _current_list = _lists->begin();
    _current_piece = _lists->front()->begin();
    return *this;
}

GroupPuzzlePiecesIterator GroupPuzzlePiecesIterator::end()
{
    GroupPuzzlePiecesIterator tmp = *this;
    tmp._current_list = _lists->end();
    tmp._current_piece = tmp._lists->back()->end();
    return tmp;
}

GroupPuzzlePiecesIterator& GroupPuzzlePiecesIterator::operator++()
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

GroupPuzzlePiecesIterator GroupPuzzlePiecesIterator::operator++(int)
{
    GroupPuzzlePiecesIterator tmp = *this;
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

Puzzle2dPiecePtr& GroupPuzzlePiecesIterator::operator*() const
{

    return *_current_piece;
}

bool GroupPuzzlePiecesIterator::operator==(const GroupPuzzlePiecesIterator& other)
{
    return (*_lists ==  *(other._lists)) && (_current_list == other._current_list) && (_current_piece == other._current_piece);
}

bool GroupPuzzlePiecesIterator::operator!=(const GroupPuzzlePiecesIterator& other)
{
    return (*_lists !=  *(other._lists)) || (_current_list != other._current_list) || (_current_piece != other._current_piece);
}