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
                //std::cout << "Rook to " << whiteRightRook + j * rookOffsets[i] << std::endl;
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

void ChessBoard::whiteBishopMoveGeneration(){ // same as rook moves with different offsets
    int j;
    if(whiteLeftBishop != -1 && whiteRightBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                std::cout << "Bishop to " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
                std::cout << "Bishop captures on " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                std::cout << "Bishop to " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightBishop + j * bishopOffsets[i]] < 0)
                std::cout << "Bishop captures on " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteLeftBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteRightBishop + j * bishopOffsets[i]] == 0){ // right bishop
                std::cout << "Bishop to " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteRightBishop + j * bishopOffsets[i]] < 0)
                std::cout << "Bishop captures on " << whiteRightBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }else if(whiteRightBishop == -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteLeftBishop + j * bishopOffsets[i]] == 0){ // left bishop
                std::cout << "Bishop to " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteLeftBishop + j * bishopOffsets[i]] < 0)
                std::cout << "Bishop captures on " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
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
    if (index/10 == 6){ // checking if the white pawn is
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

/*************************************************************************************************
; 
; BLACK move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::blackKnightMoveGeneration(){
    if (blackLeftKnight != -1 && blackRightKnight != -1){  /// both white knights are in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackLeftKnight + knightOffsets[i]] > -1 && board[blackLeftKnight + knightOffsets[i]] < 7){ // left knight
                putInLegalMoves((unsigned char)1,(unsigned char)1,(unsigned char)1,(char)1);
                std::cout << "Knight to " << blackLeftKnight + knightOffsets[i] << std::endl;
                /// may be capture as well
            }
            if (board[blackRightKnight + knightOffsets[i]] > -1 && board[blackRightKnight + knightOffsets[i]] < 7){ // right knight
                putInLegalMoves((unsigned char)1,(unsigned char)1,(unsigned char)1,(char)1);
                std::cout << "Knight to " << blackRightKnight + knightOffsets[i] << std::endl;
                /// may be capture as well
            }
        }
    }else if (blackLeftKnight == -1){ /// the white right knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackRightKnight + knightOffsets[i]] > -1 && board[blackRightKnight + knightOffsets[i]] < 7){ // right knight
                putInLegalMoves((unsigned char)1,(unsigned char)1,(unsigned char)1,(char)1);
                std::cout << "Knight to " << blackRightKnight + knightOffsets[i] << std::endl;
                /// may be capture as well
            }
        }
    }else if (blackRightKnight == -1){ /// the white left knight is in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[blackLeftKnight + knightOffsets[i]] > -1 && board[blackLeftKnight + knightOffsets[i]] < 7){ // left knight
                putInLegalMoves((unsigned char)1,(unsigned char)1,(unsigned char)1,(char)1);
                std::cout << "Knight to " << blackLeftKnight + knightOffsets[i] << std::endl;
                /// may be capture as well
            }
        }
    }
}

void ChessBoard::blackKingMoveGeneration(){
    for (int i=0 ; i<4 ; ++i){
        if (board[blackKing + bishopOffsets[i]] > -1 && board[blackKing + bishopOffsets[i]] < 7)
            std::cout << "King moves to " << blackQueen + bishopOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either black piece or off the table
        if (board[blackKing + rookOffsets[i]] > -1 && board[blackKing + rookOffsets[i]] < 7)
            std::cout << "King moves to " << blackKing + rookOffsets[i] << std::endl;
            /// the last move is a capture || else doesn't matter, either black piece or off the table
    }

    /// Castling
    ///  
}

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
    if(whiteQueen != -1){
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

void ChessBoard::putInLegalMoves(unsigned char _from, unsigned  char _to, unsigned char _takenPiece, char _value){
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
        blackKnightMoveGeneration();
        auto t2 = std::chrono::high_resolution_clock::now();

        std::cout << "test function took "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count()
              << " nanoseconds\n";
        std::cout << "test function took "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count()
              << " milliseconds\n\n\n";
    }    
}