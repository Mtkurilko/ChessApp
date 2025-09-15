#include "Game.h"
#include "Board.h"
#include "Piece.h"

#include <array>
#include <cctype>
#include <cmath>

Game::Game() {
    Board newBoard;
    
    theBoard = newBoard;
    
    myTurn = 'w';
}

Board Game::getBoard() {
    return theBoard;
}

char Game::getTurn() {
    return myTurn;
}

void Game::changeTurn() {
    if (getTurn() == 'w') {
        myTurn = 'b';
    } else {
        myTurn = 'w';
    }
}

bool Game::isKingInCheck(char color, Board isCheckedBoard) {
    char enemyColor; // Declare enemy color
    
    int kingPos[2] = {-1, -1}; // Declare king position
    
    int enemyPiecePositions[16][2]; // keep track of enemy position
    
    int enemyCount = 0; // size variable for the array of enemies
    
    if (color == 'w') {
        enemyColor = 'b';
    } else {
        enemyColor = 'w';
    }
    
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece curr; // mark the current piece
            
            curr = isCheckedBoard.getPiece(row, col); // grab the current piecer
            
            if (curr.getColor() == enemyColor) {
                if (enemyCount < 16) {
                    enemyPiecePositions[enemyCount][0] = row;
                    enemyPiecePositions[enemyCount][1] = col;
                    enemyCount++;
                }
            }
            
            if (color == 'w') {
                if (curr.getSymbol() == 'K') {
                    kingPos[0] = row;
                    kingPos[1] = col;
                }
            } else {
                if (curr.getSymbol() == 'k') {
                    kingPos[0] = row;
                    kingPos[1] = col;
                }
            }
        }
    }
    
    // If king not found, cannot be in check on an invalid board state
    if (kingPos[0] < 0 || kingPos[1] < 0) {
        return false;
    }

    for (int piece = 0; piece < enemyCount; piece++) {
        Piece curr; // Declare the current piece
        int row = enemyPiecePositions[piece][0];
        int col = enemyPiecePositions[piece][1];
        
        // Guard indices just in case
        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            continue;
        }
        curr = isCheckedBoard.getPiece(row, col);
        
        if (curr.isValidMove(row, col, kingPos[0], kingPos[1], &isCheckedBoard)) {
                if (!moveInterference(row, col, kingPos[0], kingPos[1], isCheckedBoard)) {
                    return true; // king is in check!
                }
        }
    }
    
    return false; // return false by default
}

bool Game::resolveCheck(int from_row, int from_col, int to_row, int to_col) {
    char color = theBoard.getPiece(from_row, from_col).getColor(); // for isKingInCheck method
    
    Board afterMoveBoard = theBoard; // set afterMoveBoard to start at
    
    // Move pieces on temp board
    afterMoveBoard.setPiece(to_row, to_col, afterMoveBoard.getPiece(from_row, from_col));
    Piece blank;
    afterMoveBoard.setPiece(from_row, from_col, blank);
    
    if (!isKingInCheck(color, afterMoveBoard)) {
        return true;
    }
    
    return false; // return false by default
}

