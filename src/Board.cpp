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
        if(board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
            /// the move is pseudo-legal
            /// may be capture as well
        }
        if(board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
            /// the move is pseudo-legal
            /// may be capture as well
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(){
    int j;
    for(int i=0 ; i<4 ; ++i){
        j = 1;
        while(board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
            /// the move is pseudo-legal
            ++j;
        }
        if(board[whiteLeftRook + j * rookOffsets[i]] < 0)
            /// the last move is a capture || else doesn't matter, either white piece or off the table
        
        j = 1;
        while(board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
            /// the move is pseudo-legal
            j++;
        }
        if(board[whiteRightRook + j * rookOffsets[i]] < 0)
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}

void ChessBoard::whiteBishopMoveGeneration(){ // same as rook moves with different offsets
    int j;
    for(int i=0 ; i<4 ; ++i){
        j = 1;
        while(board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
            /// the move is pseudo-legal
            ++j;
        }
        if(board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
            /// the last move is a capture || else doesn't matter, either white piece or off the table
        
        j = 1;
        while(board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
            /// the move is pseudo-legal
            j++;
        }
        if(board[whiteRightBishop + j * bishopOffsets[i]] < 0)
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}