#include "Board.h"

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

void ChessBoard::drawBoard(){
    for (int i=11; i>-1 ; --i){
        for (int j=0; j<10 ; ++j){
            if (int(board[i * 10 + j])==7)
                continue;
            std::cout << "  " << ConsolePieces[int(board[i * 10 + j])];
        }
        std::cout << std::endl << std::endl;
    }
}

void ChessBoard::ConsoleMain(){
    std::cout << "Ranger Sakkprogram / Ranger Chess Engine";
    std::cout << "Keszitette / Author: Antal Mark Medard" << std::endl;
    std::cout << "Ev / Year : 2021" << std::endl;
    std::cout << std::endl << std::endl;

    std::string Line;
    std::cout << "Valasszon szint / Pick a side (feher/white - fekete/black)" << std::endl;
    bool bad = true;
    while(bad){
        getline( std::cin, Line );
        if(Line.substr( 0, 5 ) == "feher"){
            bad = false;
            playerSide = true;
        }
        else if(Line.substr( 0, 5 ) == "white"){
            bad = false;
            playerSide = true;
        }
        else if(Line.substr( 0, 6 ) == "fekete"){
            bad = false;
            playerSide = true;
        }
        else if(Line.substr( 0, 5 ) == "black"){
            bad = false;
            playerSide = true;
        }
        else{
            std::cout << "Helytelen oldal kerem valsszon ujra" << std::endl;
        }
    }
    char c;
    for(;;){
        drawBoard();
        system ("CLS");
        std::cout << "input:";
        std::cin >> c;
        if(c == 's')
            negasearch();
        if(c == 'x')
            break;
    }
}