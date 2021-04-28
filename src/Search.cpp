#include "Board.h"

//////////// second try with pv
int ChessBoard::max(int depth){
    pvLength[currentDepth] = currentDepth;
    if(depth == 0)
        return evaluation();
    int score = -100000;
    int value;
    generatePseudoLegalMoves();
    for (int i = firstMoveOfDepth[currentDepth] ; i < firstMoveOfDepth[currentDepth + 1] ; ++i){
        //std::cout << moveToString(moves[i].m.b) << " lepest megkiserlese" << std::endl;
        if(!MakeMove(moves[i].m.b))
            continue;
        //drawIntBoard();
        //std::cout << "(Max fv)" << depth << " melyseg " << i << ". gyerekenek kiertekelese" << std::endl;
        value = min (depth - 1);
        TakeBack();
        //std::cout << moveToString(moves[i].m.b) << " taken back" << std::endl;
        //drawIntBoard();
        if(value > score){
            //std::cout << "if belseje" << std::endl;
            score = value;
            pv[currentDepth][currentDepth] = moves[i].m;
            //std::cout << "necces pont 1" << std::endl;
            for ( int j = currentDepth+1 ; j < pvLength[currentDepth + 1]; ++j)
                pv[currentDepth][j] = pv[currentDepth + 1][j];
            pvLength[currentDepth] = pvLength[currentDepth + 1];
        }
        //std::cout << "elso iteracio vege" << std::endl;
    }
    return score;
}

int ChessBoard::min(int depth){
    pvLength[currentDepth] = currentDepth;
    if(depth == 0)
        return -evaluation();
    int score = 100000;
    int value;
    generatePseudoLegalMoves();
    for (int i = firstMoveOfDepth[currentDepth] ; i < firstMoveOfDepth[currentDepth + 1] ; ++i){
        //std::cout << moveToString(moves[i].m.b) << " lepest megkiserlese" << std::endl;
        if(!MakeMove(moves[i].m.b))
            continue;
        //drawIntBoard();
        //std::cout << "(Min fv)" << depth << " melyseg " << i << ". gyerekenek kiertekelese" << std::endl;
        value = max (depth - 1);
        TakeBack();
        //std::cout << moveToString(moves[i].m.b) << " taken back" << std::endl;
        //drawIntBoard();
        if(value < score){
            //std::cout << "if belseje" << std::endl;
            score = value;
            pv[currentDepth][currentDepth] = moves[i].m;
            //std::cout << "necces pont 1" << std::endl;
            for ( int j = currentDepth+1 ; j < pvLength[currentDepth + 1]; ++j)
                pv[currentDepth][j] = pv[currentDepth + 1][j];
            pvLength[currentDepth] = pvLength[currentDepth + 1];
        }
        //std::cout << "elso iteracio vege" << std::endl;
    }
    return score;
}

int ChessBoard::negamax(int depth){
    pvLength[currentDepth] = currentDepth;
	if(depth == 0)
		return evaluation();

	int score = -100000;
	int i , value;
	generatePseudoLegalMoves();
	for (i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
		if (!MakeMove(moves[i].m.b))
			continue;
		value = -negamax(depth - 1);
		TakeBack();
		if(value > score){
			score = value;
			pv[currentDepth][currentDepth] = moves[i].m;
			for ( int j = currentDepth + 1; j < pvLength[currentDepth + 1]; ++j)
				pv[currentDepth][j] = pv[currentDepth + 1][j];
			pvLength[currentDepth] = pvLength[currentDepth + 1];

		}
	}
	return score;
}



int ChessBoard::AlphaBetaMax(int alpha, int beta, int depth )
{
    pvLength[currentDepth] = currentDepth;
    if(depth == 0)
        return evaluation();
    int i , value;
    generatePseudoLegalMoves();
    for (i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
        if (!MakeMove(moves[i].m.b))
            continue;
        value = AlphaBetaMin(alpha, beta, depth - 1 );
        TakeBack();
        if(value >= beta)
            return beta;
        if(value > alpha){
            alpha = value;
            pv[currentDepth][currentDepth] = moves[i].m;
            for ( int j = currentDepth + 1; j < pvLength[currentDepth + 1]; ++j)
                pv[currentDepth][j] = pv[currentDepth + 1][j];
            pvLength[currentDepth] = pvLength[currentDepth + 1];
        }
    }
    return alpha;
}

int ChessBoard::AlphaBetaMin(int alpha, int beta, int depth )
{
    pvLength[currentDepth] = currentDepth;
    if(depth == 0)
        return -evaluation();
    int i , value;
    generatePseudoLegalMoves();
    for (i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
        if (!MakeMove(moves[i].m.b))
            continue;
        value = AlphaBetaMax(alpha, beta, depth - 1 );
        TakeBack();
        if(value <= alpha)
            return alpha;
        if(value < beta){
            beta = value;
            pv[currentDepth][currentDepth] = moves[i].m;
            for ( int j = currentDepth + 1; j < pvLength[currentDepth + 1]; ++j)
                pv[currentDepth][j] = pv[currentDepth + 1][j];
            pvLength[currentDepth] = pvLength[currentDepth + 1];
        }
    }
    return beta;
}

void ChessBoard::search(){
    int score;
	int depth = 2;

	if (currentPlayer)
		score = max(depth);
	else
		score = min(depth);

    std::cout << "Score is (after the pv moves) : " << score << std::endl;
	for (int j = 0; j < pvLength[currentDepth]; ++j)
        std::cout << moveToString(pv[currentDepth][j].b) << " ";
    std::cout << std::endl;
    MakeMove(pv[currentDepth][currentDepth].b);
    //printPv();
    std::cout << evaluation() << std::endl;
}

void ChessBoard::negasearch(){
    int score;
	int depth = 4;

    score = negamax(depth);

	std::cout << "Score is (after the pv moves) : " << score << std::endl;
	for (int j = 0; j < pvLength[currentDepth]; ++j)
        std::cout << moveToString(pv[currentDepth][j].b) << " ";
    std::cout << std::endl;
    MakeMove(pv[currentDepth][currentDepth].b);
    currentDepth = 0;
    historyDepth = 0;
    //printPv();
    std::cout << evaluation() << std::endl;
}

void ChessBoard::printPv(){
    std::cout << std::endl << std::endl;
    for(int i=0 ; i<MAXIMUM_DEPTH ; ++i){
        for(int j=0 ; j<MAXIMUM_DEPTH ; ++j){
            std::cout << moveToString(pv[i][j].b) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl;
}
