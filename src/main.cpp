#include <iostream>
#include "Board.h"

int main(){
    ChessBoard* chessboard = new ChessBoard();
    //std::cout << convert_table_black[5] << std::endl;
    chessboard->drawIntBoard();
    chessboard->evaluation();
    //chessboard->generatePseudoLegalMoves();
    //chessboard->writeVector();
    //chessboard->testingFunction();
    //chessboard->generateRandomMove();
    move m;
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
    }while(c != 'x');
    return 0;
}
