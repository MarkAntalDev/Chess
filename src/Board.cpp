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
    currentDepth = 0;
    historyDepth = 0;
    firstMoveOfDepth[currentDepth] = 0;
    fifthyMove = 0;
    castleFlag = 15;
}

ChessBoard::ChessBoard(){
    initBoard();
    std::srand(std::time(nullptr));
}


/////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Kereső funkciók implementációja
///
/////////////////////////////////////////////////////////////////////////////////////////////////

/*int ChessBoard::max(moveBytes* bestMove){
    int score = -100000;
    int value;
    generatePseudoLegalMoves();
    for(int i=firstMoveOfDepth[currentDepth] ; i< firstMoveOfDepth[currentDepth+1] ; ++i){
        if(!MakeMove(moves[i].m.b))
            continue;
        value = -evaluation();
        TakeBack();
        if(value > score){
            score = value;

            *bestMove = moves[i].m.b;
        }
    }
    return score;
}

int ChessBoard::min(moveBytes* bestMove){
    int score = 100000;
    int value;
    generatePseudoLegalMoves();
    for(int i=firstMoveOfDepth[currentDepth] ; i< firstMoveOfDepth[currentDepth+1] ; ++i){
        if(!MakeMove(moves[i].m.b))
            continue;
            //std::cout << "the move made from " << int(moves[i].m.b.from) << " to " << int(moves[i].m.b.to) << "and the evaluation:" << std::endl;
        value = evaluation();
        TakeBack();
        //std::cout << "the move has been taken back" << std::endl;
        if(value < score){
            score = value;
            *bestMove = moves[i].m.b;
        }
    }
    return score;
}*/

/*void ChessBoard::search(){
    moveBytes bestMove ;
    int score;
    if (currentPlayer)
        score = max(&bestMove);
    else
        score = min(&bestMove);
    std::cout << "Score is : " << score << std::endl;
    std::cout << "The best move is : " << moveToString(bestMove) << std::endl;
    MakeMove(bestMove);
}*/

/////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Pseudo legális lépésekhez adás függvényei
///
/////////////////////////////////////////////////////////////////////////////////////////////////

/// megvizsgálja hogy az x integernek az n-edik bitje 1(true) vagy 0(false)
bool ChessBoard::isNthBitSet(int x, int n){
    if(x & (1 << (n-1)))
        return true;
    else
        return false;
}

bool ChessBoard::isMoveType(int bits, int type){
    return (bits & type);
}

