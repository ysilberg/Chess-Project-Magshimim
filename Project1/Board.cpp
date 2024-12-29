#include "Board.h"
#include <cctype>
#include <stdexcept>

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pwn.h"
#include "Queen.h"
#include "Rook.h"
#include "Piece.h"

#define START_OF_BOARD 'a'
#define START_OF_NUM 1
#define WHITE 'w'
#define BLACK 'b'

Board::Board(const std::string& boardData) : _board(CHESS_SIZE, std::vector<Piece*>(CHESS_SIZE, nullptr))
{
    _whiteTurn = boardData[64] == '0';
    setBoard(boardData); // Delegate to setBoard for parsing and initialization
}

Board::~Board() {
    for (int row = 0; row < CHESS_SIZE; ++row) {
        for (int col = 0; col < CHESS_SIZE; ++col) {
            delete _board[row][col]; 
        }
    }
}


const std::vector<std::vector<Piece*>>& Board::getBoard() const {
    return _board;
}

/*
* Gets the piece symbol form position.
* Input: string reference pos.
* Output: position 
*/
Piece* Board::getSymbol(std::string& pos) const {
    if (pos.size() != 2 || pos[0] < 'a' || pos[0] > 'h' || pos[1] < '1' || pos[1] > '8') {
        throw std::runtime_error("Invalid chessboard position format.");
    }
    auto col = pos[0] - 'a'; // Column is derived from the file (letter)
    auto row = '8' - pos[1]; // Row is derived from the rank (number)
    return _board[row][col];
}


/*
* Sets the game board.
* Input: string reference to boardData.
* Output: none
*/
void Board::setBoard(const std::string& boardData)
{
    if (boardData.size() != CHESS_SIZE * CHESS_SIZE +1)
    {
        throw std::runtime_error("Invalid board data size. Expected 64 characters.");
    }
    for (auto row = 0; row < CHESS_SIZE; ++row)
    {
        for (auto col = 0; col < CHESS_SIZE; ++col)
        {
            delete _board[row][col];
            _board[row][col] = nullptr;
            std::string pos = std::string(1, START_OF_BOARD + col) + std::to_string(row + START_OF_NUM);
            char pieceChar = boardData[row * CHESS_SIZE + col];
            char color = WHITE;
            if (std::isupper(pieceChar))
            {
                color = BLACK;
            }
	        switch (std::tolower(pieceChar))
	        {
	        case ROOK:
                _board[row][col] = new Rook(color,pos);
                break;
	        case PWN:
                _board[row][col] = new Pwn(color,pos);
                break;
	        case KING:
                _board[row][col] = new King(color, pos);
                break;
	        case QUEEN:
                _board[row][col] = new Queen(color, pos);
                break;
	        case KNIGHT:
                _board[row][col] = new Knight(color, pos);
                break;
            case BISHOP:
                _board[row][col] = new Bishop(color, pos);
                break;
	        case EMPTY:
                _board[row][col] = nullptr;
                break;
	        default:
                throw std::runtime_error("Invalid piece character '" + std::string(1, pieceChar) + "' at position: " + pos);
	        }
        }
    }
}


/*
* To string function.
* Input: none.
* Output: boardString
*/
std::string Board::toString() const {
    std::string boardString;
    boardString.reserve(CHESS_SIZE * (CHESS_SIZE + 1)); // Reserve space for all rows + newlines

    for (size_t row = 0; row < _board.size(); ++row) {
        for (size_t col = 0; col < _board[row].size(); ++col) {
            if (_board[row][col] == nullptr) {
                boardString += '#';  // Empty square
            }
            else {
                std::string type = _board[row][col]->getType();
                char pieceType = type[0];
                if (_board[row][col]->getColor() == WHITE) {
                    boardString += pieceType;  // Uppercase for white
                }
                else {
                    boardString += std::tolower(pieceType); // Lowercase for black
                }
            }
        }
    }
    if (_whiteTurn)
    {
        boardString += "0";
    }
    else
    {
        boardString += "1";
    }
    return boardString;
}




