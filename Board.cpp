// Board.cpp
#include "Board.h"
#include "Piece.h"

#include <iostream>
#include <array>

Board::Board() {
    setupBoard();
}

std::array<std::array<Piece,8>, 8> Board::getBoard() {
    return myBoard;
}

void Board::setupBoard() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (row == 1) {
                Pawn myPawn('w');
                myBoard[row][col] = myPawn;
            } else if (row == 6) {
                Pawn myPawn('b');
                myBoard[row][col] = myPawn;
            } else if (row == 0) {
                if (col == 0 || col == 7) {
                    Rook myRook('w');
                    myBoard[row][col] = myRook;
                } else if (col == 1 || col == 6) {
                    Knight myKnight('w');
                    myBoard[row][col] = myKnight;
                } else if (col == 2 || col == 5) {
                    Bishop myBishop('w');
                    myBoard[row][col] = myBishop;
                } else if (col == 3) {
                    Queen myQueen('w');
                    myBoard[row][col] = myQueen;
                } else if (col == 4) {
                    King myKing('w');
                    myBoard[row][col] = myKing;
                }
            } else if (row == 7) {
                if (col == 0 || col == 7) {
                    Rook myRook('b');
                    myBoard[row][col] = myRook;
                } else if (col == 1 || col == 6) {
                    Knight myKnight('b');
                    myBoard[row][col] = myKnight;
                } else if (col == 2 || col == 5) {
                    Bishop myBishop('b');
                    myBoard[row][col] = myBishop;
                } else if (col == 3) {
                    Queen myQueen('b');
                    myBoard[row][col] = myQueen;
                } else if (col == 4) {
                    King myKing('b');
                    myBoard[row][col] = myKing;
                }
            }
        }
    }
}

Piece& Board::getPiece(int row, int col) {
    return myBoard[row][col];
}

void Board::setPiece(int row, int col, Piece replacement) {
    myBoard[row][col] = replacement;
}

void Board::printBoard() {
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            std::cout << myBoard[row][col].getSymbol();
        }
        std::cout << "\n";
    }
}
