#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include<tuple>

struct legalMove{
  unsigned char from;
  unsigned char to;
  char value;
};

class ChessBoard{
public:
    ChessBoard();
    //void boardToOriginal(); //// visszaállítja a board ot az eredeti állapotába
    void drawIntBoard();//drawing the table to the console with numbers
    void generatePseudoLegalMoves();
private:
    /***********************************************************************************************
    ; egy 120 elemes 1 dimenziós tömb a gyorsabb elérés és könnyebb lépésgenerálás érdekében
    ; 10*12 es táblát reprezentál amivel a pályáról való kilépés alacsony költséggel ellenőrizhető
    ; 1 byte <-> char változó használata a kevesebb tárhely érdekében
    ; 
    ; Bábuk reprezentációja:
    ;
    ; 1 - fehér gyalog    | white pawn
    ; 2 - fehér ló        | white knight
    ; 3 - fehér futó      | white bishop
    ; 4 - fehér bástya    | white rook
    ; 5 - fehér királyanő | white queen
    ; 6 - fehér király    | white king
    ;
    ; 1 - fekete gyalog    | black pawn
    ; 2 - fekete ló        | black knight
    ; 3 - fekete futó      | black bishop
    ; 4 - fekete bástya    | black rook
    ; 5 - fekete királyanő | black queen
    ; 6 - fekete király    | black king
    ;
    ************************************************************************************************/
    char board[120] = {
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
    };

    //current positions(indices) of the pieces
    char whiteLeftRook = 21;
    char whiteLeftKnight = 22;
    char whiteLeftBishop = 23;
    char whiteQueen = 24;
    //char whiteKing = 25; 
    char whiteRightBishop = 26;
    char whiteRightKnight = 27;
    char whiteRightRook = 28;
    

    //vector for current possible moves
    std::vector<legalMove> legalMoves;

    /*************************************************************************************************
    ; 
    ; Moving constants (Offsets)
    ; 
    **************************************************************************************************/

    const char knightOffsets[8] = {-21, -19, -12, -8,  8,  12,  19,  21};
    const char rookOffsets[4] = {-1, 1, -10, 10};
    const char bishopOffsets[4] = {9, 11, -9, -11};

    /*************************************************************************************************
    ; 
    ; WHITE move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; 
    **************************************************************************************************/

    void whiteKnightMoveGeneration(); // white knight move generation function
    void whiteRookMoveGeneration();   // white rook move generation function
    void whiteBishopMoveGeneration(); // white bishop move generation function
    void whiteQueenMoveGeneration(); // white queen move generation function (rook + bishop move geenration)

    /*************************************************************************************************
    ; 
    ; BLACK move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; 
    **************************************************************************************************/
};

#endif // _BOARD_H_