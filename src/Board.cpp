#include "Board.h"

void ChessBoard::initBoard(){
    
}

ChessBoard::ChessBoard(){
    //initBoard();
    NewGame();
    std::srand(std::time(nullptr));
}

void ChessBoard::NewGame(){
    srand (time(NULL));
    int counter = 0;
    for (int x : {
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
      7,  4,  2,  3,  5,  6,  3,  2,  4,  7,
      7,  1,  1,  1,  1,  1,  1,  1,  1,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7, -1, -1, -1, -1, -1, -1, -1, -1,  7,
      7, -4, -2, -3, -5, -6, -3, -2, -4,  7,
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7
    }){
        board[counter] = x;
        ++counter;
    }

    counter = 0;
    for (int x : { 31, 32, 33, 34, 35, 36, 37, 38, 21, 28, 22, 27, 23, 26, 24, 25, 
                   81, 82, 83, 84, 85, 86, 87, 88, 91, 98, 92, 97, 93, 96, 94, 95})
    {
        allPieces[counter] = x;
        ++counter;
    }

    for(int i= 0; i< 8 ; ++i){
        whitePromoted[i] = 0;
        blackPromoted[i] = 0;
    }

    currentPlayer = true;
    currentDepth = 0;
    historyDepth = 0;
    firstMoveOfDepth[currentDepth] = 0;
    fifthyMove = 0;
    castleFlag = 15; /// ...0001111
    castled = 0; /// ...0000  ha (0001 fehér sáncolt már)
    followBook = true;
    ep = -1;
    void GenerateHash();
    inParseMove = false;
}

