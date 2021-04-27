#include <iostream>
#include "Board.h"

int main(){
    ChessBoard* chessboard = new ChessBoard();
    chessboard->drawIntBoard();
    //chessboard->evaluation();
    //chessboard->generatePseudoLegalMoves();
    //chessboard->writeVector();
    //chessboard->testingFunction();
    //chessboard->generateRandomMove();
    char c;
    do{
        std::cout << "input:";
        std::cin >> c;
        std::cout << std::endl << std::endl;
        if(c == 'r'){
            chessboard->generateRandomMove();
            chessboard->drawIntBoard();
            chessboard->evaluation();
        }
        if(c == 't'){
            chessboard->TakeBack();
            chessboard->drawIntBoard();
            chessboard->evaluation();
        }
        if(c == 's'){
            chessboard->search();
            chessboard->drawIntBoard();
            chessboard->evaluation();
        }
    }while(c != 'x');

    delete chessboard;
    
    return 0;
}
