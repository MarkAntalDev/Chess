#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <string>

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

    //change index to position map
    std::map<int, std::string> indexPosMap = {
      { 21, "a1" },
      { 22, "b1" },
      { 23, "c1" },
      { 24, "d1" },
      { 25, "e1" },
      { 26, "f1" },
      { 27, "g1" },
      { 28, "h1" },

      { 31, "a2" },
      { 32, "b2" },
      { 33, "c2" },
      { 34, "d2" },
      { 35, "e2" },
      { 36, "f2" },
      { 37, "g2" },
      { 38, "h2" },

      { 41, "a3" },
      { 42, "b3" },
      { 43, "c3" },
      { 44, "d3" },
      { 45, "e3" },
      { 46, "f3" },
      { 47, "g3" },
      { 48, "h3" },

      { 51, "a4" },
      { 52, "b4" },
      { 53, "c4" },
      { 54, "d4" },
      { 55, "e4" },
      { 56, "f4" },
      { 57, "g4" },
      { 58, "h4" },

      { 61, "a5" },
      { 62, "b5" },
      { 63, "c5" },
      { 64, "d5" },
      { 65, "e5" },
      { 66, "f5" },
      { 67, "g5" },
      { 68, "h5" },
    };

    //current positions(indices) of the pieces
    char whiteLeftRook = 21;
    char whiteLeftKnight = 22;
    char whiteLeftBishop = 23;
    char whiteQueen = 24;
    char whiteKing = 25; 
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
    ; White King in check
    ; 
    **************************************************************************************************/

    bool whiteKingInCheck(); /// checking if the white king is in check

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
    void whiteKingMoveGeneration(); // white king move generation function (queen without while loop)

    /*************************************************************************************************
    ; 
    ; BLACK move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; 
    **************************************************************************************************/
};

#endif // _BOARD_H_