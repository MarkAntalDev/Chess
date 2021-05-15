#include "Board.h"

int ChessBoard::MinMaxAlphaBeta(int depth, int alpha, int beta){
    pvLength[currentDepth] = currentDepth;

    if(currentDepth > maxDepthReached){
        maxDepthReached = currentDepth;
        std::cout << "info depth " << currentDepth << std::endl;
    }
        
    
	if(depth == 0)
        return evaluation();
		//return AlphaBetaCaps(alpha, beta);
    nodes++;
    if(currentDepth > 0 && HashReps() > 0){
        //std::cout << "rep found: " << hash << std::endl;
        return 0;
    }
    if(nodes % 10000 == 0)
        std::cout << "info nodes " << nodes << std::endl;;

    /*for (int j = 0; j < historyDepth; ++j){
        for(int i=0; i<4 ; ++i)
            std::cout << movesHistory[j].moveString[i];
        std::cout << " ";
    }
    std::cout << std::endl;*/
        
    
	int value;
    bool check;
    bool hasLegalMoves = false;
    check = inCheck(currentPlayer);
	generatePseudoLegalMoves();
	for (int i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
        sortMoves(i);
		if (!MakeMove(moves[i].m.b))
			continue;
        hasLegalMoves = true; // ha ide eljutottunk biztos volt egy legális gyerek / lépés a pozícióban
		value = -MinMaxAlphaBeta(depth - 1, -beta, -alpha);
		TakeBack();
		if(value > alpha){
            if(currentPlayer)
                histoH[0][int(allPieces[moves[i].m.b.from])][int(moves[i].m.b.to)] += depth;
            else
                histoH[1][int(allPieces[moves[i].m.b.from])][int(moves[i].m.b.to)] += depth;

            if(value >= beta)
                return beta;
			alpha = value;

			pv[currentDepth][currentDepth] = moves[i].m;
            pvStr[currentDepth][currentDepth] = moveToString(moves[i].m.b);
			for (int j = currentDepth + 1; j < pvLength[currentDepth + 1]; ++j){
                pv[currentDepth][j] = pv[currentDepth + 1][j];
                pvStr[currentDepth][j] = pvStr[currentDepth + 1][j];
            }
				
			pvLength[currentDepth] = pvLength[currentDepth + 1];

		}
	}

    if(!hasLegalMoves){
        // és sakkban vagyunk == sakk matt
        if(check){
            //std::cout << "sakk matt node";
            return -1000000 + currentDepth;
        }
        // nincs legális lépés és nem vagyunk sakkban == stalemate == patt (1/2 - 1/2)
        else {
            //std::cout << "patt matt node";
            return 0;
        }
            
    }
        
    if(fifthyMove >= 100)
        return 0;

	return alpha;
}

int ChessBoard::AlphaBetaCaps(int alpha, int beta){
    pvLength[currentDepth] = currentDepth;

    if(currentDepth > maxDepthReached){
        maxDepthReached = currentDepth;
        std::cout << "info depth " << currentDepth << std::endl;
    }
        
    if(currentDepth >= MAXIMUM_DEPTH-1)
	    return evaluation();
    if(historyDepth >= SIZE_OF_HISTORY-1)
        return evaluation();

    nodes++;
    if(nodes % 10000 == 0)
        std::cout << "info nodes " << nodes << std::endl;;

    
	int value = evaluation();
    if(value >= beta)
        return beta;
    if(value > alpha)
        alpha = value;
    
    bool check;
    bool hasLegalMoves = false;
    check = inCheck(currentPlayer);
	generatePseudoLegalCaptures();
	for (int i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
        sortMoves(i);
		if (!MakeMove(moves[i].m.b))
			continue;
        hasLegalMoves = true; // ha ide eljutottunk biztos volt egy legális gyerek / lépés a pozícióban
		value = -AlphaBetaCaps(-beta, -alpha);
		TakeBack();
		if(value > alpha){
            if(value >= beta)
                return beta;
			alpha = value;

			pv[currentDepth][currentDepth] = moves[i].m;
            pvStr[currentDepth][currentDepth] = moveToString(moves[i].m.b);
			for (int j = currentDepth + 1; j < pvLength[currentDepth + 1]; ++j){
                pv[currentDepth][j] = pv[currentDepth + 1][j];
                pvStr[currentDepth][j] = pvStr[currentDepth + 1][j];
            }
				
			pvLength[currentDepth] = pvLength[currentDepth + 1];

		}
	}

    if(!hasLegalMoves){
        // és sakkban vagyunk == sakk matt
        if(check){
            //std::cout << "sakk matt node";
            return -1000000 + currentDepth;
        }
        // nincs legális lépés és nem vagyunk sakkban == stalemate == patt (1/2 - 1/2)
        else {
            //std::cout << "patt matt node";
            return 0;
        }
            
    }

	return alpha;
}

void ChessBoard::AlphaBetaSearch(){
    if(followBook){
        int bm = FindBookMove();
        if(bm != -1){
            std::cout << "info book move" << std::endl;
            std::cout << "bestmove "<< moveToString(moves[bm].m.b) << std::endl; 
            MakeMove(moves[bm].m.b);
            currentDepth = 0;
            //std::cout << -evaluation() << std::endl;
            //std::cout << std::endl;
            return;
        }
        followBook = false;
    }
    //std::cout << "searching" << std::endl;
    memset(pv, 0, sizeof(pv));
    memset(histoH, 0 , sizeof(histoH));
    memset(pvStr, 0 , sizeof(pvStr));
    int score;
    int depth = engineDepth;
    currentDepth = 0;
    nodes = 0;
    maxDepthReached = 0;
    score = MinMaxAlphaBeta(depth, -10000, 10000);
    //std::cout << nodes << " nodes were searched" << std::endl;
    std::cout << "info pv ";
	for (int j = 0; j < pvLength[currentDepth]; ++j)
        std::cout << pvStr[0][j] << " ";
    std::cout << std::endl;
    
    std::cout << "the allPieces array: ";
    for(int z=0; z<32 ; ++z){
        std::cout << allPieces[z] << " "; 
    }
    MakeMove(pv[currentDepth][currentDepth].b);
    std::cout << "the allPieces array after making the move: ";
    for(int z=0; z<32 ; ++z){
        std::cout << allPieces[z] << " "; 
    }
    currentDepth = 0;
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
    std::cout << "bestmove "<< pvStr[0][0] << std::endl; 
}

void ChessBoard::sortMoves(int from){
    int topScore = -1;
    int topScoreIndex = from;
    genMove move;

    for (int i = from ; i < firstMoveOfDepth[currentDepth + 1] ; ++i){
        if(moves[i].score > topScore) {
            topScore = moves[i].score;
            topScoreIndex = i;
        }
    }
    move = moves[from];
    moves[from] = moves[topScoreIndex];
    moves[topScoreIndex] = move;
}

// debug miatt
void ChessBoard::printPv(){
    std::cout << std::endl << std::endl;
    for(int i=0 ; i<MAXIMUM_DEPTH ; ++i){
        for(int j=0 ; j<MAXIMUM_DEPTH ; ++j){
            std::cout << pvStr[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}

int ChessBoard::HashReps(){
    int reps = 0;
    for (int i = historyDepth-fifthyMove ; i < historyDepth ; ++i){
        if (movesHistory[i].hash == hash){
            ++reps;
        }     
    }
    return reps;
}
