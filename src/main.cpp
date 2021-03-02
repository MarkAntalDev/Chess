#include <iostream>
#include "Board.h"

int main(){
    using namespace std::chrono;
    ChessBoard* chessboard = new ChessBoard();
    chessboard->drawIntBoard();
    //chessboard->generatePseudoLegalMoves();
    //chessboard->writeVector();
    //chessboard->testingFunction();
    chessboard->generateRandomMove();
    return 0;
}