bool Game::moveInterference(int from_row, int from_col, int to_row, int to_col, Board interferenceBoard) {
    char pieceType = std::toupper(theBoard.getPiece(from_row, from_col).getSymbol());
    
    if (pieceType == 'P') {
        // Check for piece in front if try to move forward
        if (std::abs(to_row-from_row) == 1 && to_col == from_col) {// Moving forward
            if (interferenceBoard.getPiece(to_row, to_col).getSymbol() != ' ') {
                return true; // There is a piece blocking for moving forward
            } else {
                return false; // strategically return good pawn pieces to better assess diagnol
            }
        }
        
        // Only left move diagnally left if there is a piece there
        if (interferenceBoard.getPiece(to_row, to_col).getSymbol() == ' ') {
            return true; // Can't move diagnally to empty square!
        }
    } else if (pieceType == 'B') {
        // Check diagnol blocking
        int row_diff = (to_row - from_row);
        int col_diff = (to_col - from_col);
        
        if (row_diff > 0) {// Up
            if (col_diff > 0) {// UP Right
                for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                    if (interferenceBoard.getPiece(from_row+step+1, from_col+step+1).getSymbol() != ' ') {
                        return true;
                    }
                }
            } else {// UP Left
                for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                    if (interferenceBoard.getPiece(from_row+step+1, from_col-step-1).getSymbol() != ' ') {
                        return true;
                    }
                }
            }
        } else {// Down
            if (col_diff > 0) {// DOWN Right
                for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                    if (interferenceBoard.getPiece(from_row-step+-1, from_col+step+1).getSymbol() != ' ') {
                        return true;
                    }
                }
            } else {// DOWN Left
                for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                    if (interferenceBoard.getPiece(from_row-step-1, from_col-step-1).getSymbol() != ' ') {
                        return true;
                    }
                }
            }
        }
        
    } else if (pieceType == 'R') {
        // Check straightfoward blocks
        if (pieceType == 'R' || pieceType == 'Q') {
            int row_step = (to_row > from_row) ? 1 : ((to_row < from_row) ? -1 : 0);
            int col_step = (to_col > from_col) ? 1 : ((to_col < from_col) ? -1 : 0);

            int curr_row = from_row + row_step;
            int curr_col = from_col + col_step;

            while (curr_row != to_row || curr_col != to_col) {
                if (interferenceBoard.getPiece(curr_row, curr_col).getSymbol() != ' ') {
                    return true; // There is a piece blocking the path.
                }
                curr_row += row_step;
                curr_col += col_step;
            }
        }
        
    } else if (pieceType == 'Q') {
        // Run B check and R check to make Q check
        int row_diff = (to_row - from_row);
        int col_diff = (to_col - from_col);
        
        if (pieceType == 'R' || pieceType == 'Q') {
            int row_step = (to_row > from_row) ? 1 : ((to_row < from_row) ? -1 : 0);
            int col_step = (to_col > from_col) ? 1 : ((to_col < from_col) ? -1 : 0);

            int curr_row = from_row + row_step;
            int curr_col = from_col + col_step;

            while (curr_row != to_row || curr_col != to_col) {
                if (interferenceBoard.getPiece(curr_row, curr_col).getSymbol() != ' ') {
                    return true; // There is a piece blocking the path.
                }
                curr_row += row_step;
                curr_col += col_step;
            }
        }
        
        // Check diagnol blocking
        
        if (row_diff == col_diff) { // Make sure they are envoking bishop movement
            if (row_diff > 0) {// Up
                if (col_diff > 0) {// UP Right
                    for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                        if (interferenceBoard.getPiece(from_row+step+1, from_col+step+1).getSymbol() != ' ') {
                            return true;
                        }
                    }
                } else {// UP Left
                    for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                        if (interferenceBoard.getPiece(from_row+step+1, from_col-step-1).getSymbol() != ' ') {
                            return true;
                        }
                    }
                }
            } else {// Down
                if (col_diff > 0) {// DOWN Right
                    for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                        if (interferenceBoard.getPiece(from_row-step+-1, from_col+step+1).getSymbol() != ' ') {
                            return true;
                        }
                    }
                } else {// DOWN Left
                    for (int step = 0; step < (std::abs(row_diff)-1); step++) {
                        if (interferenceBoard.getPiece(from_row-step-1, from_col-step-1).getSymbol() != ' ') {
                            return true;
                        }
                    }
                }
            }
        }
        
    }
    
    return false; // return false by default
}

