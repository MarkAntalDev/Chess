#include "Board.h"

void ChessBoard::UciMain(){
    /// elküldjük a gui nak a kezdeti infókat
    std::cout << "id name Ranger Sakk" << std::endl;
    std::cout << "id author Antal Mark Medard" << std::endl;
    std::cout << "uciok" << std::endl;
    playerSide = true; // fehér a játékos
    std::string str;
    int move;
    engineDepth = 5;
    //std::cout.setf (std::ios::unitbuf);
    while(getline(std::cin, str)){
        generatePseudoLegalMoves();
        if(str == "quit"){
            std::cout << "Bye Bye" << std::endl;
            break;
        }
        if(str == "isready"){
            std::cout << "readyok" << std::endl;
            continue;
        }
        if(str == "ucinewgame"){
            NewGame();
            continue;
        }
        if(str.substr( 0, 3 ) == "go "){
            AlphaBetaSearch();
            continue;
        }
        if(str == "eval"){
            std::cout  << evaluation() << std::endl;
            continue;
        }
        if(str.substr(0, 23) == "position startpos moves"){
            UciMakeMove(str);
            std::string currentNotation = "";
            for (int i = 0 ; i<historyDepth ; ++i){
                if (movesHistory[i].moveString[4] == ' '){
                    for (int j = 0; j < 4; ++j)
                        currentNotation += movesHistory[i].moveString[j];
                }else{
                    for (int j = 0; j < 5; ++j)
                        currentNotation += movesHistory[i].moveString[j];
                }
                if (i != historyDepth-1)
                    currentNotation += " ";
            }
            std::cout << "notation after reading everything" << currentNotation << std::endl; 
            continue;
        }      
        if(str.length() > 3 && str.length() < 6){
            move = parseMove(str);
            if(move==-1 || !MakeMove(moves[move].m.b))
                //std::cout << "illegal move" << std::endl;
            currentDepth = 0;
            std::cout << "the allPieces array: ";
            for(int z=0; z<32 ; ++z){
                std::cout << int(allPieces[z]) << " "; 
            }
            std::cout << std::endl;
            continue;
        }
    }
}

void ChessBoard::UciMakeMove(std::string str){
    // ez esetben a start pozíció a kezdő pozíció
    if(str.length() < 26){
        if(historyDepth > 0){// ha nema  kezdőállapotban vagyunk
            NewGame(); // akkor visszaállítjuk
            std::cout << "Engine new game" << std::endl;
        }
    }else{
        std::string currentNotation = "";
        for (int i = 0 ; i<historyDepth ; ++i){
            if (movesHistory[i].moveString[4] == ' '){
                for (int j = 0; j < 4; ++j)
                    currentNotation += movesHistory[i].moveString[j];
            }else{
                for (int j = 0; j < 5; ++j)
                    currentNotation += movesHistory[i].moveString[j];
            }
            if (i != historyDepth-1)
                currentNotation += " ";
        }
        std::string cmove = str.substr(24);
        std::cout << "AZ ENGINE KULDTE" << std::endl;
        std::cout << "curre not: " << currentNotation << std::endl;
        std::cout << "cmove " << cmove << std::endl;
        if(currentNotation == cmove){
            std::cout << "equal " << cmove << std::endl;
            return;
        }
            
        
        if(currentNotation.length() > cmove.length()){
            //std::cout << "Engine set from start need" << std::endl;
            SetFromStart(cmove);
            return;
        }
        int i=0;
        while(i<currentNotation.length() && currentNotation[i] == cmove[i])
            ++i;
        if(i != currentNotation.length()){
            //std::cout << "Engine set from start need" << std::endl;
            SetFromStart(cmove);
            return;
        }
        //std::cout << "the I ::::: " << i << std::endl;
        //bool good=true;
        int currInd;
        if(i == 0)
            currInd = 0;
        else
            currInd = i+1;
        //std::cout << "the curind " << currInd << std::endl;
        int moveInd = 0;
        std::string tmpString;
        while(true){
            if(currInd + 4 < cmove.length()){
                tmpString= cmove.substr(currInd, 5);
                if(tmpString[4] == ' '){
                    currInd += 5;
                }else{
                    currInd += 6;
                }
                moveInd = parseMove(tmpString);
                if(moveInd==-1 || !MakeMove(moves[moveInd].m.b)){
                    //std::cout << "hiba a beolvasassnal -1 move // illegal move" << std::endl;
                    return;
                }
                currentDepth = 0;
                generatePseudoLegalMoves();
            }else if(currInd + 3 < cmove.length()){
                tmpString= cmove.substr(currInd, 4);
                moveInd = parseMove(tmpString);
                if(moveInd==-1 || !MakeMove(moves[moveInd].m.b)){
                    //std::cout << "hiba a beolvasassnal -1 move // illegal move" << std::endl;
                    return;
                }
                currentDepth = 0;
                return;
            }else{
                return;
            }
        }
    }
}

void ChessBoard::SetFromStart(std::string str){
    NewGame();
    generatePseudoLegalMoves();
    int currInd = 0;
    int i=0;
    int moveInd = 0;
    std::string tmpString;
    while(true){
        if(currInd + 4 < str.length()){
            tmpString= str.substr(currInd, 5);
            if(tmpString[4] == ' '){
                currInd += 5;
            }else{
                currInd += 6;
            }
            moveInd = parseMove(tmpString);
            if(moveInd==-1 || !MakeMove(moves[moveInd].m.b)){
                std::cout << "hiba a beolvasassnal -1 move // illegal move" << std::endl;
                return;
            }
            currentDepth = 0;
            generatePseudoLegalMoves();
        }else if(currInd + 3 < str.length()){
            tmpString= str.substr(currInd, 4);
            moveInd = parseMove(tmpString);
            if(moveInd==-1 || !MakeMove(moves[moveInd].m.b)){
                std::cout << "hiba a beolvasassnal -1 move // illegal move" << std::endl;
                return;
            }
            currentDepth = 0;
            return;
        }else{
            return;
        }
    }
}