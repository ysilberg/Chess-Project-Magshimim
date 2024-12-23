#include "Manager.h"
#include "Board.h"
#include "pieces.h"
#include <iostream>

#include "MoveException.h"
#define START_OF_ABC 'a'
#define START_OF_NUM 1


Manager::Manager() : isWhiteTurn(true) 
{}

Manager::~Manager() {}



bool Manager::validateMove(Piece* piece, std::string& newPosition)
{
    return piece->canMove(newPosition);
}

void Manager::movePiece(Piece* piece, std::string& newPosition)
{
    piece->move(newPosition);
}

bool Manager::isCheck()
{
    //return king.isAttacked();
    return false;
}

void Manager::resetGame()
{
    startGame();
}

bool Manager::isGameOver()
{
    //return king.isAttacked() && !king.canMove();
    return false;
}

void Manager::displayBoard(Board _chessBoard)
{
  auto& chess = _chessBoard.getBoard(); 

  std::cout << "  a b c d e f g h" << std::endl; 
  for (int row = CHESS_SIZE - 1; row >= 0; --row)
  {
      std::cout << row + 1 << " ";
      for (int col = 0; col < CHESS_SIZE; ++col)
      {
          if (chess[row][col] != nullptr) {
              std::cout << chess[row][col] << " ";
          }
          else {
              std::cout << ". ";
          }
      }
      std::cout << row + 1 << std::endl;
  }
}

void Manager::gameLoop(Board& board)
{
    Piece* p;
    
    
    
    
    
    std::string position = p->getPosition();


    
    while (isGameOver() == false)
    {

        Piece* selectedPiece = board.getSymbol(position); 
        if (selectedPiece == nullptr) {
            throw std::invalid_argument("No piece at the selected position");
        }

        if ((isWhiteTurn && selectedPiece->getColor() != 'w') ||
            (!isWhiteTurn && selectedPiece->getColor() != 'b')) {
            throw std::invalid_argument("It's not your turn");
        }

        std::string msgFromGraphics = pipe.getMessageFromGraphics();
        std::string newPosition = msgFromGraphics.substr(2, 2);
        if (!validateMove(selectedPiece, newPosition)) {
            throw std::invalid_argument("Invalid move for the selected piece");
        }
        
        movePiece(selectedPiece, newPosition);
        

        if (isCheck() == true)
        {
            throw MoveException::createException(3);
            //  only king can move
        }
        
    }
}

