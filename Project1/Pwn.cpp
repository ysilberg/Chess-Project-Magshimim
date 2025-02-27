#include "Pwn.h"
#define TYPE "Pwn"


//c'tor
Pwn::Pwn(char col, const std::string& pos) : Piece(col,pos), _firstMove(true)
{}



/*
* Moves the piece across the board.
* Input: string reference new position.
* Output: none
*/
void Pwn::move(const std::string & newPosition)
{
    if (canMove(newPosition)) 
    {
        setPosition(newPosition);
        _firstMove = false;
    }
    else 
    {
        throw MoveException(MOVE_INVALID_ILLEGAL_PIECE_MOVE);
    }
}

/*
* Checks if moving is possible.
* Input: string reference new position.
* Output: true or false
*/
bool Pwn::canMove(const std::string& newPosition) const
{
    int direction = (_color == 'w') ? 1 : -1;
    char newCol = newPosition[0];
    int newRow = newPosition[1] - '0';
    char currCol = _position[0];
    int currRow = _position[1]-'0';
    bool firstVAL = newCol == currCol || newPosition[2] == EAT;
    bool secondVal = newRow == currRow + direction;
    bool theredVal = newRow == currRow + 2 * direction;
    return (firstVAL && (secondVal || (_firstMove && theredVal)));
}

/*
* func return type of piece
* input:none
* output:string type
*/
std::string Pwn::getType() const
{
    return std::string(TYPE);
}

