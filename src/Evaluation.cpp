#include "Board.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
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
        if (allPieces[i] != -1 && whitePromoted[i] == 0){
            pawnSum[WHITE] += numberToValueWhite[1];
            currentFile = (allPieces[i] % 10); /// nem kell kivonni egyet, ugyanis a 2. elemtől kezdődnek a gyalogok a tömbben
            currentRank = (allPieces[i] / 10) - 2;
            if (pawnRank[WHITE][currentFile] > currentRank)
                pawnRank[WHITE][currentFile] = currentRank;    
        }
        if (allPieces[i+16] != -1 && blackPromoted[i] == 0){
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

    /// átalakított bábuk
    for(int i=0 ; i<8 ; ++i){
        if(whitePromoted[i] != 0 && allPieces[i] > -1)
            pieceSum[WHITE] = numberToValueWhite[whitePromoted[i]];
        if(blackPromoted[i] != 0 && allPieces[i+16] > -1)
            pieceSum[BLACK] = numberToValueBlack[blackPromoted[i]];
    }

    ///////////////
    ///////////////
    /// 2. körben hozzáadjuk a bábukhoz az extra értékeiket a pozíciójuk alapján
    /// Eval_tables.h tartalmazza az összes konstans táblát
    ///////////////
    ///////////////
    score[WHITE] = pieceSum[WHITE] + pawnSum[WHITE];
    score[BLACK] = pieceSum[BLACK] + pawnSum[BLACK];


    for (int i = 0; i<8 ; ++i){
        if(allPieces[i] > -1 && whitePromoted[i] == 0)
            score[WHITE] += whitePawnScore(allPieces[i]);
        if(allPieces[i+16] > -1 && blackPromoted[i] == 0)
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
    /// a pawnRank tömb alaőján vizsgáljuk a nyílt egyeneseket a bástya számára
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


    // átalakított bábuk kiértékelése
    for(int i = 0; i<8 ; ++i){
        if(allPieces[i] > -1){
            switch(whitePromoted[i]){
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
                default:
                    break;
            }
        }
        if(allPieces[i+16] > -1){
            switch(blackPromoted[i]){
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
                default:
                    break;
            }
        }
    }

    /// a király helyzetének kiértékelése
    /// itt sokat számít, hogy a játék melyen szakaszában vagyunk
    /// a király a végjátékban egy erős ütő bábu, pozíciója meghatározó lehet a pályán
    /// még a játék elejétől közepéig stabil biztonságos pozícióban kell lennie
    /// kell egy határ húzni hogy mikortól számít végjátéknak
    /// a pieceSum[megfelelő_szín] < 1200 egy megfelelő becslése a végjáték kezdetének

    if(pieceSum[WHITE] < 1200){
        score[WHITE] += king_endgame_pcsq[convert_table_white[allPieces[15]]];
    } 
    else{
        if(!(castleFlag & 3)){ /// fehér nem sáncolhat
            if(castled & 1) /// viszont sáncolt a fehér
                score[WHITE] += 100;
            else /// nem sáncolhat már
                score[WHITE] -= 100;
        }
        score[WHITE] += whiteKingScore();
    }
        

    if(pieceSum[WHITE] < 1200){
        score[BLACK] += king_endgame_pcsq[convert_table_black[allPieces[31]]];
    }
    else{
        if(!(castleFlag & 12)){ // ha a fekete nem sáncolhat
            if(castled & 2) // de sáncolt 
                score[BLACK] += 100;
            else // nem is sáncolhat
                score[BLACK] -= 100;
        }
        score[BLACK] += blackKingScore();
    }

    /// a jelenlegi játékostól függően adunk visssza
    if(currentPlayer)
        return score[WHITE] - score[BLACK];
    return score[BLACK] - score[WHITE];
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

