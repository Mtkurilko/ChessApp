// Board.h
#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <array>

#include "Piece.h"

class Board {
public:
    Board();
    void setupBoard();
    std::array<std::array<Piece,8>, 8> getBoard();
    Piece& getPiece(int row, int col);
    void setPiece(int row, int col, Piece replacement);
    void printBoard();
private:
    std::array<std::array<Piece,8>, 8> myBoard;
};

#endif // BOARD_H
