#include "Rook.h"
#include "MoveException.h"

Rook::Rook(char col, const std::string& pos)
    : Piece(col, pos)
{
    if (pos.size() != 2 || pos[ROW] < START_OF_BOARD || pos[ROW] > CHESS_END_OF_BOARD || pos[COL] < START_OF_NUM_AS_CHAR || pos[COL] > CHESS_SIZE_AS_CHAR) {
        throw MoveException(MOVE_INVALID_OUT_OF_BOUNDS); // Invalid position indices
    }
}

/*
* Moves the piece across the board.
* Input: string reference new position.
* Output: none
*/
void Rook::move(const std::string& newPosition) {
    if (!canMove(newPosition)) {
        throw MoveException(MOVE_INVALID_ILLEGAL_PIECE_MOVE); // Illegal move for Rook
    }

    _position = newPosition;
}

/*
* Checks if moving is possible.
* Input: string reference new position.
* Output: true or false
*/
bool Rook::canMove(const std::string& newPosition) const {
    // Validate position format
    if (newPosition.size() != 2 || newPosition[ROW] < START_OF_BOARD || newPosition[ROW] > CHESS_END_OF_BOARD || newPosition[COL] < START_OF_NUM_AS_CHAR || newPosition[COL] > CHESS_SIZE_AS_CHAR)
    {
        throw MoveException(MOVE_INVALID_OUT_OF_BOUNDS); // Invalid position indices
    }

    char currentFile = _position[ROW];
    char currentRank = _position[COL];
    char newFile = newPosition[ROW];
    char newRank = newPosition[COL];

    // Rook can only move in straight lines: same file or same rank
    if (currentFile == newFile || currentRank == newRank) {
        return true;
    }

    return false;
}

std::string Rook::getType() const
{
    return std::string("Rook");
}
