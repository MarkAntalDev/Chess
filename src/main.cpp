#include <iostream>
#include "Board.h"


int main(){
    ChessBoard* chessboard = new ChessBoard();
    char c;
    std::cout << "chose az "
    
    /*const wchar_t *white = L"♔♕♖♗♘♙\r\n";
    const wchar_t *black = L"♚♛♜♝♞♟\r\n";

    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), white, wcslen(white), 0, 0);
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), black, wcslen(black), 0, 0);
    */
    
    delete chessboard;
    


    return 0;
}