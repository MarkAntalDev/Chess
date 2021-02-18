#include "Board.h"

ChessBoard::ChessBoard(){
    enPassant = true;
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
    if (whiteLeftKnight != -1 && whiteRightKnight != -1){  /// both white knights are in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
                std::cout << "Knight to " << whiteLeftKnight + knightOffsets[i] << std::endl;
                /// the move is pseudo-legal
                /// may be capture as well
            }
            if (board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
                std::cout << "Knight to " << whiteRightKnight + knightOffsets[i] << std::endl;
                /// the move is pseudo-legal
                /// may be capture as well
            }
        }
    }else if (whiteLeftKnight == -1){ /// the white right knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
                std::cout << "Knight to " << whiteRightKnight + knightOffsets[i] << std::endl;
                /// the move is pseudo-legal
                /// may be capture as well
            }
        }
    }else if (whiteRightKnight == -1){ /// the white left knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
                std::cout << "Knight to " << whiteLeftKnight + knightOffsets[i] << std::endl;
                /// the move is pseudo-legal
                /// may be capture as well
            }
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(){
    int j;
    if (whiteLeftRook != -1 && whiteRightRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
                /// the move is pseudo-legal
                std::cout << "Rook to " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteLeftRook + j * rookOffsets[i]] < 0)
                std::cout << "Rook captures on " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table

            j = 1;
            while (board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
                /// the move is pseudo-legal
                std::cout << "Rook to " << whiteRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightRook + j * rookOffsets[i]] < 0)
                std::cout << "Rook captures on " << whiteRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteLeftRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
                /// the move is pseudo-legal
                std::cout << "Rook to " << whiteRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightRook + j * rookOffsets[i]] < 0)
                std::cout << "Rook captures on " << whiteRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteRightRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
                /// the move is pseudo-legal
                std::cout << "Rook to " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteLeftRook + j * rookOffsets[i]] < 0)
                std::cout << "Rook captures on " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
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
    if(whiteQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteQueen + j * bishopOffsets[i]] == 0){ 
                /// the move is pseudo-legal
                std::cout << "Queen to " << whiteQueen + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteQueen + j * bishopOffsets[i]] < 0)
                std::cout << "Queen captures " << whiteQueen + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[whiteQueen + j * rookOffsets[i]] == 0){ 
                std::cout << "Queen to " << whiteQueen + j * rookOffsets[i] << std::endl;
                /// the move is pseudo-legal
                j++;
            }
            if (board[whiteQueen + j * rookOffsets[i]] < 0)
                std::cout << "Queen captures on " << whiteQueen + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    } 
}

void ChessBoard::whiteKingMoveGeneration(){
    for (int i=0 ; i<4 ; ++i){
        if (board[whiteKing + bishopOffsets[i]] < 1)
            std::cout << "King moves to " << whiteQueen + bishopOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
        if (board[whiteKing + rookOffsets[i]] < 1)
            std::cout << "King moves to " << whiteKing + rookOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either white piece or off the table
    }

    /// Castling
    ///  
}

void ChessBoard::whitePawnsEnPassant(int index){
    if (index/10 == 6){
        if(board[index-1] == -1) /// en passant left to the current pawn
            std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 9] << " en passant" << std::endl;
        if(board[index+1] == -1) /// en passant left to the current pawn
            std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 11] << " en passant" << std::endl;    
    }
}

void ChessBoard::whitePawnsCapture(int index){
    if (board[index + 9] < 0){
        std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 9] << std::endl;
    }
    if (board[index + 11] < 0){
        std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 11] << std::endl;
    }
}

void ChessBoard::whitePawnMoveGeneration(){
    for (int i=0 ; i<8 ; ++i){
        char currentPawn = whitePawns[i];
        if (currentPawn != -1){
            if (currentPawn == 31 + i){ /// if the current pawn is on the starting position (checkable with the current index)
                whitePawnsCapture(currentPawn); /// calling the single capture function
                if (board[currentPawn + 10] == 0){ ///single push
                    std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 10] <<std::endl;
                    if(board[currentPawn + 20] == 0){ /// double push
                        std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 20] <<std::endl;
                        ///After this en passant is possible
                    }
                }
            }
            else{ /// when the current pawn is not on the starting position
                if (board[currentPawn + 10] == 0){
                    std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 10] <<std::endl;
                }
                ///here we have to check en passant, because the current pawn is not on the starting position
                if (enPassant) /// first check if en passant is possible
                    whitePawnsEnPassant(currentPawn); /// so we call en passant function with the current index

                whitePawnsCapture(currentPawn); /// we also call the basic capture function, because we can still capture normally
            }
        }
    }
}

void ChessBoard::generatePseudoLegalMoves(){
    whiteQueenMoveGeneration();
    whiteKingMoveGeneration();
    whiteKnightMoveGeneration();
    whitePawnMoveGeneration();
}

void ChessBoard::putInLegalMoves(unsigned char _from, unsigned  char _to, unsigned char _takenPiece){
    legalMove clm;
    clm.from = _from;
    clm.to = _to;
    clm.takenPiece = _takenPiece; 
    //legalMoves.push_back()
}