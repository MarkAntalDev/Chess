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
    std::cout << std::endl << std::endl;
    std::cout << "Ranger Sakkprogram" << std::endl;
    std::cout << "Keszitette: Antal Mark Medard" << std::endl;
    std::cout << "Temavezeto: Csaji Balazs Csanad" << std::endl;
    std::cout << "Belso temavezeto: dr. Gregorics Tibor" << std::endl;
    std::cout << "Ev / Year : 2021" << std::endl;
    std::cout << std::endl << std::endl;

    /*std::string Line;
    std::cout << "Valasszon szint / Pick a side (feher/white - fekete/black)" << std::endl;
    bool bad = true;
    while(bad){
        getline( std::cin, Line );
        fflush(stdin);
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
    }*/
    playerSide = true; // fehér a játékos
    std::string str;
    int move;
    engineDepth = 5;
    for(;;){
        //system ("CLS");
        generatePseudoLegalMoves();
        /*std::cout << "legal moves:" << std::endl;
        for (int i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i){
            std::cout << moveToString(moves[i].m.b) << " score: ";
        }*/
        drawBoard();
        //drawIntBoard();
        /*if (currentPlayer == !playerSide){
            AlphaBetaSearch();
            continue;
        }*/
        std::cout << "input:"; 
        std::cin >> str;
        if(str == "s"){
            AlphaBetaSearch();
            continue;
        }
        
        if(str == "x")
            break;
        if(str == "m"){
            std::cout << "legal moves : ";
            for (int i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
                std::cout << int(moves[i].m.b.from) << " " << int(moves[i].m.b.to) << " : " << moveToString(moves[i].m.b) << "   ";
            }
            std::cout << std::endl;
            continue;
        }
        if(str == "c"){
            generatePseudoLegalCaptures();
            for (int i = firstMoveOfDepth[currentDepth]; i < firstMoveOfDepth[currentDepth + 1]; ++i) {
                std::cout << int(moves[i].m.b.from) << " " << int(moves[i].m.b.to) << " : " << moveToString(moves[i].m.b) << "   ";
            }
            std::cout << std::endl;
            continue;
        }
        if(str == "eval"){
            std::cout  << evaluation() << std::endl;
            continue;
        }
        if(str == "t"){
            TakeBack();
            continue;
        }
        if(str.substr(0, 23) == "position startpos moves"){
            UciMakeMove(str);
            continue;
        }
        if(str.length() > 3){
            move = parseMove(str);
            if(move==-1 || !MakeMove(moves[move].m.b))
                std::cout << "illegal move" << std::endl;
            //currentDepth = 0;
            continue;
        }
        std::cout << "illegal command" << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
}