int ChessBoard::parseMove(std::string moveStr){
    inParseMove = true;
    int m = -1;
    int from = posToIndex[moveStr.substr(0,2)];
    int to = posToIndex[moveStr.substr(2,2)];
    //std::cout << moveStr << "len " << moveStr.length() << " from " << from << " to " << to << "      " << moveStr.substr(0,2) << " " << moveStr.substr(2,2) <<std::endl;
    int promote = 0;
    if (moveStr[4] != ' '){
        if(currentPlayer){
            //std::cout << " promocio beolvasva" << std::endl;
            promote = whitePlayerPromote[moveStr.substr(4,1)];
        }
        else{
            promote = blackPlayerPromote[moveStr.substr(4,1)];
        }
            
    }
    for (int i=firstMoveOfDepth[currentDepth]; i<firstMoveOfDepth[currentDepth+1]; ++i){
        if(moves[i].m.b.from == findPiece(from) && moves[i].m.b.to == to){
            if(promote != 0){
                if(moves[i].m.b.promote == promote)
                    m = i;
            }else{
                m = i;
            }       
        }
    }
    inParseMove = false;
    return m;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Zobrist hashelés implementációja
///
/////////////////////////////////////////////////////////////////////////////////////////////////

/// visszaad egy random bitsorozatot
int ChessBoard::RandBits(){
    int randomBits = 0;
    for (int i = 0; i < 32; i++)
        {
            randomBits ^= rand() << i;
        }
    return randomBits;
}

void ChessBoard::InitZobrist(){
    for (int i=0 ; i<120 ; ++i){
        for (int j=0 ; j<12 ; ++j){
            ZobristHash[i][j] = RandBits();
        }
    }
    for (int i=0; i<64 ; ++i){
        ZobristEnPassant[i] = RandBits();
    }
}

void ChessBoard::GenerateHash(){
    hash = 0;
    for (int i=21; i<100 ; ++i){
        if(board[i] != 0 && board[i] != 7)
            hash ^= ZobristHash[i][BoardToZobrist[board[i]]];
    }
    if(ep != -1)
        hash ^= ZobristEnPassant[ep];
}


/////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Pseudo legális lépésekhez adás függvényei
///
/////////////////////////////////////////////////////////////////////////////////////////////////

/// megvizsgálja hogy az x integernek az n-edik bitje 1(true) vagy 0(false)
bool ChessBoard::isNthBitSet(int x, int n){
    if (x & (1 << (n-1)))
        return true;
    else
        return false;
}

bool ChessBoard::isMoveType(int bits, int type){
    return (bits & type);
}

/// A lépésgeneráláshoz használt funkció mely hozzáadja a moves tömbhöz a megfelelő lépést
void ChessBoard::addToMoves(int from, int to, int moveT){
    if (moveT & 16){
        if (currentPlayer){
            if (to > 90){ 
                addPromToMoves(from, to, moveT); /// 4 lépést generálunk lsd.: addPromToMoves
                return; /// ne folytatódjon a függvény
            }
        }else{
            if (to < 29){
                addPromToMoves(from, to, moveT); /// 4 lépés a proóció
                return; /// ne folytatódjon a függvény
            }
        }
    }

    genMove *cmove;
    /// a tömb adott elemére rámutatunk, és megnöveljük a számlálót, hogy honnan következik a következő mélység első eleme
    cmove = &moves[firstMoveOfDepth[currentDepth+1]++];

    /// majd beállítjuk a rámutatott tömb eleména  megfelelő adatokat
    cmove->m.b.from = (char)from;
    cmove->m.b.to = (char)to;
    /// mivel itt már ez biztos nem promóció, ugyanis ezeket fent kezeltük és vissza is tértünk,
    /// ezért ezt az elemet 0 ra állítjuk
    cmove->m.b.promote = 0;
    cmove->m.b.moveType = (char)moveT;

    if (board[to] != 0){ /// azaz ütés történik
        /// a pontszám egy MVV/LVA alapján működik
        /// Most Valuable Victim / Least Valuable Attacker
        /// azaz legértékesebb áldozat/ legértéktelenebb támadó
        /// gyakorlatban a támadó értékét kivonjuk a végleges értékből
        /// a leütött bábu értékét pedig hozzáadjuk
        /// továbbá egy konstans nag számmal toldjuk az integerünket, mivel ütés
        /// és később a move ordering előbb vegye az ütéseket mint a nem ütéseket
        cmove->score = 1000000 + (pieceToMoveValue[board[to]] - pieceToMoveValue[board[allPieces[from]]]);
    }else{ // ellenkező esetben sima lépés
        if(currentPlayer)
            cmove->score = histoH[0][allPieces[from]][to];
        else
            cmove->score = histoH[1][allPieces[from]][to];
    }
}

void ChessBoard::addPromToMoves(int from, int to, int moveT){
    genMove *cmove;
    /// 4 bábuvá váltohat egy gyalog
    for (int i=0 ; i<4 ; ++i){
        cmove = &moves[firstMoveOfDepth[currentDepth+1]++];
        cmove->m.b.from = (char)from;
        cmove->m.b.to = (char)to;
        cmove->m.b.moveType = (char)(moveT | 32); /// így bármilyen lépés hozzátesszük, hogy promótáló lépés
        
        /// hozzáadjuk a bábu értékét amivé változni fog, de kivonjuk a gyalog értékét ugyanis az elveszik
        cmove->score = 1000000 + pieceToMoveValue[i+2] - pieceToMoveValue[1];
        if(currentPlayer){
            cmove->m.b.promote = (i+2);
        }else{
            cmove->m.b.promote = -(i+2);
        }
        
    }
}

/*************************************************************************************************
; 
; WHITE move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::whiteKnightMoveGeneration(int pieceIndex){
    int currentWhiteKnight = allPieces[pieceIndex];
    if (currentWhiteKnight != -1){
        for (int i=0 ; i<8 ; ++i){
            if (board[currentWhiteKnight + knightOffsets[i]] < 1){ 
                if (board[currentWhiteKnight + knightOffsets[i]] < 0){ 
                    addToMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], 1);
                }else{ 
                    addToMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], 0);
                }
            }
        }
    }
}

void ChessBoard::whiteRookMoveGeneration(int pieceIndex){
    int currentWhiteRook = allPieces[pieceIndex];
    int j;
    if (currentWhiteRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteRook + j * rookOffsets[i]] == 0){
                addToMoves(pieceIndex, currentWhiteRook + j * rookOffsets[i], 0);
                ++j;                
            }
            if (board[currentWhiteRook + j * rookOffsets[i]] < 0){
                addToMoves(pieceIndex, currentWhiteRook + j * rookOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            }
        }
    }
}

void ChessBoard::whiteBishopMoveGeneration(int pieceIndex){
    int currentWhiteBishop = allPieces[pieceIndex];
    int j;
    if(currentWhiteBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteBishop + j * bishopOffsets[i]] == 0){
                addToMoves(pieceIndex, currentWhiteBishop + j * bishopOffsets[i], 0);
                ++j;
            }
            if (board[currentWhiteBishop + j * bishopOffsets[i]] < 0){
                addToMoves(pieceIndex, currentWhiteBishop + j * bishopOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            }
                
        }
    }
}

void ChessBoard::whiteQueenMoveGeneration(int pieceIndex){
    int whiteQueen = allPieces[pieceIndex];
    int j;
    if(whiteQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteQueen + j * bishopOffsets[i]] == 0){
                addToMoves(pieceIndex, whiteQueen + j * bishopOffsets[i], 0);
                ++j;
            }
            if (board[whiteQueen + j * bishopOffsets[i]] < 0){
                addToMoves(pieceIndex, whiteQueen + j * bishopOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            } 
            j = 1;
            while (board[whiteQueen + j * rookOffsets[i]] == 0){
                addToMoves(pieceIndex, whiteQueen + j * rookOffsets[i], 0);
                j++;
            }
            if (board[whiteQueen + j * rookOffsets[i]] < 0){
                addToMoves(pieceIndex, whiteQueen + j * rookOffsets[i], 1);
                // the last move is a capture || else doesn't matter, either white piece or off the table
            }
        }
    } 
}

void ChessBoard::whiteKingMoveGeneration(){
    int whiteKing = allPieces[15];
    for (int i=0 ; i<4 ; ++i){
        if (board[whiteKing + bishopOffsets[i]] < 1){
            if(board[whiteKing + bishopOffsets[i]] < 0){
                addToMoves(15, whiteKing + bishopOffsets[i], 1);
            }
            else{
                addToMoves(15, whiteKing + bishopOffsets[i], 0);
            }
                
        }
        if (board[whiteKing + rookOffsets[i]] < 1){
            if(board[whiteKing + rookOffsets[i]] < 0){
                addToMoves(15, whiteKing + rookOffsets[i], 1);
            }    
            else{
                addToMoves(15, whiteKing + rookOffsets[i], 0);
            }
        }
    }
}

void ChessBoard::whiteEnPassantGeneration(){
    if(ep != -1){
        if(board[ep-9] == 1)
            addToMoves(findPiece(ep-9), ep, 21); // 21 == ütés 1 + en passan 4 + gyalog mozgás 16
        if(board[ep-11] == 1)
            addToMoves(findPiece(ep-11), ep, 21); // 21 == ütés 1 + en passan 4 + gyalog mozgás 16
    }
}

void ChessBoard::whitePawnMove(){
    for(int index=0 ; index<8 ; ++index){
        if(allPieces[index] != -1 && whitePromoted[index] == 0){
            int currentPawn = allPieces[index];
            if(board[currentPawn + 9] < 0)
                addToMoves(index, currentPawn + 9, 17);
            if(board[currentPawn + 11] < 0)
                addToMoves(index, currentPawn + 11, 17);
            if(board[currentPawn + 10] == 0){
                addToMoves(index, currentPawn + 10, 16);
                /// itt mivel már tudjuk hogy tud egyet előre lépni meg kell vizsgálni hoyg a kezdőpozíción van e és ha igen akkor tud e kettőt lépni
                if(currentPawn / 10 == 3 && board[currentPawn + 20] == 0)
                    addToMoves(index, currentPawn + 20, 24); // 16 pawn move 8 double pawn push
            }
        }
    }
}

void ChessBoard::whiteCastleMoveGeneration(){
    if(castleFlag & 1)
        addToMoves(15, 27, 2); /// allPieces[15] is the king and the board[27] is the king side castle destination, 2 is the castle move
    if(castleFlag & 2)
        addToMoves(15, 23, 2); /// queen side
}

/*************************************************************************************************
; 
; Az ütés generáló függvények
; 
**************************************************************************************************/

void ChessBoard::whiteKnightTakeGeneration(int pieceIndex){
    int currentWhiteKnight = allPieces[pieceIndex];
    if (currentWhiteKnight != -1){
        for (int i=0 ; i<8 ; ++i){
            if (board[currentWhiteKnight + knightOffsets[i]] < 0)
                addToMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], 1);
        }
    }
}

