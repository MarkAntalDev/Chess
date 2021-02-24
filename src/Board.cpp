#include "Board.h"

ChessBoard::ChessBoard(){
    enPassant = true;
    currentPlayer = false;
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
    /// allPieces[10] == white left knight
    /// allPieces[11] == white right knight
    char whiteLeftKnight = allPieces[10];
    char whiteRightKnight = allPieces[11];
    if (whiteLeftKnight != -1 && whiteRightKnight != -1){  /// both white knights are in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
                //std::cout << "Knight to " << whiteLeftKnight + knightOffsets[i] << std::endl;
                if (board[whiteLeftKnight + knightOffsets[i]] < 0){
                    putInLegalMoves(10, whiteLeftKnight + knightOffsets[i], findPiece(whiteLeftKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(10, whiteLeftKnight + knightOffsets[i], -1, 0);
                }
            }
            if (board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
                //std::cout << "Knight to " << whiteRightKnight + knightOffsets[i] << std::endl;
                if (board[whiteRightKnight + knightOffsets[i]] < 0){
                    putInLegalMoves(11, whiteRightKnight + knightOffsets[i], findPiece(whiteRightKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(11, whiteRightKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }else if (whiteLeftKnight == -1){ /// the white right knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteRightKnight + knightOffsets[i]] < 1){ // right knight
                //std::cout << "Knight to " << whiteRightKnight + knightOffsets[i] << std::endl;
                if (board[whiteRightKnight + knightOffsets[i]] < 0){
                    putInLegalMoves(11, whiteRightKnight + knightOffsets[i], findPiece(whiteRightKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(11, whiteRightKnight + knightOffsets[i], -1, 0);
                }
                /// may be capture as well
            }
        }
    }else if (whiteRightKnight == -1){ /// the white left knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[whiteLeftKnight + knightOffsets[i]] < 1){ // left knight
                //std::cout << "Knight to " << whiteLeftKnight + knightOffsets[i] << std::endl;
                if (board[whiteLeftKnight + knightOffsets[i]] < 0){
                    putInLegalMoves(10, whiteLeftKnight + knightOffsets[i], findPiece(whiteLeftKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(10, whiteLeftKnight + knightOffsets[i], -1, 0);
                }
                /// may be capture as well
            }
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(){
    /// allPieces[8] == white left rook
    /// allPieces[9] == white right rook
    char whiteLeftRook = allPieces[8];
    char whiteRightRook = allPieces[9];
    int j;
    if (whiteLeftRook != -1 && whiteRightRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
                putInLegalMoves(8, whiteLeftRook + j * rookOffsets[i], -1, 0);
                ++j;
                //std::cout << "Rook to " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                
            }
            if (board[whiteLeftRook + j * rookOffsets[i]] < 0)
                putInLegalMoves(8, whiteLeftRook + j * rookOffsets[i], findPiece(whiteLeftRook + j * rookOffsets[i]), 0);
                //std::cout << "Rook captures on " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table

            j = 1;
            while (board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
                putInLegalMoves(9, whiteRightRook + j * rookOffsets[i], -1, 0);
                //std::cout << "Rook to " << whiteRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightRook + j * rookOffsets[i]] < 0)
                putInLegalMoves(9, whiteRightRook + j * rookOffsets[i], findPiece(whiteRightRook + j * rookOffsets[i]), 0);
                //std::cout << "Rook captures on " << whiteRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteLeftRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteRightRook + j * rookOffsets[i]] == 0){ // right rooks
                putInLegalMoves(9, whiteRightRook + j * rookOffsets[i], -1, 0);
                //std::cout << "Rook to " << whiteRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightRook + j * rookOffsets[i]] < 0)
                putInLegalMoves(9, whiteRightRook + j * rookOffsets[i], findPiece(whiteRightRook + j * rookOffsets[i]), 0);
                //std::cout << "Rook captures on " << whiteRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteRightRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftRook + j * rookOffsets[i]] == 0){ // left rook
                putInLegalMoves(8, whiteLeftRook + j * rookOffsets[i], -1, 0);
                ++j;
                //std::cout << "Rook to " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                
            }
            if (board[whiteLeftRook + j * rookOffsets[i]] < 0)
                putInLegalMoves(8, whiteLeftRook + j * rookOffsets[i], findPiece(whiteLeftRook + j * rookOffsets[i]), 0);
                //std::cout << "Rook captures on " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }
}

void ChessBoard::whiteBishopMoveGeneration(){
    /// allPieces[12] == white left bishop
    /// allPieces[13] == white right bishop
    char whiteLeftBishop = allPieces[12];
    char whiteRightBishop = allPieces[13];
    int j;
    if(whiteLeftBishop != -1 && whiteRightBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                putInLegalMoves(12, whiteLeftBishop + j * bishopOffsets[i], -1, 0);
                ++j;
                //std::cout << "Bishop to " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
            }
            if (board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
                putInLegalMoves(12, whiteLeftBishop + j * bishopOffsets[i], findPiece(whiteLeftBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                putInLegalMoves(13, whiteRightBishop + j * bishopOffsets[i], -1, 0);
                //std::cout << "Bishop to " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightBishop + j * bishopOffsets[i]] < 0)
                putInLegalMoves(12, whiteRightBishop + j * bishopOffsets[i], findPiece(whiteRightBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteLeftBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                putInLegalMoves(13, whiteRightBishop + j * bishopOffsets[i], -1, 0);
                //std::cout << "Bishop to " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightBishop + j * bishopOffsets[i]] < 0)
                putInLegalMoves(12, whiteRightBishop + j * bishopOffsets[i], findPiece(whiteRightBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteRightBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                putInLegalMoves(12, whiteLeftBishop + j * bishopOffsets[i], -1, 0);
                ++j;
                //std::cout << "Bishop to " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
            }
            if (board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
                putInLegalMoves(12, whiteLeftBishop + j * bishopOffsets[i], findPiece(whiteLeftBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }
}

void ChessBoard::whiteQueenMoveGeneration(){
    /// allPieces[14] == white queen
    char whiteQueen = allPieces[14];
    int j;
    if(whiteQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteQueen + j * bishopOffsets[i]] == 0){
                putInLegalMoves(14, whiteQueen + j * bishopOffsets[i], -1, 0);
                //std::cout << "Queen to " << whiteQueen + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteQueen + j * bishopOffsets[i]] < 0)
                putInLegalMoves(14, whiteQueen + j * bishopOffsets[i], findPiece(whiteQueen + j * bishopOffsets[i]), 0);
                //std::cout << "Queen captures " << whiteQueen + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[whiteQueen + j * rookOffsets[i]] == 0){
                putInLegalMoves(14, whiteQueen + j * rookOffsets[i], -1, 0);
                //std::cout << "Queen to " << whiteQueen + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteQueen + j * rookOffsets[i]] < 0)
                putInLegalMoves(14, whiteQueen + j * rookOffsets[i], findPiece(whiteQueen + j * rookOffsets[i]), 0);
                //std::cout << "Queen captures on " << whiteQueen + j * rookOffsets[i] << std::endl;
                // the last move is a capture || else doesn't matter, either white piece or off the table
        }
    } 
}

void ChessBoard::whiteKingMoveGeneration(){
    /// allPieces[15] == white queen
    char whiteKing = allPieces[15];
    for (int i=0 ; i<4 ; ++i){
        if (board[whiteKing + bishopOffsets[i]] < 1){
            if(board[whiteKing + bishopOffsets[i]] < 0)
                putInLegalMoves(15, whiteKing + bishopOffsets[i], findPiece(whiteKing + bishopOffsets[i]), 0);
            else
                putInLegalMoves(15, whiteKing + bishopOffsets[i], -1, 0);
        }
        if (board[whiteKing + rookOffsets[i]] < 1){
            if(board[whiteKing + rookOffsets[i]] < 0)
                putInLegalMoves(15, whiteKing + rookOffsets[i], findPiece(whiteKing + rookOffsets[i]), 0);
            else
                putInLegalMoves(15, whiteKing + rookOffsets[i], -1, 0);
        }
    }

    /// Castling
    ///  
}

void ChessBoard::whitePawnsEnPassant(int index, char piece){
    if (index/10 == 6){ // checking if the pawn is on the good square
        if(board[index-1] == -1){ /// en passant left to the current pawn
            //std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 9] << " en passant" << std::endl;
            putInLegalMoves(piece, index + 9, findPiece(index-1), 0);
        } 
            
        if(board[index+1] == -1) /// en passant left to the current pawn
            //std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 11] << " en passant" << std::endl;
            putInLegalMoves(piece, index + 11, findPiece(index+1), 0);
    }
}

void ChessBoard::whitePawnsCapture(int index, char piece){
    if (board[index + 9] < 0){
        //std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 9] << std::endl;
        putInLegalMoves(piece, index + 9, findPiece(index+9), 0);
    }
    if (board[index + 11] < 0){
        //std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index + 11] << std::endl;
        putInLegalMoves(piece, index + 11, findPiece(index+11), 0);
    }
}

void ChessBoard::whitePawnMoveGeneration(){
    for (int i=0 ; i<8 ; ++i){
        char currentPawn = allPieces[i];
        if (currentPawn != -1){
            if (currentPawn == 31 + i){ /// if the current pawn is on the starting position (checkable with the current index)
                whitePawnsCapture(currentPawn, char(i)); /// calling the single capture function
                if (board[currentPawn + 10] == 0){ ///single push
                    putInLegalMoves(char(i), currentPawn + 10, -1, 0);
                    //std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 10] <<std::endl;
                    if(board[currentPawn + 20] == 0){ /// double push
                        putInLegalMoves(char(i), currentPawn + 20, -1, 0);
                        //std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 20] <<std::endl;
                        ///After this en passant is possible
                    }
                }
            }
            else{ /// when the current pawn is not on the starting position
                if (board[currentPawn + 10] == 0){
                    putInLegalMoves(char(i), currentPawn + 10, -1, 0);
                    //std::cout << "Pawn on " << indexPosMap[whitePawns[i]] << " to " << indexPosMap[whitePawns[i] + 10] <<std::endl;
                }
                ///here we have to check en passant, because the current pawn is not on the starting position
                if (enPassant) /// first check if en passant is possible
                    whitePawnsEnPassant(currentPawn, char(i)); /// so we call en passant function with the current index

                whitePawnsCapture(currentPawn, char(i)); /// we also call the basic capture function, because we can still capture normally
            }
        }
    }
}

/*************************************************************************************************
; 
; BLACK move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::blackKnightMoveGeneration(){
    /// allPieces[26] == black left knight
    /// allPieces[27] == black right knight
    char blackLeftKnight = allPieces[26];
    char blackRightKnight = allPieces[27];
    if (blackLeftKnight != -1 && blackRightKnight != -1){  /// both white knights are in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackLeftKnight + knightOffsets[i]] > -1 && board[blackLeftKnight + knightOffsets[i]] < 7){ // left knight
                if (board[blackLeftKnight + knightOffsets[i]] > 0){
                    putInLegalMoves(26, blackLeftKnight + knightOffsets[i], findPiece(blackLeftKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(26, blackLeftKnight + knightOffsets[i], -1, 0);
                }
            }
            if (board[blackRightKnight + knightOffsets[i]] > -1 && board[blackRightKnight + knightOffsets[i]] < 7){ // right knight
                if (board[blackRightKnight + knightOffsets[i]] > 0){
                    putInLegalMoves(27, blackRightKnight + knightOffsets[i], findPiece(blackRightKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(27, blackRightKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }else if (blackLeftKnight == -1){ /// the white right knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackRightKnight + knightOffsets[i]] > -1 && board[blackRightKnight + knightOffsets[i]] < 7){ // right knight
                if (board[blackRightKnight + knightOffsets[i]] > 0){
                    putInLegalMoves(27, blackRightKnight + knightOffsets[i], findPiece(blackRightKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(27, blackRightKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }else if (blackRightKnight == -1){ /// the white left knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackLeftKnight + knightOffsets[i]] > -1 && board[blackLeftKnight + knightOffsets[i]] < 7){ // left knight
                if (board[blackLeftKnight + knightOffsets[i]] > 0){
                    putInLegalMoves(26, blackLeftKnight + knightOffsets[i], findPiece(blackLeftKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(26, blackLeftKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }
}

void ChessBoard::blackKingMoveGeneration(){
    /// allPieces[31] == black king
    char blackKing = allPieces[31];
    for (int i=0 ; i<4 ; ++i){
        if (board[blackKing + bishopOffsets[i]] > -1 && board[blackKing + bishopOffsets[i]] < 7){
             if(board[blackKing + bishopOffsets[i]] > 0)
                putInLegalMoves(31, blackKing + bishopOffsets[i], findPiece(blackKing + bishopOffsets[i]), 0);
            else
                putInLegalMoves(31, blackKing + bishopOffsets[i], -1, 0);
        }
        if (board[blackKing + rookOffsets[i]] > -1 && board[blackKing + rookOffsets[i]] < 7){
            if(board[blackKing + rookOffsets[i]] > 0)
                putInLegalMoves(31, blackKing + rookOffsets[i], findPiece(blackKing + rookOffsets[i]), 0);
            else
                putInLegalMoves(31, blackKing + rookOffsets[i], -1, 0);
        }
    }

    /// Castling
    ///  
}
//////////////////////////////////////////////////////////////// CONTINUE HERE ////////////////////////////////////////////////////////
void ChessBoard::blackRookMoveGeneration(){
    int j;
    if (blackLeftRook != -1 && blackRightRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackLeftRook + j * rookOffsets[i]] == 0){ // left rook
                /// the move is pseudo-legal
                std::cout << "Rook to " << blackLeftRook + j * rookOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackLeftRook + j * rookOffsets[i]] > 0 && board[blackLeftRook + j * rookOffsets[i]] < 7)
                std::cout << "Rook captures on " << blackLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table

            j = 1;
            while (board[blackRightRook + j * rookOffsets[i]] == 0){ // right rooks
                /// the move is pseudo-legal
                std::cout << "Rook to " << blackRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[blackRightRook + j * rookOffsets[i]] > 0 && board[blackRightRook + j * rookOffsets[i]] < 7)
                std::cout << "Rook captures on " << blackRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }else if(blackLeftRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackRightRook + j * rookOffsets[i]] == 0){ // right rooks
                /// the move is pseudo-legal
                std::cout << "Rook to " << blackRightRook + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[blackRightRook + j * rookOffsets[i]] > 0 && board[blackRightRook + j * rookOffsets[i]] < 7)
                std::cout << "Rook captures on " << blackRightRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }else if(blackRightRook == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackLeftRook + j * rookOffsets[i]] == 0){ // left rook
                /// the move is pseudo-legal
                std::cout << "Rook to " << blackLeftRook + j * rookOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackLeftRook + j * rookOffsets[i]] > 0 && board[blackLeftRook + j * rookOffsets[i]] < 7)
                std::cout << "Rook captures on " << blackLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }
}

void ChessBoard::blackBishopMoveGeneration(){ // same as rook moves with different offsets
    int j;
    if(blackLeftBishop != -1 && blackRightBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                std::cout << "Bishop to " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackLeftBishop + j * bishopOffsets[i]] > 0 && board[blackLeftBishop + j * bishopOffsets[i]] < 7)
                std::cout << "Bishop captures on " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
            
            j = 1;
            while (board[blackRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                std::cout << "Bishop to " << blackRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[blackRightBishop + j * bishopOffsets[i]] > 0 && board[blackRightBishop + j * bishopOffsets[i]] < 7)
                std::cout << "Bishop captures on " << blackRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }else if(blackLeftBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                std::cout << "Bishop to " << blackRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[blackRightBishop + j * bishopOffsets[i]] > 0 && board[blackRightBishop + j * bishopOffsets[i]] < 7)
                std::cout << "Bishop captures on " << blackRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }else if(blackRightBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                std::cout << "Bishop to " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackLeftBishop + j * bishopOffsets[i]] > 0 && board[blackLeftBishop + j * bishopOffsets[i]] < 7)
                std::cout << "Bishop captures on " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }
    
}

void ChessBoard::blackQueenMoveGeneration(){
    int j;
    if(blackQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackQueen + j * bishopOffsets[i]] == 0){
                /// the move is pseudo-legal
                std::cout << "Queen to " << blackQueen + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackQueen + j * bishopOffsets[i]] > 0 && board[blackQueen + j * bishopOffsets[i]] < 7)
                std::cout << "Queen captures " << blackQueen + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[blackQueen + j * rookOffsets[i]] == 0){
                std::cout << "Queen to " << blackQueen + j * rookOffsets[i] << std::endl;
                /// the move is pseudo-legal
                j++;
            }
            if (board[blackQueen + j * rookOffsets[i]] > 0 && board[blackQueen + j * rookOffsets[i]] < 7)
                std::cout << "Queen captures on " << blackQueen + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    } 
}

void ChessBoard::blackPawnsEnPassant(int index){
    if (index/10 == 5){
        if (board[index-1] == 1) /// en passant left to the current pawn
            std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index - 9] << " en passant" << std::endl;
        if (board[index+1] == 1) /// en passant left to the current pawn
            std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index - 11] << " en passant" << std::endl;    
    }
}

void ChessBoard::blackPawnsCapture(int index){
    if (board[index - 9] > 0 && board[index - 9] < 7){
        std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index - 9] << std::endl;
    }
    if (board[index - 11] > 0 && board[index - 11] < 7){
        std::cout << "Pawn on " << indexPosMap[index] << " captures on " << indexPosMap[index - 11] << std::endl;
    }
}

void ChessBoard::blackPawnMoveGeneration(){
    for (int i=0 ; i<8 ; ++i){
        char currentPawn = blackPawns[i];
        if (currentPawn != -1){
            blackPawnsCapture(currentPawn); /// we call the capture function (because its possible anywhere)
            if (currentPawn == 81 + i){ /// if the current pawn is on the starting position (checkable with the current index)
                if (board[currentPawn - 10] == 0){ ///single push
                    std::cout << "Pawn on " << indexPosMap[blackPawns[i]] << " to " << indexPosMap[blackPawns[i] - 10] <<std::endl;
                    if (board[currentPawn - 20] == 0){ /// double push !!! it's only possible if single push is possible
                        std::cout << "Pawn on " << indexPosMap[blackPawns[i]] << " to " << indexPosMap[blackPawns[i] - 20] <<std::endl;
                        ///After this en passant is possible
                    }
                }
            }
            else{ /// when the current pawn is not on the starting position
                if (board[currentPawn - 10] == 0){
                    std::cout << "Pawn on " << indexPosMap[blackPawns[i]] << " to " << indexPosMap[blackPawns[i] - 10] <<std::endl;
                    ////// PROMOTION IF POSSIBLE
                }
                ///here we have to check en passant, because the current pawn is not on the starting position
                if (enPassant) /// first check if en passant is possible
                    blackPawnsEnPassant(currentPawn); /// so we call en passant function with the current index
            }
        }
    }
}


/*************************************************************************************************
; 
; Additional Functions
; 
**************************************************************************************************/

void ChessBoard::generatePseudoLegalMoves(){
    if(currentPlayer){ /// current player is true when it's white's turn
        whiteQueenMoveGeneration();
        whiteKingMoveGeneration();
        whiteRookMoveGeneration();
        whiteKnightMoveGeneration();
        whiteBishopMoveGeneration();
        whitePawnMoveGeneration();
    }else{ /// current player is false when it's black's turn
        blackQueenMoveGeneration();
        blackKingMoveGeneration();
        blackRookMoveGeneration();
        blackKnightMoveGeneration();
        blackBishopMoveGeneration();
        blackPawnMoveGeneration();
    }
}

void ChessBoard::putInLegalMoves(char _from, char _to, char _takenPiece, char _value){
    legalMove clm;
    clm.from = _from;
    clm.to = _to;
    clm.takenPiece = _takenPiece; 
    clm.value = _value;
    legalMoves.push_back(clm);
}

void ChessBoard::testingFunction(){
    for (int i = 0; i < 30; ++i)
    {
        auto t1 = std::chrono::high_resolution_clock::now();
        for(int j=0; j<1 ; ++j)
            generatePseudoLegalMoves();
        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << "test function took "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count()
              << " nanoseconds\n";
        std::cout << "test function took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n\n\n";
    }    
}

void ChessBoard::writeVector(){
    for (auto & element : legalMoves) {
        std::cout << int(element.from) << " " << int(element.to) << " " << int(element.takenPiece) << std::endl;
    }
}

char ChessBoard::findPiece(char index){
    for (char i=0 ; i<32 ; ++i){
        if (allPieces[i] == index){
            return i;
        }
    }
    return char(-1);
}