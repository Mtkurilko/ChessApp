// Peice.h
#ifndef PIECE_H
#define PIECE_H

class Piece {
public:
    Piece();
    char getSymbol();
    virtual bool isValidMove(int from_row, int from_col, int to_row, int to_col, class Board* board);
    bool isWhite();
    bool isBlack();
    char getColor();
    void setSymbol(char newValue);
    bool canCastle();
    void setCanCastle(bool input);
private:
    char mySymbol;
    bool myCastleAbility;
};

class Pawn : public Piece {
public:
    Pawn(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

class Knight : public Piece {
public:
    Knight(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

class Rook : public Piece {
public:
    Rook(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

class Queen : public Piece {
public:
    Queen(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

class Bishop : public Piece {
public:
    Bishop(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

class King : public Piece {
public:
    King(char type);
    bool isValidMove(int from_row, int from_col, int to_row, int to_col, Board* board) override;
};

#endif // PIECE_H
