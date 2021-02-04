#include "Board.h"

ChessBoard::ChessBoard(){

}

//drawing the chessboard with ints
void ChessBoard::drawIntBoard(){
    for(int i = 119; i>=0 ; --i){
        if ((i + 1) %10 == 0)
            std::cout << std::endl << std::endl;

        if(int(board[i])<0)
            std::cout << " " << int(board[i]);
        else
            std::cout << "  " << int(board[i]);
    }
}

/*************************************************************************************************
; 
; WHITE move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::whiteKnightMoveGeneration(){
    for(int i=0 ; i<8 ; ++i){
        if(board[whiteLeftKnight] + knightMoveConstants[i] < 1){
            /// the move is legal 
        }
    }
}