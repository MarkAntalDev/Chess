#include "Board.h"

ChessBoard::ChessBoard(){

}

//drawing the chessboard with ints
void ChessBoard::drawIntBoard(){
    for (int i=11; i>-1 ; --i){
        for (int j=0; j<10 ; ++j){
            if (int(board[i * 10 + j])<0)
                std::cout << " " << int(board[i * 10 + j]);
            else
                std::cout << "  " << int(board[i * 10 + j]);
        }
        std::cout << std::endl << std::endl;
    }
}

/*************************************************************************************************
; 
; WHITE move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::whiteKnightMoveGeneration(){
    for (int i=0 ; i<8 ; ++i){
        if (board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
            /// the move is pseudo-legal
            /// may be capture as well
        }
        if (board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
            /// the move is pseudo-legal
            /// may be capture as well
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(){
    int j;
    for (int i=0 ; i<4 ; ++i){
        j = 1;
        while (board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
            /// the move is pseudo-legal
            ++j;
        }
        if (board[whiteLeftRook + j * rookOffsets[i]] < 0)
            std::cout << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table

        j = 1;
        while (board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
            /// the move is pseudo-legal
            j++;
        }
        if (board[whiteRightRook + j * rookOffsets[i]] < 0)
            std::cout << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}

void ChessBoard::whiteBishopMoveGeneration(){ // same as rook moves with dif ferent offsets
    int j;
    for (int i=0 ; i<4 ; ++i){
        j = 1;
        while (board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
            /// the move is pseudo-legal
            ++j;
        }
        if (board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
            std::cout << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
        
        j = 1;
        while (board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
            /// the move is pseudo-legal
            j++;
        }
        if (board[whiteRightBishop + j * bishopOffsets[i]] < 0)
            std::cout << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}

void ChessBoard::whiteQueenMoveGeneration(){
    int j;
    for (int i=0 ; i<4 ; ++i){
        j = 1;
        while (board[whiteQueen + j * bishopOffsets[i]] == 0){ 
            /// the move is pseudo-legal
            std::cout << "Queen to (bishop)" << whiteQueen + j * bishopOffsets[i] << std::endl;
            ++j;
        }
        if (board[whiteQueen + j * bishopOffsets[i]] < 0)
            std::cout << "Queen captures (bishop) " << whiteQueen + j * bishopOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
        
        j = 1;
        while (board[whiteQueen + j * rookOffsets[i]] == 0){ 
            std::cout << "Queen to (rook)" << whiteQueen + j * rookOffsets[i] << std::endl;
            /// the move is pseudo-legal
            j++;
        }
        if (board[whiteQueen + j * rookOffsets[i]] < 0)
            std::cout << "Queen captures on " << whiteQueen + j * rookOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}

void ChessBoard::whiteKingMoveGeneration(){
    for (int i=0 ; i<4 ; ++i){
        if (board[whiteKing + bishopOffsets[i]] < 1)
            std::cout << "King moves to" << whiteQueen + bishopOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }
}

void ChessBoard::generatePseudoLegalMoves(){
    whiteQueenMoveGeneration();
}