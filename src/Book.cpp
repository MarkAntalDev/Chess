#include "Board.h"

void ChessBoard::OpenBook(){
    book.open("book.txt");
    if (!book) {
        std::cout << "Opening book missing" << std::endl;
        hasBook = false;
    }
    else{
        std::cout << "Opening book opened" << std::endl;
        hasBook = true;
    }
}

void ChessBoard::CloseBook(){
    if(book)
        book.close();
    hasBook = false;
}

int ChessBoard::FindBookMove(){
    // jelenlegi hely ahol tartunk
    std::string currentNotation = "";
    for (int i = 0 ; i<historyDepth ; ++i){
        if (movesHistory[i].moveString[4] == ' '){
            for (int j = 0; j < 4; ++j)
                currentNotation += movesHistory[i].moveString[j];
        }else{
            for (int j = 0; j < 4; ++j)
                currentNotation += movesHistory[i].moveString[j];
        }
        if (i != historyDepth-1)
            currentNotation += " ";
    }

    int possibleMoves[60];
    int countOfPossibleMoves[60];
    int moveCounter = 0;
    book.clear();
    book.seekg(0);
    std::string line;
    int finalMove = -1;
    while (getline(book,line)){

        if (line.length() <= currentNotation.length())
            continue;
        int i=0;
        while (i<currentNotation.length() && currentNotation[i] == line[i])
            ++i;
        if (i == currentNotation.length()){
            int m;
            if(historyDepth == 0)
                m = parseMove(line.substr(i, 4) + " ");
            else
                m = parseMove(line.substr(i+1, 4) + " ");
            //std::cout << "possible move " << line.substr(i+1, 5) << " and m is " << m << std::endl; 
            if (moveCounter < 60){
                int z = 0;
                while (z < moveCounter && possibleMoves[z] != m){
                    ++z;
                }
                if (z == moveCounter){
                    possibleMoves[moveCounter] = m;
                    countOfPossibleMoves[moveCounter] = 1;
                    ++moveCounter;
                }else{
                    countOfPossibleMoves[z] ++;
                }
            }else {
                break;
            }
        }
    }

    if (moveCounter == 0){ // nem találtunk klépést a könyvből
        followBook = false; // ne is keressünk több lépést már
        return -1;
    }

    // véletlenszerűen választunk egy indexet, és visszaadjuk
    finalMove = possibleMoves[rand() % moveCounter];
    return finalMove;
}