void ChessBoard::whiteRookTakeGeneration(int pieceIndex){
    int currentWhiteRook = allPieces[pieceIndex];
    int j;
    if (currentWhiteRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteRook + j * rookOffsets[i]] == 0)
                ++j;                
            if (board[currentWhiteRook + j * rookOffsets[i]] < 0)
                addToMoves(pieceIndex, currentWhiteRook + j * rookOffsets[i], 1);
        }
    }
}

void ChessBoard::whiteBishopTakeGeneration(int pieceIndex){
    int currentWhiteBishop = allPieces[pieceIndex];
    int j;
    if(currentWhiteBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentWhiteBishop + j * bishopOffsets[i]] == 0)
                ++j;
            if (board[currentWhiteBishop + j * bishopOffsets[i]] < 0)
                addToMoves(pieceIndex, currentWhiteBishop + j * bishopOffsets[i], 1);             
        }
    }
}

void ChessBoard::whiteQueenTakeGeneration(int pieceIndex){
    int whiteQueen = allPieces[pieceIndex];
    int j;
    if(whiteQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[whiteQueen + j * bishopOffsets[i]] == 0)
                ++j;
            if (board[whiteQueen + j * bishopOffsets[i]] < 0){
                addToMoves(pieceIndex, whiteQueen + j * bishopOffsets[i], 1);
            
            } 
            j = 1;
            while (board[whiteQueen + j * rookOffsets[i]] == 0)
                j++;
            if (board[whiteQueen + j * rookOffsets[i]] < 0)
                addToMoves(pieceIndex, whiteQueen + j * rookOffsets[i], 1);
        }
    } 
}

void ChessBoard::whiteKingTakeGeneration(){
    int whiteKing = allPieces[15];
    for (int i=0 ; i<4 ; ++i){
        if (board[whiteKing + bishopOffsets[i]] < 0)
            addToMoves(15, whiteKing + bishopOffsets[i], 1);
        if (board[whiteKing + rookOffsets[i]] < 0)
            addToMoves(15, whiteKing + rookOffsets[i], 1);
    }
}


void ChessBoard::whitePawnTake(){
    for(int index=0 ; index<8 ; ++index){
        if(allPieces[index] != -1 && whitePromoted[index] == 0){
            int currentPawn = allPieces[index];
            if(board[currentPawn + 9] < 0)
                addToMoves(index, currentPawn + 9, 17);
            if(board[currentPawn + 11] < 0)
                addToMoves(index, currentPawn + 11, 17);
        }
    }
}