bool Game::makeMove(int from_row, int from_col, int to_row, int to_col) {
    if (0 > to_row || to_row >= 8 || 0 > to_col || to_col >= 8) {// Check if move is in boundries
        return false;
    }
    
    // Declare the peice being moved
    Piece& movePiece = theBoard.getPiece(from_row,from_col);
    
    std::cout << "Attempting to move "<< movePiece.getSymbol() << std::endl;
    
    std::cout << "Attempting step 1 on "<< movePiece.getSymbol() << std::endl;
    if (movePiece.getSymbol() == ' ') {// Check if selected piece exists
        return false;
    }
    
    std::cout << "Attempting step 2 on "<< movePiece.getSymbol() << std::endl;
    if (!((getTurn() == 'w') == movePiece.isWhite())) {// Ensure turn
        return false;
    }
    
    std::cout << "Attempting step 3 on "<< movePiece.getSymbol() << std::endl;
    if (theBoard.getPiece(to_row, to_col).getSymbol() != ' ') {//Ensure that you can't move onto another one of your pieces
        
        if (theBoard.getPiece(to_row, to_col).getColor() == movePiece.getColor()) {
            return false;
        }
    }
    
    Board tempBoard = theBoard;
    char movePieceColor = theBoard.getPiece(from_row, from_col).getColor();
    
    // Temp move on the board
    tempBoard.setPiece(to_row, to_col, tempBoard.getPiece(from_row, from_col));
    Piece blank;
    tempBoard.setPiece(from_row, from_col, blank);
    
    std::cout << "Attempting step 4 on "<< movePiece.getSymbol() << std::endl;
    // Double pawn move
    if (std::toupper(theBoard.getPiece(from_row, from_col).getSymbol()) == 'P') {// Let pawns move to foward if on starting rank
        if (theBoard.getPiece(from_row, from_col).isWhite()) {
            if (isKingInCheck('w', tempBoard)) {
                return false; // No double movement if in check
            }
            if ((to_row-from_row) == 2 && (to_col == from_col) && from_row == 1) {
                // Check for interference
                if (theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece((to_row-1), to_col).getSymbol() == ' ') {
                    // Make move and change turn
                    theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                    Piece blank;
                    theBoard.setPiece(from_row, from_col, blank);
                    
                    changeTurn();
                    
                    return true; // return successful
                }
            }
        } else {
            if ((from_row-to_row) == 2 && (to_col == from_col) && from_row == 6) {
                if (isKingInCheck('b', tempBoard)) {
                    return false; // No double movement if in check
                }
                if (theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece((to_row+1), to_col).getSymbol() == ' ') {
                    // Make move and change turn
                    theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                    Piece blank;
                    theBoard.setPiece(from_row, from_col, blank);
                    
                    changeTurn();
                    
                    return true; // return successful
                }
            }
        }
    }
    
    std::cout << "Attempting step 5 on "<< movePiece.getSymbol() << std::endl;
    // Let King's Castle
    if (std::toupper(movePiece.getSymbol()) == 'K' && (to_col == 2 || to_col == 6)) {
        
        if (movePieceColor == 'w') {
            if (isKingInCheck('w', tempBoard)) {
                return false; // No movement if in check
            }
            
            Piece supposeKing = theBoard.getPiece(0,4);
            Piece supposeRook1 = theBoard.getPiece(0,0);
            Piece supposeRook2 = theBoard.getPiece(0,7);
            
            if (to_col == 2) {
                if (supposeKing.getSymbol() == 'K' && supposeRook1.getSymbol() == 'R') {
                    if (supposeKing.canCastle() && supposeRook1.canCastle() && theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece(to_row, to_col+1).getSymbol() == ' ') {
                        // Turn castling off because just castled
                        theBoard.getPiece(0,4).setCanCastle(false);
                        theBoard.getPiece(0,0).setCanCastle(false);
                        
                        // Castle
                        theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                        theBoard.setPiece(to_row, to_col+1, supposeRook2);
                        Piece blank;
                        theBoard.setPiece(from_row, from_col, blank);
                        theBoard.setPiece(0, 0, blank);
                        
                        changeTurn();
                        
                        return true; // return successful
                    }
                }
            } else {
                if (supposeKing.getSymbol() == 'K' && supposeRook2.getSymbol() == 'R') {
                    if (supposeKing.canCastle() && supposeRook2.canCastle() && theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece(to_row, to_col-1).getSymbol() == ' ') {
                        // Turn castling off because just castled
                        theBoard.getPiece(0,4).setCanCastle(false);
                        theBoard.getPiece(0,7).setCanCastle(false);
                        
                        // Castle
                        theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                        theBoard.setPiece(to_row, to_col-1, supposeRook2);
                        Piece blank;
                        theBoard.setPiece(from_row, from_col, blank);
                        theBoard.setPiece(0, 7, blank);
                        
                        changeTurn();
                        
                        return true; // return successful
                    }
                }
            }
                
            } else {
                if (isKingInCheck('b', tempBoard)) {
                    return false; // No movement if in check
                }
                
                Piece supposeKing = theBoard.getPiece(7,4);
                Piece supposeRook1 = theBoard.getPiece(7,0);
                Piece supposeRook2 = theBoard.getPiece(7,7);
                
                if (to_col == 2) {
                    if (supposeKing.getSymbol() == 'k' && supposeRook1.getSymbol() == 'r') {
                        if (supposeKing.canCastle() && supposeRook1.canCastle() && theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece(to_row, to_col+1).getSymbol() == ' ') {
                            // Turn castling off because just castled
                            theBoard.getPiece(7,4).setCanCastle(false);
                            theBoard.getPiece(7,0).setCanCastle(false);
                            
                            // Castle
                            theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                            theBoard.setPiece(to_row, to_col+1, supposeRook2);
                            Piece blank;
                            theBoard.setPiece(from_row, from_col, blank);
                            theBoard.setPiece(7, 0, blank);
                            
                            changeTurn();
                            
                            return true; // return successful
                        }
                    }
                } else {
                    if (supposeKing.getSymbol() == 'K' && supposeRook2.getSymbol() == 'R') {
                        if (supposeKing.canCastle() && supposeRook2.canCastle() && theBoard.getPiece(to_row, to_col).getSymbol() == ' ' && theBoard.getPiece(to_row, to_col-1).getSymbol() == ' ') {
                            // Turn castling off because just castled
                            theBoard.getPiece(7,4).setCanCastle(false);
                            theBoard.getPiece(7,7).setCanCastle(false);
                            
                            // Castle
                            theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
                            theBoard.setPiece(to_row, to_col-1, supposeRook2);
                            Piece blank;
                            theBoard.setPiece(from_row, from_col, blank);
                            theBoard.setPiece(7, 7, blank);
                            
                            changeTurn();
                            
                            return true; // return successful
                        }
                    }
                }
            }
    }
    
    std::cout << "Attempting step 6 on "<< movePiece.getSymbol() << std::endl;
    if (!movePiece.isValidMove(from_row, from_col, to_row, to_col, &theBoard)) {
        return false;
    }
    
    std::cout << "Attempting step 7 on "<< movePiece.getSymbol() << std::endl;
    if (moveInterference(from_row, from_col, to_row, to_col, theBoard)) {
        return false;
    }
    
    std::cout << "Attempting step 8 on "<< movePiece.getSymbol() << std::endl;
    if (isKingInCheck(movePieceColor, tempBoard)) {
            return false;
    }
    
    std::cout << "Moving "<< movePiece.getSymbol() << std::endl;
    
    // Check for upgrading pawn
    if (std::toupper(theBoard.getPiece(from_row, from_col).getSymbol()) == 'P') {
        if (movePieceColor == 'w') {
            if (to_row == 7) {
                Queen myQueen('w');
                theBoard.setPiece(to_row, to_col, myQueen);
                theBoard.setPiece(from_row, from_col, blank);
                
                changeTurn();
                
                return true;
            }
        } else {
            if (to_row == 0) {
                Queen myQueen('b');
                theBoard.setPiece(to_row, to_col, myQueen);
                theBoard.setPiece(from_row, from_col, blank);
                
                changeTurn();
                
                return true;
            }
        }
    }
    
    // If Rook or King set can castle to false
    if (std::toupper(movePiece.getSymbol() == 'K' || movePiece.getSymbol() == 'R')) {
        theBoard.getPiece(from_row, from_col).setCanCastle(false);
        
        std::cout << "Castle off" << std::endl;
    }
    
    // Make move and change turn
    theBoard.setPiece(to_row, to_col, theBoard.getPiece(from_row, from_col));
    theBoard.setPiece(from_row, from_col, blank);
    
    changeTurn();
    
    return true; // return successful
}
