#ifndef GROUPED_PIECE_LIST_H
#define GROUPED_PIECE_LIST_H

#include <iterator>
#include <list>

#include "Puzzle2dPiece.h"

class GroupPuzzlePiecesIterator
{
public:
    using value_type = Puzzle2dPiece;
    using difference_type = std::ptrdiff_t;
    using pointer = Puzzle2dPiecePtr*;
    using reference = Puzzle2dPiecePtr&;
    using iterator_category = std::forward_iterator_tag;
    reference operator*() const;

    GroupPuzzlePiecesIterator() { _lists = std::make_shared<std::list<Puzzle2dPieceListPtr>>(std::list<Puzzle2dPieceListPtr>()); };
    ~GroupPuzzlePiecesIterator() { };

    void concat_list(Puzzle2dPieceListPtr puzzle_piece_list_ptr);
    GroupPuzzlePiecesIterator begin();
    GroupPuzzlePiecesIterator end();
    GroupPuzzlePiecesIterator& operator++();
    GroupPuzzlePiecesIterator operator++(int);
    bool operator==(const GroupPuzzlePiecesIterator& other);
    bool operator!=(const GroupPuzzlePiecesIterator& other);

private:
    Puzzle2dPieceList::iterator _current_piece;
    std::list<Puzzle2dPieceListPtr>::iterator _current_list;
    std::shared_ptr<std::list<Puzzle2dPieceListPtr>> _lists;

};


#endif //GROUPED_PIECE_LIST_H