void ChessBoard::callAllWhiteTake(){
    whiteRookTakeGeneration(8);
    whiteRookTakeGeneration(9);
    whiteBishopTakeGeneration(12);
    whiteBishopTakeGeneration(13);
    whiteQueenTakeGeneration(14);
    whiteKingTakeGeneration();
    whiteKnightTakeGeneration(10);
    whiteKnightTakeGeneration(11);
    whiteEnPassantGeneration();
    whitePawnTake();
    promotedPiecesTakeGeneration(true);
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

///összes meghívása
void ChessBoard::callAllWhiteMoveGeneration(){
    whiteRookMoveCall();
    whiteBishopMoveCall();
    whiteQueenMoveCall();
    whiteKingMoveCall();
    whiteKnightMoveCall();
    promotedPiecesMoveGeneration(true);

    /// uj cucc
    whitePawnMove();
    whiteEnPassantGeneration();
    whiteCastleMoveGeneration();
}

/*************************************************************************************************
; 
; BLACK move generation functions for each piece
; 
**************************************************************************************************/

void ChessBoard::blackKnightMoveGeneration(int pieceIndex){
    int currentBlackKnight = allPieces[pieceIndex];
    if (currentBlackKnight != -1){  /// both white knights are in the game
        for (int i=0 ; i<8 ; ++i){
            if (board[currentBlackKnight + knightOffsets[i]] > -1 && board[currentBlackKnight + knightOffsets[i]] < 7){ // left knight
                if (board[currentBlackKnight + knightOffsets[i]] > 0){
                    addToMoves(pieceIndex, currentBlackKnight + knightOffsets[i], 1);
                }else{
                    addToMoves(pieceIndex, currentBlackKnight + knightOffsets[i], 0);
                }
            }
        }
    }
}

void ChessBoard::blackKingMoveGeneration(){
    int blackKing = allPieces[31];
    for (int i=0 ; i<4 ; ++i){
        if (board[blackKing + bishopOffsets[i]] > -1 && board[blackKing + bishopOffsets[i]] < 7){
            if(board[blackKing + bishopOffsets[i]] > 0){
                addToMoves(31, blackKing + bishopOffsets[i], 1);
            }
            else{
                addToMoves(31, blackKing + bishopOffsets[i], 0);
            }
        }
        if (board[blackKing + rookOffsets[i]] > -1 && board[blackKing + rookOffsets[i]] < 7){
            if(board[blackKing + rookOffsets[i]] > 0){
                addToMoves(31, blackKing + rookOffsets[i], 1);
            }
            else{
                addToMoves(31, blackKing + rookOffsets[i], 0);
            }
        }
    }
}

void ChessBoard::blackRookMoveGeneration(int pieceIndex){
    int currentBlackRook = allPieces[pieceIndex];
    int j;
    if (currentBlackRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackRook + j * rookOffsets[i]] == 0){
                addToMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], 0);
                ++j;
            }
            if (board[currentBlackRook + j * rookOffsets[i]] > 0 && board[currentBlackRook + j * rookOffsets[i]] < 7){
                addToMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either black piece or off the table
            }
        }
    }
}

void ChessBoard::blackBishopMoveGeneration(int pieceIndex){
    int currentBlackBishop = allPieces[pieceIndex];
    int j;
    if(currentBlackBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackBishop + j * bishopOffsets[i]] == 0){ // left bishop
                addToMoves(pieceIndex, currentBlackBishop + j * bishopOffsets[i], 0);
                ++j;
            }
            if (board[currentBlackBishop + j * bishopOffsets[i]] > 0 && board[currentBlackBishop + j * bishopOffsets[i]] < 7){
                addToMoves(pieceIndex, currentBlackBishop + j * bishopOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either black piece or off the table
            }
        }
    }   
}

void ChessBoard::blackQueenMoveGeneration(int pieceIndex){
    int blackQueen = allPieces[pieceIndex];
    int j;
    if(blackQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackQueen + j * bishopOffsets[i]] == 0){
                addToMoves(pieceIndex, blackQueen + j * bishopOffsets[i], 0);
                ++j;
            }
            if (board[blackQueen + j * bishopOffsets[i]] > 0 && board[blackQueen + j * bishopOffsets[i]] < 7 ){
                addToMoves(pieceIndex, blackQueen + j * bishopOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            }            
            j = 1;
            while (board[blackQueen + j * rookOffsets[i]] == 0){
                addToMoves(pieceIndex, blackQueen + j * rookOffsets[i], 0);
                j++;
            }
            if (board[blackQueen + j * rookOffsets[i]] > 0 && board[blackQueen + j * rookOffsets[i]] < 7){
                addToMoves(pieceIndex, blackQueen + j * rookOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either white piece or off the table
            }
        }
    } 
}

void ChessBoard::blackEnPassantGeneration(){
    if(ep != -1){
        if(board[ep+9] == -1)
            addToMoves(findPiece(ep+9), ep, 21); // 21 == ütés 1 + en passan 4 + gyalog mozgás 16
        if(board[ep+11] == -1)
            addToMoves(findPiece(ep+11), ep, 21); // 21 == ütés 1 + en passan 4 + gyalog mozgás 16
    }
}

