// Piece.cpp
#include "Piece.h"
#include "Board.h"

#include <iostream>
#include <cctype>
#include <string>
#include <cmath>

Piece::Piece() : mySymbol(' '), myCastleAbility(false) {
    
}

void Piece::setSymbol(char newValue) {
    mySymbol = newValue;
    
    if (std::toupper(newValue) == 'K' || std::toupper(newValue) == 'R') {
        myCastleAbility = true;
    }
}

bool Piece::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    // Reject no-op
    if (from_row == to_row && from_col == to_col) {
        return false;
    }
    int dr = to_row - from_row;
    int dc = to_col - from_col;
    int adr = std::abs(dr);
    int adc = std::abs(dc);

    char t = std::toupper(getSymbol());
    switch (t) {
        case 'P': {
            int dir = (getColor() == 'w') ? 1 : -1;
            // Allow 1-step forward or 1-step diagonal in the correct direction.
            if (dr == dir && (dc == 0 || adc == 1)) {
                return true;
            }
            return false;
        }
        case 'N':
            return (adr == 2 && adc == 1) || (adr == 1 && adc == 2);
        case 'B':
            return adr == adc;
        case 'R':
            return (dr == 0) || (dc == 0);
        case 'Q':
            return (adr == adc) || (dr == 0) || (dc == 0);
        case 'K':
            return adr <= 1 && adc <= 1;
        default:
            return false;
    }
}

char Piece::getSymbol() {
    return mySymbol;
}

bool Piece::isWhite() {
    return isupper(getSymbol());
}

bool Piece::isBlack() {
    return islower(getSymbol());
}

char Piece::getColor() {
    if (isWhite()) {
        return 'w';
    } else {
        return 'b';
    }
}

void Piece::setCanCastle(bool input) {
    myCastleAbility = input;
}

bool Piece::canCastle() {
    return myCastleAbility;
}

Pawn::Pawn(char type) {
    if (type == 'w') {
        setSymbol('P');
    } else if (type == 'b') {
        setSymbol('p');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool Pawn::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) {
    if (from_row == to_row && from_col == to_col) {
        return false;
    }
    Piece& dest = board->getPiece(to_row, to_col);
    int rowDir = isWhite() ? 1 : -1;
    // Forward move
    if (to_col == from_col && (to_row - from_row) == rowDir) {
        if (dest.getSymbol() == ' ') {
            return true;
        }
    }
    // Diagonal capture
    if (std::abs(to_col - from_col) == 1 && (to_row - from_row) == rowDir) {
        if (dest.getSymbol() != ' ' && dest.getColor() != getColor()) {
            return true;
        }
    }
    return false;
}

Knight::Knight(char type) {
    if (type == 'w') {
        setSymbol('N');
    } else if (type == 'b') {
        setSymbol('n');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool Knight::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    if (from_row == to_row && from_col == to_col) {
        return false; // Cannot be move to the same place (edge case)
    }
    
    // Knight movement
    if (abs(to_row-from_row) == 3) {
        if (abs(to_col-from_col) == 2) {
            return true;
        }
    } else if (abs(to_col-from_col) == 3) {
        if (abs(to_row-from_row) == 2) {
            return true;
        }
    }
    
    return false; // temp
}

Bishop::Bishop(char type) {
    if (type == 'w') {
        setSymbol('B');
    } else if (type == 'b') {
        setSymbol('b');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool Bishop::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    if (from_row == to_row && from_col == to_col) {
        return false; // Cannot be move to the same place (edge case)
    }
    
    if (abs(from_row-to_row) == abs(from_col-to_col)) {
        return true; // Only allows diagnol movement
    }
    
    return false; // temp
}

Rook::Rook(char type) {
    if (type == 'w') {
        setSymbol('R');
    } else if (type == 'b') {
        setSymbol('r');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool Rook::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    if (from_row == to_row && from_col == to_col) {
        return false; // Cannot be move to the same place (edge case)
    }
    
    if ((to_row - from_row) == 0 || (to_col - from_col) == 0) {
        return true; // Should allow Rook to move if it's in a straight line
    }
    
    return false; // temp
}

King::King(char type) {
    if (type == 'w') {
        setSymbol('K');
    } else if (type == 'b') {
        setSymbol('k');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool King::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    if (from_row == to_row && from_col == to_col) {
        return false; // Cannot be move to the same place (edge case)
    }
    
    double distance =  sqrt(pow(to_row-from_row, 2) + pow((to_col-from_col), 2)); // calculates distance
    
    if (distance < 2) {
        return true; // let the king only move one square
    }
    
    return false; // temp
}

Queen::Queen(char type) {
    if (type == 'w') {
        setSymbol('Q');
    } else if (type == 'b') {
        setSymbol('q');
    } else {
        std::cout << "Invalid Piece color" << std::endl;
    }
}

bool Queen::isValidMove(int from_row, int from_col, int to_row, int to_col, Board* /*board*/) {
    if (from_row == to_row && from_col == to_col) {
        return false; // Cannot be move to the same place (edge case)
    }
    
    if (abs(from_row-to_row) == abs(from_col-to_col)) {
        return true; // Bishop movement
    }
    
    if ((to_row - from_row) == 0 || (to_col - from_col) == 0) {
        return true; // Rook movement
    }
    
    return false; // temp
}
