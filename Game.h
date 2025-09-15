// Game.h
#ifndef GAME_H
#define GAME_H

#include "Board.h"

class Game {
public:
    Game();
    Board getBoard();
    char getTurn();
    void changeTurn();
    bool makeMove(int from_row, int from_col, int to_row, int to_col);
    bool isKingInCheck(char color, Board isCheckedBoard);
    bool resolveCheck(int from_row, int from_col, int to_row, int to_col);
    bool moveInterference(int from_row, int from_col, int to_row, int to_col, Board interferenceBoard);
private:
    Board theBoard;
    char myTurn;
};

#endif // GAME_H