void ChessBoard::blackPawnMove(){
    for(int index=16 ; index<24 ; ++index){
        if(allPieces[index] != -1 && blackPromoted[index - 16] == 0){
            int currentPawn = allPieces[index];
            if(board[currentPawn - 9] > 0 && board[currentPawn - 9] < 7)
                addToMoves(index, currentPawn - 9, 17);
            if(board[currentPawn - 11] > 0 && board[currentPawn - 11] < 7)
                addToMoves(index, currentPawn - 11, 17);
            if(board[currentPawn - 10] == 0){
                addToMoves(index, currentPawn - 10, 16);
                /// itt mivel már tudjuk hogy tud egyet előre lépni meg kell vizsgálni hoyg a kezdőpozíción van e és ha igen akkor tud e kettőt lépni
                if(currentPawn / 10 == 8 && board[currentPawn - 20] == 0)
                    addToMoves(index, currentPawn - 20, 24); // 16 pawn move 8 double pawn push
            }
        }
    }
}

void ChessBoard::blackCastleMoveGeneration(){
    if(castleFlag & 4)
        addToMoves(31, 97, 2); // king side 
    if(castleFlag & 8)
        addToMoves(31, 93, 2); // queen side
}


/*************************************************************************************************
; 
; Az ütés generálő függvények
; 
**************************************************************************************************/

void ChessBoard::blackKnightTakeGeneration(int pieceIndex){
    int currentBlackKnight = allPieces[pieceIndex];
    if (currentBlackKnight != -1){
        for (int i=0 ; i<8 ; ++i){
            if (board[currentBlackKnight + knightOffsets[i]] > 0 && board[currentBlackKnight + knightOffsets[i]] < 7) 
                addToMoves(pieceIndex, currentBlackKnight + knightOffsets[i], 1);
        }
    }
}

void ChessBoard::blackKingTakeGeneration(){
    int blackKing = allPieces[31];
    for (int i=0 ; i<4 ; ++i){
        if (board[blackKing + bishopOffsets[i]] > 0 && board[blackKing + bishopOffsets[i]] < 7)
            addToMoves(31, blackKing + bishopOffsets[i], 1);

        if (board[blackKing + rookOffsets[i]] > 0 && board[blackKing + rookOffsets[i]] < 7)
                addToMoves(31, blackKing + rookOffsets[i], 1);
    }
}

void ChessBoard::blackRookTakeGeneration(int pieceIndex){
    int currentBlackRook = allPieces[pieceIndex];
    int j;
    if (currentBlackRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackRook + j * rookOffsets[i]] == 0)
                ++j;
            if (board[currentBlackRook + j * rookOffsets[i]] > 0 && board[currentBlackRook + j * rookOffsets[i]] < 7)
                addToMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], 1);
        }
    }
}

void ChessBoard::blackBishopTakeGeneration(int pieceIndex){
    int currentBlackBishop = allPieces[pieceIndex];
    int j;
    if(currentBlackBishop != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackBishop + j * bishopOffsets[i]] == 0){ // left bishop
                ++j;
            }
            if (board[currentBlackBishop + j * bishopOffsets[i]] > 0 && board[currentBlackBishop + j * bishopOffsets[i]] < 7){
                addToMoves(pieceIndex, currentBlackBishop + j * bishopOffsets[i], 1);
                /// the last move is a capture || else doesn't matter, either black piece or off the table
            }
        }
    }   
}

void ChessBoard::blackQueenTakeGeneration(int pieceIndex){
    int blackQueen = allPieces[pieceIndex];
    int j;
    if(blackQueen != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[blackQueen + j * bishopOffsets[i]] == 0)
                ++j;
            if (board[blackQueen + j * bishopOffsets[i]] > 0 && board[blackQueen + j * bishopOffsets[i]] < 7 )
                addToMoves(pieceIndex, blackQueen + j * bishopOffsets[i], 1);
                            
            j = 1;
            while (board[blackQueen + j * rookOffsets[i]] == 0)
                ++j;
            if (board[blackQueen + j * rookOffsets[i]] > 0 && board[blackQueen + j * rookOffsets[i]] < 7)
                addToMoves(pieceIndex, blackQueen + j * rookOffsets[i], 1);
        }
    } 
}

void ChessBoard::blackPawnTake(){
    for(int index=16 ; index<24 ; ++index){
        if(allPieces[index] != -1 && blackPromoted[index - 16] == 0){
            int currentPawn = allPieces[index];
            if(board[currentPawn - 9] > 0 && board[currentPawn - 9] < 7)
                addToMoves(index, currentPawn - 9, 17);
            if(board[currentPawn - 11] > 0 && board[currentPawn - 11] < 7)
                addToMoves(index, currentPawn - 11, 17);
        }
    }
}

