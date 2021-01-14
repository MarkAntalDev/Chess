#ifndef _BOARD_H_
#define _BOARD_H_

class ChessBoard{
public:
    ChessBoard();
    //boardToOriginal(); //// visszaállítja a board ot az eredeti állapotába
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
      7,  4,  2,  3,  6,  5,  3,  2,  4,  7,
      7,  1,  1,  1,  1,  1,  1,  1,  1,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7,  0,  0,  0,  0,  0,  0,  0,  0,  7,
      7, -1, -1, -1, -1, -1, -1, -1, -1,  7,
      7, -4, -2, -3, -6, -5, -3, -2, -4,  7,
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7,
      7,  7,  7,  7,  7,  7,  7,  7,  7,  7
    };

    char 
}

#endif // _BOARD_H_