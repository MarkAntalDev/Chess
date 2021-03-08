#include "Board.h"

ChessBoard::ChessBoard(){
    enPassant = false;
    currentPlayer = true;
    std::srand(std::time(nullptr));
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

void ChessBoard::whiteKnightMoveGeneration(char pieceIndex){
    char currentWhiteKnight = allPieces[pieceIndex];
    if (currentWhiteKnight != -1){  /// both white knights are in the game
        for (char i=0 ; i<8 ; ++i){
            if (board[currentWhiteKnight + knightOffsets[i]] < 1){ // left knight
                if (board[currentWhiteKnight + knightOffsets[i]] < 0){
                    putInLegalMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], findPiece(currentWhiteKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(char pieceIndex){
    char currentWhiteRook = allPieces[pieceIndex];
    char j;
    if (currentWhiteRook != -1){
        for (char i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteRook + j * rookOffsets[i]] == 0){ // left rook
                putInLegalMoves(pieceIndex, currentWhiteRook + j * rookOffsets[i], -1, 0);
                ++j;
                //std::cout << "Rook to " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                
            }
            if (board[currentWhiteRook + j * rookOffsets[i]] < 0)
                putInLegalMoves(pieceIndex, currentWhiteRook + j * rookOffsets[i], findPiece(currentWhiteRook + j * rookOffsets[i]), 0);
                //std::cout << "Rook captures on " << whiteLeftRook + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }
}

void ChessBoard::whiteBishopMoveGeneration(char pieceIndex){
    char currentWhiteBishop = allPieces[pieceIndex];
    char j;
    if(currentWhiteBishop != -1){
        for (char i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteBishop + j * bishopOffsets[i]] == 0){ // left bishop
                putInLegalMoves(pieceIndex, currentWhiteBishop + j * bishopOffsets[i], -1, 0);
                ++j;
                //std::cout << "Bishop to " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
            }
            if (board[currentWhiteBishop + j * bishopOffsets[i]] < 0)
                putInLegalMoves(pieceIndex, currentWhiteBishop + j * bishopOffsets[i], findPiece(currentWhiteBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << whiteLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    }
}

void ChessBoard::whiteQueenMoveGeneration(char pieceIndex){
    /// allPieces[14] == white queen
    char whiteQueen = allPieces[pieceIndex];
    char j;
    if(whiteQueen != -1){
        for (char i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteQueen + j * bishopOffsets[i]] == 0){
                putInLegalMoves(pieceIndex, whiteQueen + j * bishopOffsets[i], -1, 0);
                //std::cout << "Queen to " << whiteQueen + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[whiteQueen + j * bishopOffsets[i]] < 0)
                putInLegalMoves(pieceIndex, whiteQueen + j * bishopOffsets[i], findPiece(whiteQueen + j * bishopOffsets[i]), 0);
                //std::cout << "Queen captures " << whiteQueen + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[whiteQueen + j * rookOffsets[i]] == 0){
                putInLegalMoves(pieceIndex, whiteQueen + j * rookOffsets[i], -1, 0);
                //std::cout << "Queen to " << whiteQueen + j * rookOffsets[i] << std::endl;
                j++;
            }
            if (board[whiteQueen + j * rookOffsets[i]] < 0)
                putInLegalMoves(pieceIndex, whiteQueen + j * rookOffsets[i], findPiece(whiteQueen + j * rookOffsets[i]), 0);
                //std::cout << "Queen captures on " << whiteQueen + j * rookOffsets[i] << std::endl;
                // the last move is a capture || else doesn't matter, either white piece or off the table
        }
    } 
}

void ChessBoard::whiteKingMoveGeneration(){
    /// allPieces[15] == white king
    char whiteKing = allPieces[15];
    for (char i=0 ; i<4 ; ++i){
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

void ChessBoard::whitePawnsEnPassant(char index, char piece){
    if (index/10 == 6){ // checking if the pawn is on the good square
        if(board[index-1] == -1){ /// en passant left to the current pawn
            //std::cout << "Pawn on " << indexToPos[index] << " captures on " << indexToPos[index + 9] << " en passant" << std::endl;
            putInLegalMoves(piece, index + 9, findPiece(index-1), 0);
        } 
            
        if(board[index+1] == -1) /// en passant left to the current pawn
            //std::cout << "Pawn on " << indexToPos[index] << " captures on " << indexToPos[index + 11] << " en passant" << std::endl;
            putInLegalMoves(piece, index + 11, findPiece(index+1), 0);
    }
}

void ChessBoard::whitePawnsCapture(char index, char piece){
    if (board[index + 9] < 0){
        if(board[index + 20] == 7){
            putInLegalMoves(piece, index + 9, -2, 0);
            putInLegalMoves(piece, index + 9, -3, 0);
            putInLegalMoves(piece, index + 9, -4, 0);
            putInLegalMoves(piece, index + 9, -5, 0);
        }else{
            putInLegalMoves(piece, index + 9, findPiece(index+9), 0);
        }
    }
    if (board[index + 11] < 0){
        if(board[index + 20] == 7){
            putInLegalMoves(piece, index + 11, -2, 0);
            putInLegalMoves(piece, index + 11, -3, 0);
            putInLegalMoves(piece, index + 11, -4, 0);
            putInLegalMoves(piece, index + 11, -5, 0);
        }else{
            putInLegalMoves(piece, index + 11, findPiece(index+11), 0);
        }
    }
}

void ChessBoard::whitePawnMoveGeneration(char pieceIndex){
    char currentPawn = allPieces[pieceIndex]; /// 0-7
    if (currentPawn != -1){
        if (currentPawn == 31 + pieceIndex){ /// if the current pawn is on the starting position (checkable with the current index)
            whitePawnsCapture(currentPawn, pieceIndex); /// calling the single capture function
            if (board[currentPawn + 10] == 0){ ///single push
                putInLegalMoves(pieceIndex, currentPawn + 10, -1, 0);
                //std::cout << "Pawn on " << indexToPos[whitePawns[i]] << " to " << indexToPos[whitePawns[i] + 10] <<std::endl;
                if(board[currentPawn + 20] == 0){ /// double push
                    putInLegalMoves(pieceIndex, currentPawn + 20, -1, 0);
                    //std::cout << "Pawn on " << indexToPos[whitePawns[i]] << " to " << indexToPos[whitePawns[i] + 20] <<std::endl;
                    ///After this en passant is possible
                }
            }
        }
        else{ /// when the current pawn is not on the starting position
            if (board[currentPawn + 20] == 7 && board[currentPawn + 10] == 0){
                /// gyalog promóció történik a lépés után
                /// mind a 4 lehetséges bábura való promóciót hozzá kell tenni a lehetséges lépésekhez
                putInLegalMoves(pieceIndex, currentPawn + 10, -2, 0);
                putInLegalMoves(pieceIndex, currentPawn + 10, -3, 0);
                putInLegalMoves(pieceIndex, currentPawn + 10, -4, 0);
                putInLegalMoves(pieceIndex, currentPawn + 10, -5, 0);
            }else if (board[currentPawn + 10] == 0){
                putInLegalMoves(pieceIndex, currentPawn + 10, -1, 0);
            }
            ///here we have to check en passant, because the current pawn is not on the starting position
            if (enPassant) /// first check if en passant is possible
                whitePawnsEnPassant(currentPawn, pieceIndex); /// so we call en passant function with the current index

            whitePawnsCapture(currentPawn, pieceIndex); /// we also call the basic capture function, because we can still capture normally
        }
    }
}

/*************************************************************************************************
; 
; A meghívó függvények
; 
**************************************************************************************************/

void ChessBoard::whiteRookMoveCall(){
    whiteRookMoveGeneration(8);
    whiteRookMoveGeneration(9);
}

void ChessBoard::whiteBishopMoveCall(){
    whiteBishopMoveGeneration(12);
    whiteBishopMoveGeneration(13);
}

void ChessBoard::whiteQueenMoveCall(){
    whiteQueenMoveGeneration(14);
}

void ChessBoard::whiteKingMoveCall(){
    whiteKingMoveGeneration();
}

void ChessBoard::whiteKnightMoveCall(){
    whiteKnightMoveGeneration(10);
    whiteKnightMoveGeneration(11);
}

void ChessBoard::whitePawnMoveCall(){
    for(char i=0 ; i<8 ; ++i){
        whitePawnMoveGeneration(i);
    }
}

///összes meghívása
void ChessBoard::callAllWhiteMoveGeneration(){
    whiteRookMoveCall();
    whiteBishopMoveCall();
    whiteQueenMoveCall();
    whiteKingMoveCall();
    whiteKnightMoveCall();
    whitePawnMoveCall();
    promotedPiecesMoveGeneration(true);
}

/*************************************************************************************************
; 
; BLACK move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::blackKnightMoveGeneration(char pieceIndex){
    char currentBlackKnight = allPieces[pieceIndex];
    if (currentBlackKnight != -1){  /// both white knights are in the game
        for (char i=0 ; i<8 ; ++i){
            if (board[currentBlackKnight + knightOffsets[i]] > -1 && board[currentBlackKnight + knightOffsets[i]] < 7){ // left knight
                if (board[currentBlackKnight + knightOffsets[i]] > 0){
                    putInLegalMoves(pieceIndex, currentBlackKnight + knightOffsets[i], findPiece(currentBlackKnight + knightOffsets[i]), 0);
                }else{
                    putInLegalMoves(pieceIndex, currentBlackKnight + knightOffsets[i], -1, 0);
                }
            }
        }
    }
}

void ChessBoard::blackKingMoveGeneration(){
    /// allPieces[31] == black king
    char blackKing = allPieces[31];
    for (char i=0 ; i<4 ; ++i){
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

void ChessBoard::blackRookMoveGeneration(char pieceIndex){
    char currentBlackRook = allPieces[pieceIndex];
    char j;
    if (currentBlackRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackRook + j * rookOffsets[i]] == 0){ // left rook
                putInLegalMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], -1, 0);
                //std::cout << "Rook to " << blackLeftRook + j * rookOffsets[i] << std::endl;
                ++j;
            }
            if (board[currentBlackRook + j * rookOffsets[i]] > 0 && board[currentBlackRook + j * rookOffsets[i]] < 7)
                //std::cout << "Rook captures on " << blackLeftRook + j * rookOffsets[i] << std::endl;
                putInLegalMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], findPiece(currentBlackRook + j * rookOffsets[i]), 0);
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }
}

void ChessBoard::blackBishopMoveGeneration(char pieceIndex){
    char currentBlackBishop = allPieces[pieceIndex];
    char j;
    if(currentBlackBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackBishop + j * bishopOffsets[i]] == 0){ // left bishop
                putInLegalMoves(pieceIndex, currentBlackBishop + j * bishopOffsets[i], -1, 0);
                //std::cout << "Bishop to " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[currentBlackBishop + j * bishopOffsets[i]] > 0 && board[currentBlackBishop + j * bishopOffsets[i]] < 7)
                putInLegalMoves(pieceIndex, currentBlackBishop + j * bishopOffsets[i], findPiece(currentBlackBishop + j * bishopOffsets[i]), 0);
                //std::cout << "Bishop captures on " << blackLeftBishop + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either black piece or off the table
        }
    }   
}

void ChessBoard::blackQueenMoveGeneration(char pieceIndex){
    /// allPieces[30] == black queen
    char blackQueen = allPieces[pieceIndex];
    char j;
    if(blackQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackQueen + j * bishopOffsets[i]] == 0){
                putInLegalMoves(pieceIndex, blackQueen + j * bishopOffsets[i], -1, 0);
                //std::cout << "Queen to " << blackQueen + j * bishopOffsets[i] << std::endl;
                ++j;
            }
            if (board[blackQueen + j * bishopOffsets[i]] > 0 && board[blackQueen + j * bishopOffsets[i]] < 7 )
                putInLegalMoves(pieceIndex, blackQueen + j * bishopOffsets[i], findPiece(blackQueen + j * bishopOffsets[i]), 0);
                //std::cout << "Queen captures " << blackQueen + j * bishopOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            
            j = 1;
            while (board[blackQueen + j * rookOffsets[i]] == 0){
                putInLegalMoves(pieceIndex, blackQueen + j * rookOffsets[i], -1, 0);
                //std::cout << "Queen to " << blackQueen + j * rookOffsets[i] << std::endl;
                /// the move is pseudo-legal
                j++;
            }
            if (board[blackQueen + j * rookOffsets[i]] > 0 && board[blackQueen + j * rookOffsets[i]] < 7)
                putInLegalMoves(pieceIndex, blackQueen + j * rookOffsets[i], findPiece(blackQueen + j * rookOffsets[i]), 0);
                //std::cout << "Queen captures on " << blackQueen + j * rookOffsets[i] << std::endl;
                /// the last move is a capture || else doesn't matter, either white piece or off the table
        }
    } 
}

void ChessBoard::blackPawnsEnPassant(char index, char piece){
    if (index/10 == 5){
        if (board[index-1] == 1){/// en passant left to the current pawn
            putInLegalMoves(piece, index - 11, findPiece(index-1), 0);
            //std::cout << "Pawn on " << indexToPos[index] << " captures on " << indexToPos[index - 9] << " en passant" << std::endl;
        } 
        if (board[index+1] == 1){/// en passant left to the current pawn
            putInLegalMoves(piece, index - 9, findPiece(index+1), 0);
            //std::cout << "Pawn on " << indexToPos[index] << " captures on " << indexToPos[index - 11] << " en passant" << std::endl; 
        }     
    }
}

void ChessBoard::blackPawnsCapture(char index, char piece){
    if (board[index - 9] > 0 && board[index - 9] < 7){
        if(board[index - 20] == 7){
            putInLegalMoves(piece, index - 9, -2, 0);
            putInLegalMoves(piece, index - 9, -3, 0);
            putInLegalMoves(piece, index - 9, -4, 0);
            putInLegalMoves(piece, index - 9, -5, 0);
        }else{
            putInLegalMoves(piece, index - 9, findPiece(index-9), 0);
        }
    }
    if (board[index - 11] > 0 && board[index - 11] < 7){
        if(board[index - 20] == 7){
            putInLegalMoves(piece, index - 11, -2, 0);
            putInLegalMoves(piece, index - 11, -3, 0);
            putInLegalMoves(piece, index - 11, -4, 0);
            putInLegalMoves(piece, index - 11, -5, 0);
        }else{
            putInLegalMoves(piece, index - 11, findPiece(index-11), 0);
        }
        
        //std::cout << "Pawn on " << indexToPos[index] << " captures on " << indexToPos[index - 11] << std::endl;
    }
}

void ChessBoard::blackPawnMoveGeneration(char pieceIndex){
    char currentPawn = allPieces[pieceIndex];
    if (currentPawn != -1){
        if (currentPawn == 81 + pieceIndex - 16){ /// if the current pawn is on the starting position (checkable with the current index)
            blackPawnsCapture(currentPawn, pieceIndex);
            if (board[currentPawn - 10] == 0){ ///single push
                //std::cout << "Pawn on " << indexToPos[blackPawns[i]] << " to " << indexToPos[blackPawns[i] - 10] <<std::endl;
                putInLegalMoves(pieceIndex, currentPawn - 10, -1, 0);
                if (board[currentPawn - 20] == 0){ /// double push !!! it's only possible if single push is possible
                    putInLegalMoves(pieceIndex, currentPawn - 20, -1, 0);
                    //std::cout << "Pawn on " << indexToPos[blackPawns[i]] << " to " << indexToPos[blackPawns[i] - 20] <<std::endl;
                    ///After this en passant is possible
                }
            }
        }else{ /// when the current pawn is not on the starting position
            if (board[currentPawn - 20] == 7 && board[currentPawn - 10] == 0){
                /// gyalog promóció történik a lépés után
                /// mind a 4 lehetséges bábura való promóciót hozzá kell tenni a lehetséges lépésekhez
                putInLegalMoves(pieceIndex, currentPawn - 10, -2, 0);
                putInLegalMoves(pieceIndex, currentPawn - 10, -3, 0);
                putInLegalMoves(pieceIndex, currentPawn - 10, -4, 0);
                putInLegalMoves(pieceIndex, currentPawn - 10, -5, 0);
            }
            else if (board[currentPawn - 10] == 0){
                putInLegalMoves(pieceIndex, currentPawn - 10, -1, 0);
                //std::cout << "Pawn on " << indexToPos[blackPawns[i]] << " to " << indexToPos[blackPawns[i] - 10] <<std::endl;
                ////// PROMOTION IF POSSIBLE
            }
            ///here we have to check en passant, because the current pawn is not on the starting position
            if (enPassant) /// first check if en passant is possible
                blackPawnsEnPassant(currentPawn, pieceIndex); /// so we call en passant function with the current index
        }
    }
}

/*************************************************************************************************
; 
; A meghívó függvények a fekete bábuknak
; 
**************************************************************************************************/

void ChessBoard::blackRookMoveCall(){
    blackRookMoveGeneration(24);
    blackRookMoveGeneration(25);
}

void ChessBoard::blackBishopMoveCall(){
    blackBishopMoveGeneration(28);
    blackBishopMoveGeneration(29);
}

void ChessBoard::blackQueenMoveCall(){
    blackQueenMoveGeneration(30);
}

void ChessBoard::blackKingMoveCall(){
    blackKingMoveGeneration();
}

void ChessBoard::blackKnightMoveCall(){
    blackKnightMoveGeneration(26);
    blackKnightMoveGeneration(27);
}

void ChessBoard::blackPawnMoveCall(){
    for(char i=0 ; i<8 ; ++i){
        blackPawnMoveGeneration(i+16);
    }
}

///összes meghívása
void ChessBoard::callAllBlackMoveGeneration(){
    blackRookMoveCall();
    blackBishopMoveCall();
    blackQueenMoveCall();
    blackKingMoveCall();
    blackKnightMoveCall();
    blackPawnMoveCall();
    promotedPiecesMoveGeneration(false);
}


////Promótált bábuk függvénye
void ChessBoard::promotedPiecesMoveGeneration(bool player){
    char i = 32;
    if(player){
        while(i < 38 && allPieces[i] > 0){
            switch (board[allPieces[i]])
            {
            case 5:
                whiteQueenMoveGeneration(i);
                break;
            case 2:
                whiteKnightMoveGeneration(i);
                break;
            case 3:
                whiteBishopMoveGeneration(i);
                break;
            case 4:
                whiteRookMoveGeneration(i);
            default:
                break;
            }
        }
    }else{
        while(i < 38 && allPieces[i] > 0){
            switch (board[allPieces[i]])
            {
            case -5:
                blackQueenMoveGeneration(i);
                break;
            case -2:
                blackKnightMoveGeneration(i);
                break;
            case -3:
                blackBishopMoveGeneration(i);
                break;
            case -4:
                blackRookMoveGeneration(i);
            default:
                break;
            }
        }
    }
}


/*************************************************************************************************
; 
; Additional Functions
; 
**************************************************************************************************/

void ChessBoard::doPawnPromotion(legalMove promotionMove){
    if(board[allPieces[promotionMove.from]] > 0){
        if(allPieces[promotionMove.from] + 10 != promotionMove.to){
            /// gyalog promóció ütéssel együtt
            board[allPieces[promotionMove.from]] = 0;
            allPieces[promotionMove.from] = -1;
            allPieces[findPiece(promotionMove.to)] = -1;
            char i = 47;
            while(allPieces[i-1] < 0)
                --i;
            allPieces[i] = promotionMove.to;
            switch (promotionMove.takenPiece){
            case -2:
                board[promotionMove.to] = 2;
                break;
            case -3:
                board[promotionMove.to] = 3;
                break;
            case -4:
                board[promotionMove.to] = 4;
                break;
            case -5:
                board[promotionMove.to] = 5;
                break;
            default:
                break;
            }
        }else{
            /// gyalog promóció egyszerű előrelépéssel
            board[allPieces[promotionMove.from]] = 0;
            allPieces[promotionMove.from] = -1;
            char i = 47;
            while(allPieces[i-1] < 0)
                --i;
            allPieces[i] = promotionMove.to;
            switch (promotionMove.takenPiece){
            case -2:
                board[promotionMove.to] = 2;
                break;
            case -3:
                board[promotionMove.to] = 3;
                break;
            case -4:
                board[promotionMove.to] = 4;
                break;
            case -5:
                board[promotionMove.to] = 5;
                break;
            default:
                break;
            }
        }
    }else{
        if(allPieces[promotionMove.from] - 10 != promotionMove.to){
            /// gyalog promóció ütéssel együtt
            board[allPieces[promotionMove.from]] = 0;
            allPieces[promotionMove.from] = -1;
            allPieces[findPiece(promotionMove.to)] = -1;
            char i = 47;
            while(allPieces[i-1] < 0)
                --i;
            allPieces[i] = promotionMove.to;
            switch (promotionMove.takenPiece){
            case -2:
                board[promotionMove.to] = -2;
                break;
            case -3:
                board[promotionMove.to] = -3;
                break;
            case -4:
                board[promotionMove.to] = -4;
                break;
            case -5:
                board[promotionMove.to] = -5;
                break;
            default:
                break;
            }
        }else{
            /// gyalog promóció egyszerű előrelépéssel
            board[allPieces[promotionMove.from]] = 0;
            allPieces[promotionMove.from] = -1;
            char i = 47;
            while(allPieces[i-1] < 0)
                --i;
            allPieces[i] = promotionMove.to;
            switch (promotionMove.takenPiece){
            case -2:
                board[promotionMove.to] = -2;
                break;
            case -3:
                board[promotionMove.to] = -3;
                break;
            case -4:
                board[promotionMove.to] = -4;
                break;
            case -5:
                board[promotionMove.to] = -5;
                break;
            default:
                break;
            }
        }
    }
}

void ChessBoard::generatePseudoLegalMoves(){
    if(currentPlayer){ /// current player is true when it's white's turn
        //std::cout << "white's turn" << std::endl;
        callAllWhiteMoveGeneration();
    }else{ /// current player is false when it's black's turn
        //std::cout << "black's turn" << std::endl;
        callAllBlackMoveGeneration();
    }
}

void ChessBoard::makeMove(char indexOfMove){
    if(legalMoves.size() < indexOfMove+1){
        std::cout << "out of range";
    }
    else{
        if (legalMoves[indexOfMove].takenPiece == -1){ /// lépés, de nem ütés
            std::cout << "Piece: " << int(getPieceNumber(legalMoves[indexOfMove].from)) << " index: " << int(allPieces[legalMoves[indexOfMove].from]) << " on: "<< indexToPos[allPieces[legalMoves[indexOfMove].from]] << " moves to " << indexToPos[legalMoves[indexOfMove].to] << std::endl;
            board[allPieces[legalMoves[indexOfMove].from]] = 0;
            board[legalMoves[indexOfMove].to] = getPieceNumber(legalMoves[indexOfMove].from);
            allPieces[legalMoves[indexOfMove].from] = legalMoves[indexOfMove].to;
        }else if(legalMoves[indexOfMove].takenPiece < -1){ /// bábu promóció
            doPawnPromotion(legalMoves[indexOfMove]);
        }else{ /// ütések
            std::cout << "Piece: " << int(getPieceNumber(legalMoves[indexOfMove].from)) << " on: "<< indexToPos[allPieces[legalMoves[indexOfMove].from]] << " captures: " << int(getPieceNumber(legalMoves[indexOfMove].takenPiece)) << indexToPos[legalMoves[indexOfMove].to] << std::endl;
            board[allPieces[legalMoves[indexOfMove].from]] = 0;
            board[legalMoves[indexOfMove].takenPiece] = 0;
            board[legalMoves[indexOfMove].to] = getPieceNumber(legalMoves[indexOfMove].from);
            allPieces[legalMoves[indexOfMove].from] = legalMoves[indexOfMove].to;
            allPieces[legalMoves[indexOfMove].takenPiece] = -1; 
        }
    }
}


/// A vecktorunkhoz push-ol egy legalMove struktúrát a paraméterekben megadott értékekkel
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
        for(int j=0; j<1000000 ; ++j)
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

/// finds the index of the piece in allPieces[] based on the given index in board[]
char ChessBoard::findPiece(char index){
    for (char i=0 ; i<48 ; ++i){
        if (allPieces[i] == index){
            return i;
        }
    }
    return char(-1);
}

char ChessBoard::getPieceNumber(char index){
    if(index > -1 && index < 8)
        return 1;
    if(index > 15 && index < 24)
        return -1;
    switch(index){
////// Fehér bábuk
        case 8:
            return 4;
            break;
        case 9:
            return 4;
            break;
        case 10:
            return 2;
            break;
        case 11:
            return 2;
            break;
        case 12:
            return 3;
            break;
        case 13:
            return 3;
            break;
        case 14:
            return 5;
            break;
        case 15:
            return 6;
            break;

/////// Fekete bábuk            
        case 24:
            return -4;
            break;
        case 25:
            return -4;
            break;
        case 26:
            return -2;
            break;
        case 27:
            return -2;
            break;
        case 28:
            return -3;
            break;
        case 29:
            return -3;
            break;
        case 30:
            return -5;
            break;
        case 31:
            return -6;
            break;
    }
}

void ChessBoard::generateRandomMove(){
    legalMoves.clear();
    generatePseudoLegalMoves();
    char randomIndex = std::rand()%legalMoves.size();
    std::cout << int(legalMoves[randomIndex].from) << " " << int(legalMoves[randomIndex].to) << " " << int(legalMoves[randomIndex].takenPiece) << std::endl;
    makeMove(randomIndex);
    this->currentPlayer = !this->currentPlayer;
}