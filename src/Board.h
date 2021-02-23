#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <ctime>
#include <ratio>
#include <chrono>


struct legalMove{
  unsigned char from;
  unsigned char to;
  unsigned char takenPiece; /// if there was any
  char value;
};

/* side note for takenPiece 
; 0 if nothing
; 1 - 8 white pawns ( -1 equals the index in the array)
; 9 white left rook
; 10 white right rook
; 11 white left knight
; 12 white right knight
; 13 white left bishop
; 14 white right bishop
; 15 white queen
; 16 white king
; 
; [17 - 32] same with black 
;
*/
class ChessBoard{
public:
    ChessBoard();
    //void boardToOriginal(); //// visszaállítja a board ot az eredeti állapotába
    void drawIntBoard();//drawing the table to the console with numbers
    void generatePseudoLegalMoves();
    void testingFunction();
private:
    //vector for current possible moves
    std::vector<legalMove> legalMoves;
    
    bool enPassant; //is en passant an option

    bool currentPlayer; // true-white || false-black

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

    //**************white pieces**************************

    char whiteLeftRook =    21;
    char whiteLeftKnight =  22;
    char whiteLeftBishop =  23;
    char whiteQueen =       24;
    char whiteKing =        25; 
    char whiteRightBishop = 26;
    char whiteRightKnight = 27;
    char whiteRightRook =   28;

    char whitePawns[8] = {31, 32, 33, 34, 35, 66, 37, 38};
    
    //**************black pieces**************************

    char blackLeftRook =    91;
    char blackLeftKnight =  92;
    char blackLeftBishop =  93;
    char blackQueen =       94;
    char blackKing =        95; 
    char blackRightBishop = 96;
    char blackRightKnight = 97;
    char blackRightRook =   98;

    char blackPawns[8] = {81, 82, 83, 84, 85, 86, 87, 88};



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

    
    void whiteRookMoveGeneration();   // white rook move generation function
    void whiteBishopMoveGeneration(); // white bishop move generation function
    void whiteQueenMoveGeneration();  // white queen move generation function (rook + bishop move geenration)
    void whiteKingMoveGeneration();   // white king move generation function (queen without while loop)
    void whiteKnightMoveGeneration(); // white knight move generation function

    //pawn moves: separate functions because of the complications (double push, en passant,different capture)
    void whitePawnsCapture(int index);
    void whitePawnsEnPassant(int index);
    void whitePawnMoveGeneration();  // all the white pawn functions together

    /*************************************************************************************************
    ; 
    ; BLACK move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; Same naming as whites
    ; 
    **************************************************************************************************/

    void blackRookMoveGeneration();
    void blackBishopMoveGeneration();
    void blackKnightMoveGeneration(); 
    void blackKingMoveGeneration();
    void blackQueenMoveGeneration();

    void blackPawnsCapture(int index);
    void blackPawnsEnPassant(int index);
    void blackPawnMoveGeneration();

   /*************************************************************************************************
    ; 
    ; Additional functions
    ; 
    **************************************************************************************************/

    bool whiteKingInCheck(); /// checking if the white king is in check
    void putInLegalMoves(unsigned char _from, unsigned  char _to, unsigned char _takenPiece, char _value); /// puches back the vector with a new legalMove item 


    /*************************************************************************************************
    ; 
    ; Maps to exchange the indexes with positions
    ; 
    **************************************************************************************************/
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

      { 71, "a6" },
      { 72, "b6" },
      { 73, "c6" },
      { 74, "d6" },
      { 75, "e6" },
      { 76, "f6" },
      { 77, "g6" },
      { 78, "h6" },

      { 81, "a7" },
      { 82, "b7" },
      { 83, "c7" },
      { 84, "d7" },
      { 85, "e7" },
      { 86, "f7" },
      { 87, "g7" },
      { 88, "h7" },

      { 91, "a8" },
      { 92, "b8" },
      { 93, "c8" },
      { 94, "d8" },
      { 95, "e8" },
      { 96, "f8" },
      { 97, "g8" },
      { 98, "h8" }
    };

};

#endif // _BOARD_H_