/*
* Moves the piece across the board.
* Input: string reference from , string reference to.
* Output: none
*/
void Board::movePiece(const std::string& from, const std::string& to) {
    // Convert "from" and "to" to row and column indices
    int fromRow = from[1] - '1';
    int fromCol = from[0] - 'a';
    int toRow = to[1] - '1';
    int toCol = to[0] - 'a';

    // Validate bounds
    if (fromRow < 0 || fromRow >= CHESS_SIZE || fromCol < 0 || fromCol >= CHESS_SIZE ||
        toRow < 0 || toRow >= CHESS_SIZE || toCol < 0 || toCol >= CHESS_SIZE) {
        throw std::out_of_range("Move out of bounds.");
    }

    // Get the piece at the "from" position
    Piece* piece = _board[fromRow][fromCol];
    if (piece == nullptr) {
        throw std::invalid_argument("No piece at the source position.");
    }

    // Get the type and color of the piece
    std::string pieceType = piece->getType();
    std::string toPosition = std::string(1, 'a' + toCol) + std::to_string(toRow + 1);

    // Check if the move is valid for the piece
    if (!piece->canMove(toPosition)) {
        throw std::invalid_argument("Invalid move for the selected piece.");
    }

    // Path validation for applicable pieces
    if (pieceType == "Rook" || pieceType == "Bishop" || pieceType == "Queen") {
        if (!isPathClear(fromRow, fromCol, toRow, toCol, pieceType)) {
            throw std::invalid_argument("Path is blocked.");
        }
    }

    // Handle capturing
    Piece* targetPiece = _board[toRow][toCol];
    if (targetPiece != nullptr) {
        if (targetPiece->getColor() == piece->getColor()) {
            throw std::invalid_argument("Cannot capture your own piece.");
        }
        delete targetPiece; // Remove the captured piece
    }

    // Perform the move
    _board[toRow][toCol] = piece;
    _board[fromRow][fromCol] = nullptr;
    piece->setPosition(toPosition);
}


/*
* Checks if the path is clear by checking for direction, obstacles according to different pieces.
* Input: int fromRow, int fromCol, int toRow, int toCol, string reference pieceType.
* Output: true or false
*/
bool Board::isPathClear(const int fromRow,const int fromCol,const int toRow,const int toCol, const std::string& pieceType) const {
    int rowDiff = toRow - fromRow;
    int colDiff = toCol - fromCol;

    if (pieceType == "Rook") {
        // Rook moves must be straight: either rowDiff or colDiff is 0
        if (rowDiff != 0 && colDiff != 0)
        {
            return false;
        }

        // Determine direction of movement
        int rowDirection = (rowDiff == 0) ? 0 : rowDiff / abs(rowDiff);
        int colDirection = (colDiff == 0) ? 0 : colDiff / abs(colDiff);

        // Check path for obstacles
        int currentRow = fromRow + rowDirection;
        int currentCol = fromCol + colDirection;
        while (currentRow != toRow || currentCol != toCol) {
            if (_board[currentRow][currentCol] != nullptr) {
                return false; // Path is blocked
            }
            currentRow += rowDirection;
            currentCol += colDirection;
        }
        return true; // Path is clear
    }
    else if (pieceType == "Bishop") {
        // Bishop moves must be diagonal: abs(rowDiff) == abs(colDiff)
        if (abs(rowDiff) != abs(colDiff)) return false;

        // Determine direction of movement
        int rowDirection = rowDiff / abs(rowDiff);
        int colDirection = colDiff / abs(colDiff);

        // Check path for obstacles
        int currentRow = fromRow + rowDirection;
        int currentCol = fromCol + colDirection;
        while (currentRow != toRow || currentCol != toCol) {
            if (_board[currentRow][currentCol] != nullptr) {
                return false; // Path is blocked
            }
            currentRow += rowDirection;
            currentCol += colDirection;
        }
        return true; // Path is clear
    }
    else if (pieceType == "Queen") {
        // Queen moves must be straight or diagonal
        if (fromRow == toRow || fromCol == toCol) {
            // Straight-line move (like Rook)
            int rowDirection = (rowDiff == 0) ? 0 : rowDiff / abs(rowDiff);
            int colDirection = (colDiff == 0) ? 0 : colDiff / abs(colDiff);

            int currentRow = fromRow + rowDirection;
            int currentCol = fromCol + colDirection;
            while (currentRow != toRow || currentCol != toCol) {
                if (_board[currentRow][currentCol] != nullptr) {
                    return false; // Path is blocked
                }
                currentRow += rowDirection;
                currentCol += colDirection;
            }
            return true; // Path is clear
        }
        else if (abs(rowDiff) == abs(colDiff)) {
            // Diagonal move (like Bishop)
            int rowDirection = rowDiff / abs(rowDiff);
            int colDirection = colDiff / abs(colDiff);

            int currentRow = fromRow + rowDirection;
            int currentCol = fromCol + colDirection;
            while (currentRow != toRow || currentCol != toCol) {
                if (_board[currentRow][currentCol] != nullptr) {
                    return false; // Path is blocked
                }
                currentRow += rowDirection;
                currentCol += colDirection;
            }
            return true; // Path is clear
        }
        return false; // Invalid move for Queen
    }
    else if (pieceType == "Knight"|| pieceType == "Pawn")
    {
        return true;
    }
    return false; // Invalid piece type or unsupported path validation
}

