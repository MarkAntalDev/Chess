#include <iostream>
#include "Board.h"

int main(){
    std::ios::sync_with_stdio(false);
    ChessBoard* chessboard = new ChessBoard();
    chessboard->InitZobrist();
    chessboard->OpenBook();
    std::string s;
    std::cout << "Valassz egy interface-t (console, uci): ";
    std::cin >> s;
    if (s.substr(0,7) == "console"){
        chessboard->ConsoleMain();
    }
    if (s.substr(0,3) == "uci"){
        chessboard->UciMain();
    }
    chessboard->CloseBook();
    delete chessboard;
    


    return 0;
}