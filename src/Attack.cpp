#include "Board.h"

/// igazat ad vissza ha egy mező támadás alatt áll, hamisat ha nem áll támadás alatt, vagy nincs a pályán
/// két paramétere: sq - az indexe a mezőnek, side - a szín/oldal ami által támadva van-e az adott mező (true-fehér, false-fekete)
bool ChessBoard::attack(int sq, bool side){
    if (board[sq] == 7)
        return false;
    if (side){ // ha fehér a támadó fél
        for (int i=0; i < 8 ; ++i){ // csak a gyalogok
            if(allPieces[i] > 0){
                if(whitePromoted[i] == 0){
                    if ((allPieces[i] + 9) == sq)
                        return true;
                    if ((allPieces[i] + 11) == sq)
                        return true;
                }else{
                    switch(whitePromoted[i]){
                        case 5:
                            if(attackQueen(sq, i))
                                return true;
                            break;
                        case 2:
                            if(attackKnight(sq,i))
                                return true;
                            break;
                        case 3:
                            if(attackBishop(sq,i))
                                return true;
                            break;
                        case 4:
                            if(attackRook(sq,i))
                                return true;
                            break;
                        default:
                            break; 
                    }
                }
            }
        }
        
        if (attackRook(sq,8) || attackRook(sq,9))
            return true;
        if (attackKnight(sq,10) || attackKnight(sq,11))
            return true;
        if (attackBishop(sq,12) || attackBishop(sq,13))
            return true;
        if (attackQueen(sq, 14))
            return true;
        if (attackKing(sq, 15))
            return true;
    }

    else{ /// ha fekete a támadó fél
        for (int i=16; i < 24; ++i){ // csak a gyalogok
            if(allPieces[i] > 0){
                if(blackPromoted[i-16] == 0){
                    if ((allPieces[i] - 9) == sq)
                        return true;
                    if ((allPieces[i] - 11) == sq)
                        return true;
                }else{
                    switch(blackPromoted[i-16]){
                        case -5:
                            if(attackQueen(sq, i))
                                return true;
                            break;
                        case -2:
                            if(attackKnight(sq,i))
                                return true;
                            break;
                        case -3:
                            if(attackBishop(sq,i))
                                return true;
                            break;
                        case -4:
                            if(attackRook(sq,i))
                                return true;
                            break;
                        default:
                            break; 
                    }
                }
            }
        }
        if (attackRook(sq,24) || attackRook(sq,25))
            return true;
        if (attackKnight(sq,26) || attackKnight(sq,27))
            return true;
        if (attackBishop(sq,28) || attackBishop(sq,29))
            return true;
        if (attackQueen(sq, 30))
            return true;
        if (attackKing(sq, 31))
            return true;
    }

    // ha sehol sem igaz akkor nem támadott a mező azaz hamissal térünk vissza

    return false;
}

bool ChessBoard::inCheck(bool side){
    if(side){
        return attack(allPieces[15], !side);
    }else{
        return attack(allPieces[31], !side);
    }
    return false; // ide nem kéne eljutnia a kódnak sosem
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// Támadó funkciók implementálása
//////////////////////////////////////////////////////////////////////////////////////////////////

bool ChessBoard::attackRook(int sq, int attacker){
    if (/*allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0*/ allPieces[attacker] < 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (int i=0 ; i<4 ; ++i){
        int mul = 1;
        while(board[allPieces[attacker] + mul * rookOffsets[i]] == 0 && allPieces[attacker] + mul * rookOffsets[i] != sq)
            ++mul;
        if(allPieces[attacker] + mul * rookOffsets[i] == sq /*|| allPieces[attacker] + (mul-1) * rookOffsets[i] == sq*/)
            return true;
    }
    return false;
}

bool ChessBoard::attackBishop(int sq, int attacker){
    if (/*allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0*/ allPieces[attacker] < 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (int i=0 ; i<4 ; ++i){
        int mul = 1;
        while(board[allPieces[attacker] + mul * bishopOffsets[i]] == 0 && allPieces[attacker] + mul * bishopOffsets[i] != sq)
            ++mul;
        if(allPieces[attacker] + mul * bishopOffsets[i] == sq /*|| allPieces[attacker] + (mul-1) * bishopOffsets[i] == sq*/)
            return true;
    }
    return false;
}

bool ChessBoard::attackKnight(int sq, int attacker){
    if (/*allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0*/ allPieces[attacker] < 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for(int i=0 ; i<8 ; ++i){
        if(allPieces[attacker] + knightOffsets[i] == sq)
            return true;
    }
    return false;
}

bool ChessBoard::attackQueen(int sq, int attacker){
    if (/*allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0*/ allPieces[attacker] < 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (int i=0 ; i<4 ; ++i){
        /// az átlós (futó) támadás vizsgálata
        int mul = 1;
        while(board[allPieces[attacker] + mul * bishopOffsets[i]] == 0 && allPieces[attacker] + mul * bishopOffsets[i] != sq)
            ++mul;
        if(allPieces[attacker] + mul * bishopOffsets[i] == sq /*|| allPieces[attacker] + (mul-1) * bishopOffsets[i] == sq*/)
            return true;

        /// a vízszintes és függőleges (bástya) támadás vizsgálata
        mul = 1;
        while(board[allPieces[attacker] + mul * rookOffsets[i]] == 0 && allPieces[attacker] + mul * rookOffsets[i] != sq)
            ++mul;
        if(allPieces[attacker] + mul * rookOffsets[i] == sq /*|| allPieces[attacker] + (mul-1) * rookOffsets[i] == sq*/)
            return true;
    }
    return false;
}

bool ChessBoard::attackKing(int sq, int attacker){
    for(int i=0 ; i<4 ; ++i){
        if(allPieces[attacker] + rookOffsets[i] == sq)
            return true;
        if(allPieces[attacker] + bishopOffsets[i] == sq)
            return true;
    }
    return false;
}
