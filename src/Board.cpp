#include "Board.h"

void ChessBoard::initBoard(){
    int counter = 0;
    for (char x : {
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
    for (char x : { 31, 32, 33, 34, 35, 36, 37, 38, 21, 28, 22, 27, 23, 26, 24, 25, 
                           81, 82, 83, 84, 85, 86, 87, 88, 91, 98, 92, 97, 93, 96, 94, 95,
                           -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1})
    {
        allPieces[counter] = x;
        ++counter;
    }

    currentPlayer = true; 
    enPassant = false;
}

ChessBoard::ChessBoard(){
    initBoard();
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

/// igazat ad vissza ha egy mező támadás alatt áll, hamisat ha nem áll támadás alatt, vagy nincs a pályán
/// két paramétere: sq - az indexe a mezőnek, side - a szín/oldal ami által támadva van-e az adott mező (true-fehér, false-fekete)
bool ChessBoard::attack(char sq, bool side){
    if (board[sq] == 7)
        return false;
    if (side){ // ha fehér a támadó fél
        for (int i=0; i < 8 ; ++i){ // csak a gyalogok
            if(allPieces[i]+9 == sq)
                return true;
            if(allPieces[i]+11 == sq)
                return true;
        }
        if(attackRook(sq,8) || attackRook(sq,9))
            return true;
        if(attackKnight(sq,10) || attackKnight(sq,11))
            return true;
        if(attackBishop(sq,12) || attackBishop(sq,13))
            return true;
        if(attackQueen(sq, 14))
            return true;
    }
    else{ /// ha fekete a támadó fél
        for (int i=16; i < 24; ++i){ // csak a gyalogok
            if(allPieces[i]-9 == sq)
                return true;
            if(allPieces[i]-11 == sq)
                return true;
        }
        if(attackRook(sq,24) || attackRook(sq,25))
            return true;
        if(attackKnight(sq,26) || attackKnight(sq,27))
            return true;
        if(attackBishop(sq,28) || attackBishop(sq,29))
            return true;
        if(attackQueen(sq, 30))
            return true;
    }

    /// szükség van még a promótált bábuk checkolására is

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

bool ChessBoard::attackRook(char sq, char attacker){
    if (allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (char i=0 ; i<4 ; ++i){
        char mul = 1;
        while(board[allPieces[attacker] + mul * rookOffsets[i]] == 0)++mul;
        if(allPieces[attacker] + mul * rookOffsets[i] == sq)
            return true;
    }
    return false;
}

bool ChessBoard::attackBishop(char sq, char attacker){
    if (allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (char i=0 ; i<4 ; ++i){
        char mul = 1;
        while(board[allPieces[attacker] + mul * bishopOffsets[i]] == 0)++mul;
        if(allPieces[attacker] + mul * bishopOffsets[i] == sq)
            return true;
    }
    return false;
}

bool ChessBoard::attackKnight(char sq, char attacker){
    if (allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for(char i=0 ; i<4 ; ++i){
        if(allPieces[attacker] + knightOffsets[i] == sq)
            return true;
    }
    return false;
}

bool ChessBoard::attackQueen(char sq, char attacker){
    if (allPieces[attacker] > 0 && board[allPieces[attacker]] * board[sq] > 0) /// biztonsági ellenőzés hogy a támadott mezőn nem a támadó fél babuja áll, ez esetben a visszatérés kérdés nélkül false
        return false;
    for (char i=0 ; i<4 ; ++i){
        /// az átlós (futó) támadás vizsgálata
        char mul = 1;
        while(board[allPieces[attacker] + mul * bishopOffsets[i]] == 0)++mul;
        if(allPieces[attacker] + mul * bishopOffsets[i] == sq)
            return true;

        /// a vízszintes és függőleges (bástya) támadás vizsgálata
        mul = 1;
        while(board[allPieces[attacker] + mul * rookOffsets[i]] == 0)++mul;
        if(allPieces[attacker] + mul * rookOffsets[i] == sq)
            return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// A kiértékelő függvény
//////////////////////////////////////////////////////////////////////////////////////////////////

int ChessBoard::evaluation(){
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// minden értéket tároló tömb 2 elemű, melynek első eleme a fehér félre vonatkozik, a másodika fekete félre
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    /// a végleges összpontok
    int score[2];    
    /// jelenlegi oszlop
    int currentFile;
    /// jelenlegi sor
    int currentRank;

    /// a változók alaphelyzetbe állítása
    score[WHITE] = 0;
    score[BLACK] = 0;
    
    pieceSum[WHITE] = 0;
    pieceSum[BLACK] = 0;

    pawnSum[WHITE] = 0;
    pawnSum[BLACK] = 0;

    // beállítjuk az osztály pawnRank adattagját
    for(int i=0; i<10 ; ++i){
        pawnRank[WHITE][i] = 7;
        pawnRank[BLACK][i] = 0;
    }

    

    /// első körben csak a bábuk értékét adjuk össze, és a pawnRank tömböt helyesen feltöltjük
    for (int i=0 ; i<8 ; ++i){
        if (allPieces[i] != -1){
            pawnSum[WHITE] += numberToValueWhite[1];
            currentFile = (allPieces[i] % 10); /// nem kell kivonni egyet, ugyanis a 2. elemtől kezdődnek a gyalogok a tömbben
            currentRank = (allPieces[i] / 10) - 2;
            if (pawnRank[WHITE][currentFile] > currentRank)
                pawnRank[WHITE][currentFile] = currentRank;    
        }
        if (allPieces[i+16] != -1){
            pawnSum[BLACK] += numberToValueBlack[-1];
            currentFile = (allPieces[i+16] % 10); /// nem kell kivonni egyet, ugyanis a 2. elemtől kezdődnek a gyalogok a tömbben
            currentRank = (allPieces[i+16] / 10) - 2;
            if (pawnRank[BLACK][currentFile] < currentRank)
                pawnRank[BLACK][currentFile] = currentRank;
        }        
    }

    // összes többi bábu
    for (int i=8 ; i<15 ; ++i){
        if (allPieces[i] != -1){
            pieceSum[WHITE] += indexToValue[i];
        }
    }
    for (int i=24 ; i<31 ; ++i){
        if (allPieces[i] != -1){
            pieceSum[BLACK] += indexToValue[i];
        }
    }


    /// promótált bábuk
    int ind=32;
    while (ind < 48 && allPieces[ind] != -1){
        if (board[allPieces[ind]] > 0){
            pieceSum[WHITE] = numberToValueWhite[board[allPieces[ind]]];
        }
        else{
            pieceSum[BLACK] = numberToValueBlack[board[allPieces[ind]]];
        }
        ++ind;
    }

    ///////////////
    ///////////////
    /// 2. körben hozzáadjuk a bábukhoz az extra értékeiket a pozíciójuk alapján
    /// eval_tables.h tartalmazza az összes konstans táblát
    ///////////////
    ///////////////
    score[WHITE] = pieceSum[WHITE] + pawnSum[WHITE];
    score[BLACK] = pieceSum[BLACK] + pawnSum[BLACK];


    for (int i = 0; i<8 ; ++i){
        if(allPieces[i] != -1)
            score[WHITE] += whitePawnScore(allPieces[i]);
        if(allPieces[i+16] != -1)
            score[BLACK] += blackPawnScore(allPieces[i+16]);
    }

    /// Hozzáadjuk a pozíció alapú értéket a futókhoz és lovakhoz
    score[WHITE] += knight_pcsq[convert_table_white[allPieces[10]]] 
                  + knight_pcsq[convert_table_white[allPieces[11]]]
                  + bishop_pcsq[convert_table_white[allPieces[12]]]
                  + bishop_pcsq[convert_table_white[allPieces[13]]];

    score[BLACK] += knight_pcsq[convert_table_black[allPieces[26]]] 
                  + knight_pcsq[convert_table_black[allPieces[27]]]
                  + bishop_pcsq[convert_table_black[allPieces[28]]]
                  + bishop_pcsq[convert_table_black[allPieces[29]]];

    /// bástya kiértékelése
    /// fehér
    if (pawnRank[WHITE][allPieces[8]%10] == 7) {
		if (pawnRank[BLACK][allPieces[8]%10] == 0)
			score[WHITE] += ROOK_OPEN_FILE_BONUS;
        else
			score[WHITE] += ROOK_SEMI_OPEN_FILE_BONUS;
	}
	if (allPieces[8]/10 -2 == 6)
		score[WHITE] += ROOK_ON_SEVENTH_BONUS;

    if (pawnRank[WHITE][allPieces[9]%10] == 7) {
		if (pawnRank[BLACK][allPieces[9]%10] == 0)
			score[WHITE] += ROOK_OPEN_FILE_BONUS;
        else
			score[WHITE] += ROOK_SEMI_OPEN_FILE_BONUS;
	}
	if (allPieces[9]/10 - 2  == 6)
		score[WHITE] += ROOK_ON_SEVENTH_BONUS;

    /// fekete
    if (pawnRank[BLACK][allPieces[24]%10] == 0) {
		if (pawnRank[WHITE][allPieces[24]%10] == 7)
			score[BLACK] += ROOK_OPEN_FILE_BONUS;
        else
			score[BLACK] += ROOK_SEMI_OPEN_FILE_BONUS;
	}
	if (allPieces[24]/10 -2 == 1)
		score[BLACK] += ROOK_ON_SEVENTH_BONUS;

    if (pawnRank[BLACK][allPieces[25]%10] == 0) {
		if (pawnRank[WHITE][allPieces[25]%10] == 7)
			score[BLACK] += ROOK_OPEN_FILE_BONUS;
        else
			score[BLACK] += ROOK_SEMI_OPEN_FILE_BONUS;
	}
	if (allPieces[25]/10 -2 == 1)
		score[BLACK] += ROOK_ON_SEVENTH_BONUS;
    

    int i = 32;
    /// a promótált bábuk
    while(allPieces[i] != -1 && i<48){
        switch(board[allPieces[i]]){
            case 2:
                score[WHITE] += knight_pcsq[convert_table_white[allPieces[i]]];
                break;
            case 3:
                score[WHITE] += bishop_pcsq[convert_table_white[allPieces[i]]];
                break;
            case 4:
                if (pawnRank[WHITE][allPieces[i]%10] == 7) {
                    if (pawnRank[BLACK][allPieces[i]%10] == 0)
                        score[WHITE] += ROOK_OPEN_FILE_BONUS;
                    else
                        score[WHITE] += ROOK_SEMI_OPEN_FILE_BONUS;
	            }
                if (allPieces[i]/10 -2 == 1)
                    score[WHITE] += ROOK_ON_SEVENTH_BONUS;
                break;
            case -2:
                score[BLACK] += knight_pcsq[convert_table_black[allPieces[i]]];
                break;
            case -3:
                score[BLACK] += bishop_pcsq[convert_table_black[allPieces[i]]];
                break;
            case -4:
                if (pawnRank[BLACK][allPieces[i]%10] == 0) {
                    if (pawnRank[WHITE][allPieces[i]%10] == 7)
                        score[BLACK] += ROOK_OPEN_FILE_BONUS;
                    else
                        score[BLACK] += ROOK_SEMI_OPEN_FILE_BONUS;
	            }
                if (allPieces[i]/10 -2 == 1)
                    score[BLACK] += ROOK_ON_SEVENTH_BONUS;
                break;
        }
        ++i;
    }

    /// a király helyzetének kiértékelése
    /// itt sokat számít, hogy a játék melyen szakaszában vagyunk
    /// a király a végjátékban egy erős ütő bábu, pozíciója meghatározó lehet a pályán
    /// még a játék elejétől közepéig stabil biztonságos pozícióban kell lennie
    /// kell egy határ húzni hogy mikortól számít végjátéknak
    /// a pieceSum[megfelelő_szín] < 1200 egy megfelelő becslése a végjáték kezdetének

    if(pieceSum[WHITE] < 1200)
        score[WHITE] += king_endgame_pcsq[convert_table_white[allPieces[15]]];
    else
        score[WHITE] += whiteKingScore();

    if(pieceSum[WHITE] < 1200)
        score[BLACK] += king_endgame_pcsq[convert_table_black[allPieces[31]]];
    else
        score[BLACK] += blackKingScore();

    std::cout << "Feher pontjai " << score[WHITE] << std::endl << "Fekete pontjai " << score[BLACK] << std::endl;

    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
/// A kiértékelő függvény segédfüggvényei
//////////////////////////////////////////////////////////////////////////////////////////////////

int ChessBoard::whiteKingScore(){
    int value = 0; // a  visszaadnandó érték
    int f = allPieces[15] % 10; // a király oszlopa
    int r = allPieces[15] / 10 - 2; // a király sora

    value += king_pcsq[convert_table_white[allPieces[15]]];

    /// ha a király sáncolva van, akkor megvizsgáljuk a gyalog struktúrát körülötte
    if (f < 3){/// királynő oldal
        value += whiteKingPawnScore(1);
        value += whiteKingPawnScore(2);
        value += whiteKingPawnScore(3) / 2; /// annyira nem végzetes ha itt nem jó a gyalog helyzete
    }else if(f > 4){/// király oldal
        value += whiteKingPawnScore(8);
        value += whiteKingPawnScore(7);
        value += whiteKingPawnScore(6) / 2; /// annyira nem végzetes ha itt nem jó a gyalog helyzete
    }
    /// ha nincs sáncolva a király, akkor viszont pontot vonunk le minden nyitott oszlopért a király közelépben
    else{
        for(int i = (f-1) ; i < (f+2) ; ++i){
            if(pawnRank[WHITE][i] == 7 && pawnRank[BLACK][i] == 0)
                r-= 10;
        }
    }

    /// az ellenfél bábuinak pontszáma alpján skálázzuk a király helyének az értékét
    /// azt az elvet követve, hogyha az ellenfelednek nincs elegendő bábuja, akkor nincs is mivel támadnia
    /// ezáltal a király ilyen helyzetekkor nem lehet már akkora veszélyben
    value *= pieceSum[BLACK];
    value /= 3100;

    return value;
}

int ChessBoard::blackKingScore(){
    int value = 0; // a  visszaadnandó érték
    int f = allPieces[31] % 10; // a király oszlopa
    int r = allPieces[31] / 10 - 2; // a király sora

    value += king_pcsq[convert_table_black[allPieces[31]]];

    /// ha a király sáncolva van, akkor megvizsgáljuk a gyalog struktúrát körülötte
    if (f < 3){/// királynő oldal
        value += blackKingPawnScore(1);
        value += blackKingPawnScore(2);
        value += blackKingPawnScore(3) / 2; /// annyira nem végzetes ha itt nem jó a gyalog helyzete
    }else if(f > 4){/// király oldal
        value += blackKingPawnScore(8);
        value += blackKingPawnScore(7);
        value += blackKingPawnScore(6) / 2; /// annyira nem végzetes ha itt nem jó a gyalog helyzete
    }
    /// ha nincs sáncolva a király, akkor viszont pontot vonunk le minden nyitott oszlopért a király közelépben
    else{
        for(int i = (f-1) ; i < (f+2) ; ++i){
            if(pawnRank[WHITE][i] == 7 && pawnRank[BLACK][i] == 0)
                r-= 10;
        }
    }

    /// az ellenfél bábuinak pontszáma alpján skálázzuk a király helyének az értékét
    /// azt az elvet követve, hogyha az ellenfelednek nincs elegendő bábuja, akkor nincs is mivel támadnia
    /// ezáltal a király ilyen helyzetekkor nem lehet már akkora veszélyben
    value *= pieceSum[WHITE];
    value /= 3100;

    return value;
}

int ChessBoard::whiteKingPawnScore(int f){
    int value = 0;

	if (pawnRank[WHITE][f] == 1);  /* a gyalog nem mozgott */
	else if (pawnRank[WHITE][f] == 2)
		value -= 10;  /* mozgott egy mezőt előre */
	else if (pawnRank[WHITE][f] != 7)
		value -= 20;  /* a gyalog több mezőt is mozgott előre */
	else
		value -= 25;  /* nincs gyalog az oszlopban */

	if (pawnRank[BLACK][f] == 0)
		value -= 15;  /* no enemy pawn */
	else if (pawnRank[BLACK][f] == 5)
		value -= 10;  /* enemy pawn on the 3rd rank */
	else if (pawnRank[BLACK][f] == 4)
		value -= 5;   /* enemy pawn on the 4th rank */

	return value;
}

int ChessBoard::blackKingPawnScore(int f){
    int value = 0;

	if (pawnRank[BLACK][f] == 6);  /* a gyalog nem mozgott */
	else if (pawnRank[BLACK][f] == 5)
		value -= 10;  /* mozgott egy mezőt előre */
	else if (pawnRank[BLACK][f] != 0)
		value -= 20;  /* a gyalog több mezőt is mozgott előre */
	else
		value -= 25;  /* nincs gyalog az oszlopban */

	if (pawnRank[WHITE][f] == 7)
		value -= 15;  /* nincs ellenséges gyalog */
	else if (pawnRank[WHITE][f] == 2)
		value -= 10;  /* enemy pawn on the 3rd rank */
	else if (pawnRank[WHITE][f] == 3)
		value -= 5;   /* enemy pawn on the 4th rank */

	return value;
}

int ChessBoard::whitePawnScore(int index){
    int value = 0; // a  visszaadnandó érték
    int f = index % 10; // a gyalog oszlopa
    int r = index / 10 - 2;

    value += pawn_pcsq[convert_table_white[index]];
    
    /// megvizsgáljuk, hogy van e a jelenlegi mögött gyalog
    if (pawnRank[WHITE][f] < r)
        value -= DOUBLED_PAWN_PENALTY;
    
    /// gyalog izoláltság vizsgálata
    if ((pawnRank[WHITE][f - 1] == 7) && (pawnRank[WHITE][f + 1] == 7))
		value -= ISOLATED_PAWN_PENALTY;
    /// ha nem izolált akkor lehet hátsó gyalog
    else if((pawnRank[WHITE][f - 1] > r) && (pawnRank[WHITE][f + 1] > r))
        value -= BACKWARDS_PAWN_PENALTY;
    
    // bónusz a passed gyalogokért
    if((pawnRank[BLACK][f - 1] <= r) && (pawnRank[BLACK][f] <= r) && (pawnRank[BLACK][f + 1] <= r))
        value += r * PASSED_PAWN_BONUS;
    return value;
}

int ChessBoard::blackPawnScore(int index){
    int value = 0; // a  visszaadnandó érték
    int f = index % 10; // a gyalog oszlopa
    int r = index / 10 - 2;

    value += pawn_pcsq[convert_table_black[index]];
    
    /// megvizsgáljuk, hogy van e a jelenlegi mögött gyalog
    if (pawnRank[BLACK][f] > r)
        value -= DOUBLED_PAWN_PENALTY;
    
    /// gyalog izoláltság vizsgálata
    if ((pawnRank[BLACK][f - 1] == 0) && (pawnRank[BLACK][f + 1] == 0))
		value -= ISOLATED_PAWN_PENALTY;
    /// ha nem izolált akkor lehet hátsó gyalog
    else if((pawnRank[BLACK][f - 1] < r) && (pawnRank[BLACK][f + 1] < r))
        value -= BACKWARDS_PAWN_PENALTY;
    // bónusz a passed gyalogokért
    if((pawnRank[WHITE][f - 1] >= r) && (pawnRank[WHITE][f] >= r) && (pawnRank[WHITE][f + 1] >= r))
        value += (7 - r) * PASSED_PAWN_BONUS;
    return value;
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