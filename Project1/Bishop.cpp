#include "Bishop.h"
#include "MoveException.h"

#include <cmath> // For std::abs

Bishop::Bishop(char color, const std::string& position) : Piece(color, position)
{
}

bool Bishop::canMove(const std::string& newPosition) const
{
    int currentRow = _position[1] - '1'; // Convert '1'-'8' to 0-7
    int currentCol = _position[0] - 'a'; // Convert 'a'-'h' to 0-7
    int newRow = newPosition[1] - '1';
    int newCol = newPosition[0] - 'a';

    // Check for valid indices
    if (currentRow < 0 || currentRow >= 8 || currentCol < 0 || currentCol >= 8 ||
        newRow < 0 || newRow >= 8 || newCol < 0 || newCol >= 8) {
        throw MoveException::createException(5); // Invalid indices
    }

    // Check if the move is diagonal
    if (std::abs(newRow - currentRow) != std::abs(newCol - currentCol)) {
        throw MoveException::createException(6); // Illegal move for the piece
    }

    return true;
}

void Bishop::move(const std::string& newPosition)
{
    if (!canMove(newPosition)) {
        throw MoveException::createException(6); // Illegal move for the piece
    }

    // Additional checks for the move could go here, e.g., if the move causes a check
    setPosition(newPosition);
}