void ChessBoard::callAllBlackTake(){
    blackRookTakeGeneration(24);
    blackRookTakeGeneration(25);
    blackBishopTakeGeneration(28);
    blackBishopTakeGeneration(29);
    blackQueenTakeGeneration(30);
    blackKingTakeGeneration();
    blackKnightTakeGeneration(26);
    blackKnightTakeGeneration(27);
    blackEnPassantGeneration();
    blackPawnTake();
    promotedPiecesTakeGeneration(false);
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

///összes meghívása
void ChessBoard::callAllBlackMoveGeneration(){
    blackRookMoveCall();
    blackBishopMoveCall();
    blackQueenMoveCall();
    blackKingMoveCall();
    blackKnightMoveCall();
    promotedPiecesMoveGeneration(false);

    /// uj cucc
    blackPawnMove();
    blackEnPassantGeneration();
    blackCastleMoveGeneration();
}


////Promótált bábuk függvénye
void ChessBoard::promotedPiecesMoveGeneration(bool player){
    if(player){
        for (int i=0; i<8 ; ++i){
            if(allPieces[i]>0){
                switch (whitePromoted[i])
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
                        break;
                    default:
                        break;
                }
            }
        }
    }else{
        for (int i=0; i<8 ; ++i){
            if(allPieces[i+16] > 0){
                switch (blackPromoted[i])
                {
                    case -5:
                        blackQueenMoveGeneration(i+16);
                        break;
                    case -2:
                        blackKnightMoveGeneration(i+16);
                        break;
                    case -3:
                        blackBishopMoveGeneration(i+16);
                        break;
                    case -4:
                        blackRookMoveGeneration(i+16);
                        break;
                    default:
                        break;
                }
            }
            
        }
    }
}

