#include <iostream>
#include "Board.h"

int main(){
    ChessBoard* chessboard = new ChessBoard();
    //chessboard->drawIntBoard();
    //chessboard->generatePseudoLegalMoves();
    //chessboard->writeVector();
    chessboard->testingFunction();
    //chessboard->generateRandomMove();
    /*char c;
    do{
        std::cout << "input:";
        std::cin >> c;
        std::cout << std::endl << std::endl;
        if(c == 'r'){
            chessboard->generateRandomMove();
            chessboard->drawIntBoard();
        }
    }while(c != 'x');*/
    return 0;
}
