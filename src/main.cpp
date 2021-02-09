#include <iostream>
#include "Board.h"

int main(){
    ChessBoard* chessboard = new ChessBoard();
    chessboard->drawIntBoard();
    chessboard->generatePseudoLegalMoves();
    return 0;
}