/// A lépésgeneráláshoz használt funkció mely hozzáadja a moves tömbhöz a megfelelő lépést
void ChessBoard::addToMoves(int from, int to, int moveT){
    if(isMoveType(moveT, PAWNMOVE)){
        if(currentPlayer == WHITESIDE){
            if(to > 90){ 
                addPromToMoves(from, to, moveT); /// 4 lépést generálunk lsd.: addPromToMoves
                return; /// ne folytatódjon a függvény
            }
        }else{
            if(to < 29){
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

    if(board[to] != 0){ /// azaz ütés történik
        /// a pontszám egy MVV/LVA alapján működik
        /// Most Valuable Victim / Least Valuable Attacker
        /// azaz legértékesebb áldozat/ legértéktelenebb támadó
        /// gyakorlatban a támadó értékét kivonjuk a végleges értékből
        /// a leütött bábu értékét pedig hozzáadjuk
        /// továbbá egy konstans nag számmal toldjuk az integerünket, mivel ütés
        /// és később a move ordering előbb vegye az ütéseket mint a nem ütéseket
        cmove->score = 10000 + (pieceToMoveValue[board[to]] - pieceToMoveValue[board[allPieces[from]]]);
    }else{ // ellenkező esetben sima lépés
        /// to be implemented
        cmove->score = 0;
    }
}

void ChessBoard::addPromToMoves(int from, int to, int moveT){
    genMove *cmove;
    /// 4 bábuvá váltohat egy gyalog
    for(int i=0 ; i<4 ; ++i){
        cmove = &moves[firstMoveOfDepth[currentDepth+1]++];
        cmove->m.b.from = (char)from;
        cmove->m.b.to = (char)to;
        cmove->m.b.promote = pow(2,i);
        cmove->m.b.moveType = (char)(moveT | 32); /// így bármilyen lépés hozzátesszük, hogy promótáló lépés
        /// hozzáadjuk a bábu értékét amivé változni fog, de kivonjuk a gyalog értékét ugyanis az elveszik
        cmove->score = 10000 + pieceToMoveValue[i+2] - pieceToMoveValue[1];
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

    //std::cout << "Feher pontjai " << score[WHITE] << std::endl << "Fekete pontjai " << score[BLACK] << std::endl;

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
                if (board[currentWhiteKnight + knightOffsets[i]] < 0){ /// ha ütés
                    addToMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], 1);
                }else{ /// ha nem ütés
                    addToMoves(pieceIndex, currentWhiteKnight + knightOffsets[i], 0);
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

void ChessBoard::whiteBishopMoveGeneration(char pieceIndex){
    char currentWhiteBishop = allPieces[pieceIndex];
    char j;
    if(currentWhiteBishop != -1){
        for (char i=0 ; i<4 ; ++i){
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

void ChessBoard::whiteQueenMoveGeneration(char pieceIndex){
    char whiteQueen = allPieces[pieceIndex];
    char j;
    if(whiteQueen != -1){
        for (char i=0 ; i<4 ; ++i){
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
                addToMoves(pieceIndex, whiteQueen + j * rookOffsets[i], 0);
                // the last move is a capture || else doesn't matter, either white piece or off the table
            }
        }
    } 
}

void ChessBoard::whiteKingMoveGeneration(){
    char whiteKing = allPieces[15];
    for (char i=0 ; i<4 ; ++i){
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
        if(allPieces[index] != -1){
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

void ChessBoard::blackKnightMoveGeneration(char pieceIndex){
    char currentBlackKnight = allPieces[pieceIndex];
    if (currentBlackKnight != -1){  /// both white knights are in the game
        for (char i=0 ; i<8 ; ++i){
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
    /// allPieces[31] == black king
    char blackKing = allPieces[31];
    for (char i=0 ; i<4 ; ++i){
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

void ChessBoard::blackRookMoveGeneration(char pieceIndex){
    char currentBlackRook = allPieces[pieceIndex];
    char j;
    if (currentBlackRook != -1){
        for (int i=0 ; i<4 ; ++i){
            j = 1;
            while (board[currentBlackRook + j * rookOffsets[i]] == 0){
                addToMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], 0);
                ++j;
            }
            if (board[currentBlackRook + j * rookOffsets[i]] > 0 && board[currentBlackRook + j * rookOffsets[i]] < 7){
                addToMoves(pieceIndex, currentBlackRook + j * rookOffsets[i], 0);
                /// the last move is a capture || else doesn't matter, either black piece or off the table
            }
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

void ChessBoard::blackQueenMoveGeneration(char pieceIndex){
    char blackQueen = allPieces[pieceIndex];
    char j;
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
        if(allPieces[index] != -1){
            int currentPawn = allPieces[index];
            if(board[currentPawn - 9] > 0 && board[currentPawn - 9] < 7)
                addToMoves(index, currentPawn - 9, 17);
            if(board[currentPawn - 11] < 0 && board[currentPawn - 11] < 7)
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
                if(board[24] != 0 || board[23] != 0 || attack(24, !currentPlayer) || attack(23, !currentPlayer))
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
                if(board[94] != 0 || board[93] != 0 || attack(94, !currentPlayer) || attack(93, !currentPlayer))
                    return false;
                fromRook = 24;
                toRook = 94;
                break;
            default:
                break; // ide nem juthatunk el
        }
        // sáncolás esetémn előbb leléptetjük a bástyát, majd a királyt később a sima karaktermozgatás fogja intézni
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
    if(moveData.moveType & 4){
        if(currentPlayer)
            movesHistory[historyDepth].toAllPieces = findPiece(moveData.to - 10);
        else
            movesHistory[historyDepth].toAllPieces = findPiece(moveData.to + 10);
    }else{
        movesHistory[historyDepth].toAllPieces = findPiece(moveData.to);
    }
    
    movesHistory[historyDepth].hash = 0; /// jelenleg még nincs hash generálva
    ++currentDepth;
    ++historyDepth;


    ///////
    /// A sáncolás Flagjének frissítése
    ///////
    if(moveData.moveType & 2){ // ha sáncolás a lépés
        if(moveData.to < 29) // ha fehér oldali sánc történt 
            castleFlag &= 12; // "és"eljük 12 vel (1100) így az 1 es és 2 es helyiérték bitje biztos 0 lesz
        else // ha fehér oldali sánc történt akkor éseljük 12 vel (1100) így az 1 es és 2 es helyiérték bitje biztos 0 lesz
            castleFlag &= 3; //a fekete oldalon ugyanígy járunk el
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
        if(moveData.moveType & 1){
            allPieces[findPiece(moveData.to)] = -1;
            board[moveData.to] = moveData.promote;
            int i = 32;
            while(i<48 && allPieces[i] != -1)++i;
            allPieces[i] = moveData.to;
            board[allPieces[moveData.from]] = 0;
            allPieces[moveData.from] = -1; // a gyalog eltűnik
        }
    }else{
        if(moveData.moveType & 4){ /// en passant esetén
            if(currentPlayer){ // ha fehér üt
                board[allPieces[findPiece(moveData.to - 10)]] = 0;
                allPieces[findPiece(moveData.to - 10)] = -1;
                board[moveData.to] = board[allPieces[moveData.from]];
                board[allPieces[moveData.from]] = 0;
                allPieces[moveData.from] = moveData.to;
            }else{ // ha fekete üt
            board[allPieces[findPiece(moveData.to + 10)]] = 0;
                allPieces[findPiece(moveData.to + 10)] = -1;
                board[moveData.to] = board[allPieces[moveData.from]];
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
    return true;
}

// egy lépés visszacsinálása ha illegális
// 
void ChessBoard::TakeBack(){
    moveBytes m;

    currentPlayer = !currentPlayer;
    --currentDepth;
    --historyDepth;
    
    m = movesHistory[historyDepth].m.b;
    castleFlag = movesHistory[historyDepth].castleFlag;
    ep = movesHistory[historyDepth].ep;
    fifthyMove = movesHistory[historyDepth].fifthy;
    // hash nincs még implementálva

    allPieces[m.from] = movesHistory[historyDepth].boardFrom;
    if (m.moveType & 32){ // gyalog átalakítás
        board[allPieces[m.from]] = currentPlayer ? 1 : -1;
        int i=32;
        while(i < 48 && allPieces[i] != -1)++i;
        allPieces[i] = -1;
    }else{
        board[allPieces[m.from]] = board[m.to];
    }
    if(! (m.moveType & 4) ){
        board[m.to] = movesHistory[historyDepth].capture;
    }
    if (m.moveType & 1){ // ha ütés
        allPieces[movesHistory[historyDepth].toAllPieces] = m.to;
    }
    if (m.moveType & 2){ // sánc
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
    }
    if (m.moveType & 4){ // en passant
        if(currentPlayer){
            board[m.to] = 0;
            allPieces[movesHistory[historyDepth].toAllPieces] = m.to - 10;
            board[allPieces[movesHistory[historyDepth].toAllPieces]] = -1;
        }else{
            board[m.to] = 0;
            allPieces[movesHistory[historyDepth].toAllPieces] = m.to + 10;
            board[allPieces[movesHistory[historyDepth].toAllPieces]] = 1;
        }
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
    std::cout << "current player: ";
    if(currentPlayer)
        std::cout << "white" << std::endl;
    else
        std::cout << "black" << std::endl;
    generatePseudoLegalMoves();
    std::cout << "current depth: " << currentDepth << std::endl;
    std::cout << "First index of the move of the current depth: " << firstMoveOfDepth[currentDepth] << std::endl; 
    for (int i = firstMoveOfDepth[currentDepth] ; i<firstMoveOfDepth[currentDepth+1] ; ++i){
        std::cout << "from " << int(moves[i].m.b.from) << " to " << int(moves[i].m.b.to) << " type " << int(moves[i].m.b.moveType) << std::endl;
    }
    int randomIndex = std::rand()%(firstMoveOfDepth[currentDepth+1] - firstMoveOfDepth[currentDepth]) + firstMoveOfDepth[currentDepth];
    std::cout << "the chosen index is: " << randomIndex << " and the move is: from " << int(moves[randomIndex].m.b.from) << " to " << int(moves[randomIndex].m.b.to) << " type " << int(moves[randomIndex].m.b.moveType) << std::endl;
    MakeMove(moves[randomIndex].m.b);
}

std::string ChessBoard::moveToString(moveBytes move){
    std::string moveString;
    if(move.to > 98 || move.to < 21)
        return "0000";
    moveString = indexToPos[allPieces[move.from]] + indexToPos[move.to];
    return moveString;
}