void ChessBoard::promotedPiecesTakeGeneration(bool player){
    if(player){
        for (int i=0; i<8 ; ++i){
            if(allPieces[i] > 0){
                switch (whitePromoted[i])
                {
                    case 5:
                        whiteQueenTakeGeneration(i);
                        break;
                    case 2:
                        whiteKnightTakeGeneration(i);
                        break;
                    case 3:
                        whiteBishopTakeGeneration(i);
                        break;
                    case 4:
                        whiteRookTakeGeneration(i);
                        break;
                    default:
                        break;
                }
            }
        }
    }else{
        for (int i=0; i<8 ; ++i){
            if(allPieces[i+16] > 0){
                switch (blackPromoted[i])
                {
                    case -5:
                        blackQueenTakeGeneration(i+16);
                        break;
                    case -2:
                        blackKnightTakeGeneration(i+16);
                        break;
                    case -3:
                        blackBishopTakeGeneration(i+16);
                        break;
                    case -4:
                        blackRookTakeGeneration(i+16);
                        break;
                    default:
                        break;
                }
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
    firstMoveOfDepth[currentDepth+1] = firstMoveOfDepth[currentDepth];
    if(currentPlayer){ /// current player is true when it's white's turn
        //std::cout << "white's turn" << std::endl;
        callAllWhiteMoveGeneration();
    }else{ /// current player is false when it's black's turn
        //std::cout << "black's turn" << std::endl;
        callAllBlackMoveGeneration();
    }
}

void ChessBoard::generatePseudoLegalCaptures(){
    firstMoveOfDepth[currentDepth+1] = firstMoveOfDepth[currentDepth];
    if(currentPlayer){ /// current player is true when it's white's turn
        //std::cout << "white's turn" << std::endl;
        callAllWhiteTake();
    }else{ /// current player is false when it's black's turn
        //std::cout << "black's turn" << std::endl;
        callAllBlackTake();
    }
}

bool ChessBoard::MakeMove(moveBytes moveData){
    // először megvizsgáljuk a sáncolás lehetőségét
    if(moveData.moveType & 2){
        if(inCheck(currentPlayer))
            return false;
        int fromRook, toRook;
        switch(moveData.to){
            case 27:
                if(board[26] != 0 || board[27] != 0 || attack(26, !currentPlayer) || attack(27, !currentPlayer))
                    return false;
                fromRook = 9; // a bástya indexe az allPieces tömbben
                toRook = 26; // a célmező indexe a board tömbben
                break;
            case 23:
                if(board[24] != 0 || board[23] != 0 || board[22] != 0 || attack(24, !currentPlayer) || attack(23, !currentPlayer))
                    return false;
                fromRook = 8; // a bástya indexe az allPieces tömbben
                toRook = 24; // a célmező indexe a board tömbben
                break;
            case 97:
                if(board[96] != 0 || board[97] != 0 || attack(96, !currentPlayer) || attack(97, !currentPlayer))
                    return false;
                fromRook = 25;
                toRook = 96;
                break;
            case 93:
                if(board[94] != 0 || board[93] != 0 || board[92] != 0 || attack(94, !currentPlayer) || attack(93, !currentPlayer))
                    return false;
                fromRook = 24;
                toRook = 94;
                break;
            default: // ide nem juthatunk el
                fromRook = -1;
                toRook = -1;
                break; 
        }
        // sáncolás esetén előbb leléptetjük a bástyát, majd a királyt később a sima karaktermozgatás fogja intézni
        if(currentPlayer){
            board[toRook] = 4;
            board[allPieces[fromRook]] = 0;
            allPieces[fromRook] = toRook;
        }else{
            board[toRook] = -4;
            board[allPieces[fromRook]] = 0;
            allPieces[fromRook] = toRook;
        }
    }

    /// változtatás előtt eltároljuk a moveHistory tömbben a lépés szükséges adatit
    /// ha vissza kéne csinálni minden rendelkezésre áljon, hogy ezt megtehessük
    movesHistory[historyDepth].m.b = moveData;
    movesHistory[historyDepth].capture = board[int(moveData.to)];
    movesHistory[historyDepth].ep = ep;
    movesHistory[historyDepth].castleFlag = castleFlag;
    movesHistory[historyDepth].fifthy = fifthyMove;
    movesHistory[historyDepth].boardFrom = allPieces[moveData.from];
    movesHistory[historyDepth].castled = castled;
    if(moveData.moveType & 4){
        if(currentPlayer){
            movesHistory[historyDepth].toAllPieces = findPiece(moveData.to - 10);
        } 
        else{
            movesHistory[historyDepth].toAllPieces = findPiece(moveData.to + 10);
        }
    }else if(moveData.moveType & 1) {
        movesHistory[historyDepth].toAllPieces = findPiece(moveData.to);
    }else{
        movesHistory[historyDepth].toAllPieces = -1;
    }
    std::string moveStr = moveToString(moveData);
    for(int i=0 ; i<5 ; ++i){
        movesHistory[historyDepth].moveString[i] = moveStr[i];
    }

    movesHistory[historyDepth].hash = hash;
    ++currentDepth;
    ++historyDepth;


    ///////
    /// A sáncolás Flagjének frissítése
    ///////
    if(moveData.moveType & 2){ // ha sáncolás a lépés
        if(moveData.to < 29) // ha fehér oldali sánc történt 
        {
            castleFlag &= 12; // "és"eljük 12 vel (1100) így az 1 es és 2 es helyiérték bitje biztos 0 lesz
            castled ^= 1; 
        }
        else // ha fekete oldali sánc történt akkor éseljük 12 vel (1100) így az 1 es és 2 es helyiérték bitje biztos 0 lesz
        {
            castleFlag &= 3; //a fekete oldalon ugyanígy járunk el
            castled ^= 2; 
        }   
    }else{
        // a moveData.to azaz a cél esetében elég azt vizgsálni, hogy a cél az a bástya helye-e
        // ha igen akkor az adott oldalra már nem leht sáncolni
        // ha a moveData.to a király pozíciója, akkor a király már biztos ellépett onnan, azaz nem lehetséges a sáncolás értelmetlen a vizsgálat és az átállítás
        switch(moveData.to){ 
            case 28:
                castleFlag &= 14; /// fehér király oldali 14 == 1110
                break;
            case 21:
                castleFlag &= 13; /// fehér királynő oldal 13 == 1101
                break;
            case 98:
                castleFlag &= 11; /// 1011
                break;
            case 91:
                castleFlag &= 7; /// 0111
                break;
            default:
                break;
        }
        // a moveData.from az allPieces ben lévő indexeket tárolja nem a pályán lévő indexet !!!
        switch(moveData.from){
            // a fehér király mozog 
            case 15:
                castleFlag &= 12; // 1100 a fehér oldali sáncolás nullázása
                break;
            case 31:
                castleFlag &= 3; // 0011 a fekete oldali sáncolás nullázása
                break;
            case 9:
                castleFlag &= 14; /// fehér király oldali 14 == 1110
                break;
            case 8:
                castleFlag &= 13; /// fehér királynő oldal 13 == 1101
                break;
            case 25:
                castleFlag &= 11; /// 1011
                break;
            case 24:
                castleFlag &= 7; /// 0111
                break;
            default:
                break;
        }
    }

    // en passan frissítése
    if(moveData.moveType & 8){
        if(currentPlayer){
            ep = moveData.to - 10;
        }else{
            ep = moveData.to + 10;
        }
    }else{
        ep = -1;
    }

    // 50 lépéses szabály frissítése
    if(moveData.moveType & 17)
        fifthyMove = 0;
    else
        fifthyMove++;

    // a lépés megtétele
    if(moveData.moveType & 32){ // bábu átalakítás
        if(moveData.moveType & 1)
            allPieces[findPiece(moveData.to)] = -1;
        board[moveData.to] = moveData.promote;
        board[allPieces[moveData.from]] = 0; // ahonnan léptünk 0 ra állítjuk
        allPieces[moveData.from] = moveData.to;
        if(currentPlayer)
            whitePromoted[moveData.from] = moveData.promote;
        else
            blackPromoted[moveData.from-16] = moveData.promote;
        movesHistory[historyDepth-1].promotedToIndex = moveData.from;
    }else{
        movesHistory[historyDepth-1].promotedToIndex = -1;
        if(moveData.moveType & 4){ /// en passant esetén
            if(currentPlayer){ // ha fehér üt
                board[allPieces[findPiece(moveData.to - 10)]] = 0; 
                allPieces[findPiece(moveData.to - 10)] = -1;
                board[moveData.to] = 1;
                board[allPieces[moveData.from]] = 0;
                allPieces[moveData.from] = moveData.to;
            }else{ // ha fekete üt
                board[allPieces[findPiece(moveData.to + 10)]] = 0;
                allPieces[findPiece(moveData.to + 10)] = -1;
                board[moveData.to] = -1;
                board[allPieces[moveData.from]] = 0;
                allPieces[moveData.from] = moveData.to;
            }
        }else{
            if(moveData.moveType & 1){ // sima ütés
                allPieces[findPiece(moveData.to)] = -1;
                board[moveData.to] = board[allPieces[moveData.from]];
                board[allPieces[moveData.from]] = 0;
                allPieces[moveData.from] = moveData.to;
            }else{ // lépés
                board[moveData.to] = board[allPieces[moveData.from]];
                board[allPieces[moveData.from]] = 0;
                allPieces[moveData.from] = moveData.to;
            }
        }   
    }
    
    currentPlayer = !currentPlayer;
    if(inCheck(!currentPlayer)){
        TakeBack();
        return false;
    }
    
    GenerateHash();
    
    return true;
}

// egy lépés visszacsinálása
// 
void ChessBoard::TakeBack(){
    if(currentDepth == 0)
        return;
    moveBytes m;

    currentPlayer = !currentPlayer;
    --currentDepth;
    --historyDepth;
    
    m = movesHistory[historyDepth].m.b;
    castleFlag = movesHistory[historyDepth].castleFlag;
    ep = movesHistory[historyDepth].ep;
    fifthyMove = movesHistory[historyDepth].fifthy;
    hash = movesHistory[historyDepth].hash;
    castled = movesHistory[historyDepth].castled;

    // gyalog átalakítás
    if (m.moveType & 32){ 
        if(currentPlayer)
            whitePromoted[m.from] = 0;
        else
            blackPromoted[m.from - 16] = 0;
        board[movesHistory[historyDepth].boardFrom] = currentPlayer ? 1 : -1;
        allPieces[m.from] = movesHistory[historyDepth].boardFrom;
        if(m.moveType & 1){
            allPieces[movesHistory[historyDepth].toAllPieces] = m.to;
            board[m.to] = movesHistory[historyDepth].capture;
        }else{
            board[m.to] = 0;
        }
    }
    // sáncolás
    else if (m.moveType & 2){ 
        int fromRook, toRook;
        switch(m.to){
            case 27:
                fromRook = 9;
                toRook = 28;
                break;
            case 23:
                fromRook = 8;
                toRook = 21;
                break;
            case 97:
                fromRook = 25;
                toRook = 98;
                break;
            case 93:
                fromRook = 24;
                toRook = 91;
                break;
        }
        board[allPieces[fromRook]] = 0;
        allPieces[fromRook] = toRook;
        board[toRook] = currentPlayer ? 4 : -4;
        board[m.to] = 0;
        allPieces[m.from] = currentPlayer ? 25 : 95;
        board[allPieces[m.from]] = currentPlayer ? 6 : -6;
    }
    // en passant
    else if(m.moveType & 4){
        if(currentPlayer){
            board[m.to] = 0;
            allPieces[movesHistory[historyDepth].toAllPieces] = m.to - 10;
            board[m.to - 10] = -1;
            allPieces[m.from] = movesHistory[historyDepth].boardFrom;
            board[movesHistory[historyDepth].boardFrom] = 1;
        }else{
            board[m.to] = 0;
            allPieces[movesHistory[historyDepth].toAllPieces] = m.to + 10;
            board[m.to + 10] = 1;
            allPieces[m.from] = movesHistory[historyDepth].boardFrom;
            board[movesHistory[historyDepth].boardFrom] = -1;
        }
    }

    // ütés
    else if(m.moveType & 1){
        allPieces[m.from] = movesHistory[historyDepth].boardFrom;
        board[movesHistory[historyDepth].boardFrom] = board[m.to];
        board[m.to] = movesHistory[historyDepth].capture;
        allPieces[movesHistory[historyDepth].toAllPieces] = m.to;
    }
    // sima lépés
    else{
        allPieces[m.from] = movesHistory[historyDepth].boardFrom;
        board[movesHistory[historyDepth].boardFrom] = board[m.to];
        board[m.to] = 0;
    }   
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

/// megtalálja a bábut az allPieces Tömbben
int ChessBoard::findPiece(int index){
    for (int i=0 ; i<32 ; ++i){
        if (allPieces[i] == index)
            return i;
    }
    
    if(inParseMove)
        return -1;
    // ide nem szabadna eljutni ...
    
    std::cout << "NEGATIV INDEXET ADUNK VISSZA" << std::endl;
    std::cout << "a keresett index: " << int(index)  << std::endl;
    if(currentPlayer)
        std::cout << "FEHER VAN SORON" << std::endl;
    else
        std::cout << "FEKETE VAN SORON" << std::endl;
    //drawIntBoard();
    for(int z=0; z<32 ; ++z){
        std::cout << int(allPieces[z]) << " ";
    }
    std::cout << std::endl;
    std::cout << "white promoted : ";
    for(int z=0; z<8 ; ++z){
        std::cout << whitePromoted[z] << " ";
    }
    std::cout << std::endl;
    std::cout << "black promoted : ";
    for(int z=0; z<8 ; ++z){
        std::cout << blackPromoted[z] << " ";
    }
    std::cout << std::endl;
    drawIntBoard();
    std::cout << std::endl << std::endl;

    return -1;
}

std::string ChessBoard::moveToString(moveBytes move){
    if(move.to > 98 || move.to < 21)
        return "00000";
    std::string moveString = indexToPos[allPieces[move.from]] + indexToPos[move.to];
    
    if (move.promote != 0)
        moveString = moveString.append(promoteNumberToStr[move.promote]);
    else
        moveString = moveString.append(" ");
    return moveString;
}