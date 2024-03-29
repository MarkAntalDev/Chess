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
#include <cstdlib>
#include <math.h>
#include "eval_tables.h"
#include "defs.h"
#include <windows.h>
#include <unistd.h>
#include <fstream>
// for <streamsize>
#include<ios>     
  
// for numeric_limits
#include<limits> 


/* side note for takenPiece 
; 0 - 7 white pawns ( -1 equals the index in the array)
; 8  white left rook
; 9  white right rook
; 10 white left knight
; 11 white right knight
; 12 white left bishop
; 13 white right bishop
; 14 white queen
; 15 white king
; 
; [16 - 31] same with black 
;
*/

class ChessBoard{
public:
    ChessBoard();

    void OpenBook();
    void CloseBook();

    void drawIntBoard(); // kirajzolja a teljes pályát int -ekkel
    void drawBoard(); // kirajzolja a pályát "szépen"
    void generatePseudoLegalMoves();
    void generatePseudoLegalCaptures();
    void testingFunction();
    int evaluation();
    bool isNthBitSet(int x, int n);
    void addToMoves(int from, int to, int moveT);
    void addPromToMoves(int from, int to, int moveT);
    bool isMoveType(int bits, int type);

    bool MakeMove(moveBytes moveByte);
    void TakeBack();
    void search();
    bool getCp(){return currentPlayer;}
    void printPv();
    void negasearch();
    void AlphaBetaSearch(); /// the final one
    void InitZobrist();
    /// protocols to run in main
    void ConsoleMain();
    void UciMain();

    bool inParseMove;


protected:
    bool currentPlayer; // true-white || false-black
    //bool enPassant; // is en passant an option
private:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    genMove moves[SIZE_OF_MOVES_ARRAY];
    history movesHistory[SIZE_OF_HISTORY];
    int firstMoveOfDepth[MAXIMUM_DEPTH]; /// első lépés indexe minden 
    int currentDepth; /// jelenlegi mélység a keresőfában
    bool playerSide; /// a játékos által választott oldal
    ////////
    // bits for catling
    // 1111
    // 1 for white right caste
    // 2 for white left
    // 4 for black right
    // 8 for black left
    int castleFlag;
    int castled;
    int fifthyMove;
    int historyDepth;
    int ep; /// en passant mező indexe, ha lehetséges (dupla gyalog lépés után a mögötte lévő mező indexe) egyébként -1

    /// Változók a Zobrist Hash-hez
    int ZobristHash[120][12];
    int ZobristEnPassant[120];
    int hash;

    // függvények
    int RandBits();
    void GenerateHash();

    std::map<char, int> BoardToZobrist = {
      { 1, 0 },
      { 2, 1 },
      { 3, 2 },
      { 4, 3 },
      { 5, 4 },
      { 6, 5 },
      { -1, 6 },
      { -2, 7 },
      { -3, 8 },
      { -4, 9 },
      { -5, 10 },
      { -6, 11 }
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////

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
    ; -1 - fekete gyalog    | black pawn
    ; -2 - fekete ló        | black knight
    ; -3 - fekete futó      | black bishop
    ; -4 - fekete bástya    | black rook
    ; -5 - fekete királyanő | black queen
    ; -6 - fekete király    | black king
    ;
    ************************************************************************************************/
    int board[120];

    /*************************************************************************************************
    ; 
    ; All the pieces in one array. It is easier to implement capture like this
    ; Just the right index is needed in case of capturing
    ; 
    **************************************************************************************************/

    /* side note for the pieces (indices in allPieces array)
    ; 0 - 7 white pawns
    ; 8  white left rook
    ; 9  white right rook
    ; 10 white left knight
    ; 11 white right knight
    ; 12 white left bishop
    ; 13 white right bishop
    ; 14 white queen
    ; 15 white king
    ; 
    ; 16 - 23 black pawns
    ; 24 black left rook
    ; 25 black right rook
    ; 26 black left knight
    ; 27 black right knight
    ; 28 black left bishop
    ; 29 black right bishop
    ; 30 black queen
    ; 31 black king
    */
    int allPieces[32];/* = { 31, 32, 33, 34, 35, 36, 37, 38, 21, 28, 22, 27, 23, 26, 24, 25, 
                           81, 82, 83, 84, 85, 86, 87, 88, 91, 98, 92, 97, 93, 96, 94, 95 }*/
                           
    int whitePromoted[8]; // ha 0 akkor nem átalakított, egyébként azt a számot ami a promótált bábut reprezentál
    int blackPromoted[8]; // ha 0 akkor nem átalakított, egyébként azt a számot ami a promótált bábut reprezentál

    /*************************************************************************************************
    ; 
    ; Mozgási konstans offsetek
    ; 
    **************************************************************************************************/

    const int knightOffsets[8] = {-21, -19, -12, -8,  8,  12,  19,  21};
    const int rookOffsets[4] = {-1, 1, -10, 10};
    const int bishopOffsets[4] = {9, 11, -9, -11};


    /*************************************************************************************************
    ; 
    ; WHITE move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; 
    **************************************************************************************************/

    
    void whiteRookMoveGeneration(int pieceIndex);   // white rook move generation function
    void whiteBishopMoveGeneration(int pieceIndex); // white bishop move generation function
    void whiteQueenMoveGeneration(int pieceIndex);  // white queen move generation function (rook + bishop move geenration)
    void whiteKingMoveGeneration();   // white king move generation function (queen without while loop)
    void whiteKnightMoveGeneration(int pieceIndex); // white knight move generation function

    //pawn moves: separate functions because of the complications (double push, en passant,different capture)
    void whiteEnPassantGeneration();
    void whitePawnMove();

    // sáncolási lehetőségek generálása
    void whiteCastleMoveGeneration();

    //A függvényeket megfelelő számokkal meghívó függvénye
    void whiteRookMoveCall();
    void whiteBishopMoveCall();
    void whiteQueenMoveCall();
    void whiteKingMoveCall();
    void whiteKnightMoveCall();

    void callAllWhiteMoveGeneration();

    ////////////////////////////////////
    /// Pseudo legális ütések generálása
    ////////////////////////////////////

    void whiteRookTakeGeneration(int pieceIndex);  
    void whiteBishopTakeGeneration(int pieceIndex); 
    void whiteQueenTakeGeneration(int pieceIndex); 
    void whiteKingTakeGeneration();  
    void whiteKnightTakeGeneration(int pieceIndex);

    void whitePawnTake();

    void callAllWhiteTake();

    /*************************************************************************************************
    ; 
    ; BLACK move generation functions for each piece
    ; !! Pseudo-legal moves are generated !!
    ; Same naming as whites
    ; 
    **************************************************************************************************/

    void blackRookMoveGeneration(int pieceIndex);
    void blackBishopMoveGeneration(int pieceIndex);
    void blackKnightMoveGeneration(int pieceIndex); 
    void blackKingMoveGeneration();
    void blackQueenMoveGeneration(int pieceIndex);

    void blackEnPassantGeneration();
    void blackPawnMove();

    // sáncolási lehetőségek generálása
    void blackCastleMoveGeneration();    


    // a függvényeet megfelelő számokkal meghívó függvénye
    void blackRookMoveCall();
    void blackBishopMoveCall();
    void blackQueenMoveCall();
    void blackKingMoveCall();
    void blackKnightMoveCall();

    void callAllBlackMoveGeneration();

    ////////////////////////////////////
    /// Pseudo legális ütések generálása
    ////////////////////////////////////

    void blackRookTakeGeneration(int pieceIndex);   
    void blackBishopTakeGeneration(int pieceIndex); 
    void blackQueenTakeGeneration(int pieceIndex); 
    void blackKingTakeGeneration();  
    void blackKnightTakeGeneration(int pieceIndex); 

    void blackPawnTake();

    void callAllBlackTake();

    /// Promótált bábuk mozgatása.
    /// Nincs szükség külön függvényre a két félnek
    /// Paraméterként adjuk át hogy ki van soron
    void promotedPiecesMoveGeneration(bool player);
    void promotedPiecesTakeGeneration(bool player);


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// Támadást eldöntő funkciók bábunként
    /// első paraméterként (sq) megkapja a támadott mező indexét
    /// másodikban (attacker) a támadó indexét bábukat tároló többern (allPieces)
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    bool   attackRook(int sq, int attacker);
    bool attackBishop(int sq, int attacker);
    bool attackKnight(int sq, int attacker);
    bool  attackQueen(int sq, int attacker);
    bool   attackKing(int sq, int attacker);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// Kiértékelő függvény
    /// Megadja a győzelmi esélyeket egy adott állásra a megfelelő félnek
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    // **************************************************************************************************************int evaluation();
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// Kiértékelő függvény segédfüggvényeinek közösen használt változói
    /// minden értéket tároló tömb 2 elemű, melynek első eleme a fehér félre vonatkozik, a másodika fekete félre
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    /// ebben a tömbben minden egyes oszlopokban vizsgáljuk a legkevésbé előrehaladottabb gyalogot
    /// alaphelyzet a fehérnek 7 a feketének 0 ugyanis feltételezzük hogy "végtelenségig" előrenyomult 
    /// az adott oszlopban a gyalog 
    /// (ha nincs az adott oszlopban gyalog ez az érték marad)
    /// azért 10 hosszú hogy a széső gyalogoknak később érzékelhessük, hogy nincs mellettük másik
    int pawnRank[2][10];

    /// a nem-gyalog bábuk értékének összessége
    /// futó || ló == 300   bástya == 500   királynő == 900 
    int pieceSum[2];

    /// a gyalogok értékeinek összege
    /// gyalog == 100
    int pawnSum[2];

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// Kiértékelő függvényt segítő függvények
    ///////////////////////////////////////////////////////////////////////////////////////////////////

    int whitePawnScore(int index);
    int blackPawnScore(int index);

    int whiteKingScore();
    int blackKingScore();

    int whiteKingPawnScore(int f);
    int blackKingPawnScore(int f);

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// 
    ///
    /// A KERESŐFÜGGVÉNY
    ///
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////////


    /// A Principle Variation tárolására használt változók
    move pv[MAXIMUM_DEPTH][MAXIMUM_DEPTH];
    std::string pvStr[MAXIMUM_DEPTH][MAXIMUM_DEPTH];
    int pvLength[MAXIMUM_DEPTH];
    int histoH[2][120][120];
    
    int HashReps(); // visszaadja hányszor értük el az adott pozíciót eddig a játékban
    void sortMoves(int i); /// sorbarakja a lépéseket az alapján, hogy mennyi pontjuk van tárolva
    int MinMaxAlphaBeta(int depth, int alpha, int beta);
    int AlphaBetaCaps(int alpha, int beta);
    int nodes; //  akiszámolt node ok mennyisége egy pozícióban
    int maxDepthReached;
    int engineDepth;

    /*************************************************************************************************
    ; 
    ; Nyitási könyv
    ; 
    **************************************************************************************************/

    std::ifstream book; // a book.txt re mutató változó
    int FindBookMove();
    bool hasBook;
    bool followBook;

    /*************************************************************************************************
    ; 
    ; Additional functions
    ; 
    **************************************************************************************************/

    void NewGame();
    int parseMove(std::string moveStr);
    //void putInLegalMoves(char _from, char _to, char _takenPiece, char _value); /// puches back the vector with a new legalMove item 
    int findPiece(int index); /// finds which piece is on the board with the given index
    //void doPawnPromotion(legalMove promotionMove);
    void initBoard(); /// a pályát kezdeti állapotba állítja minden szükséges változójával együtt
    bool attack(int sq, bool side); /// igazat ad vissza ha egy mező támadás alatt áll, hamisat ha nem áll támadás alatt, vagy nincs a pályán
    bool inCheck(bool side); /// A fehér mindig az igaz fekete mindig a hamis. Visszaadja hogy az adott oldal királya sakkban áll-e?
    std::string moveToString(moveBytes move);
    void UciMakeMove(std::string str);
    void SetFromStart(std::string str);
    /*************************************************************************************************
    ; 
    ; Maps to exchange the indexes with positions
    ; 
    **************************************************************************************************/
    std::map<int, std::string> indexToPos = {
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

    /*************************************************************************************************
    ; 
    ; Maps to exchange table positions into indices
    ; Map a táblapozíciók indexre való átváltására
    ; 
    **************************************************************************************************/
    std::map<std::string, int> posToIndex = {
      { "a1", 21 },
      { "b1", 22 },
      { "c1", 23 },
      { "d1", 24 },
      { "e1", 25 },
      { "f1", 26 },
      { "g1", 27 },
      { "h1", 28 },

      { "a2", 31 },
      { "b2", 32 },
      { "c2", 33 },
      { "d2", 34 },
      { "e2", 35 },
      { "f2", 36 },
      { "g2", 37 },
      { "h2", 38 },

      { "a3", 41 },
      { "b3", 42 },
      { "c3", 43 },
      { "d3", 44 },
      { "e3", 45 },
      { "f3", 46 },
      { "g3", 47 },
      { "h3", 48 },

      { "a4", 51 },
      { "b4", 52 },
      { "c4", 53 },
      { "d4", 54 },
      { "e4", 55 },
      { "f4", 56 },
      { "g4", 57 },
      { "h4", 58 },

      { "a5", 61 },
      { "b5", 62 },
      { "c5", 63 },
      { "d5", 64 },
      { "e5", 65 },
      { "f5", 66 },
      { "g5", 67 },
      { "h5", 68 },

      { "a6", 71 },
      { "b6", 72 },
      { "c6", 73 },
      { "d6", 74 },
      { "e6", 75 },
      { "f6", 76 },
      { "g6", 77 },
      { "h6", 78 },

      { "a7", 81 },
      { "b7", 82 },
      { "c7", 83 },
      { "d7", 84 },
      { "e7", 85 },
      { "f7", 86 },
      { "g7", 87 },
      { "h7", 88 },

      { "a8", 91 },
      { "b8", 92 },
      { "c8", 93 },
      { "d8", 94 },
      { "e8", 95 },
      { "f8", 96 },
      { "g8", 97 },
      { "h8", 98 }
    };

    std::map<int, int> indexToValue = {
      {8,  500},
      {9,  500},
      {10, 300},
      {11, 300},
      {12, 300},
      {13, 300},
      {14, 900},
      {24, 500},
      {25, 500},
      {26, 300},
      {27, 300},
      {28, 300},
      {29, 300},
      {30, 900}
    };

    std::map<int, int> numberToValueWhite = {
      {1, 100},
      {2, 300},
      {3, 300},
      {4, 500},
      {5, 900}
    };

    std::map<int, int> numberToValueBlack = {
      {-1, 100},
      {-2, 300},
      {-3, 300},
      {-4, 500},
      {-5, 900}
    };

    std::map<int, int> pieceToMoveValue = {
      {1, 100},
      {2, 300},
      {3, 300},
      {4, 500},
      {5, 900},
      {-1, 100},
      {-2, 300},
      {-3, 300},
      {-4, 500},
      {-5, 900}
    };

    std::map<int, char> ConsolePieces = {
      {0,  '_'},
      {1,  'P'},
      {-1, 'p'},
      {2,  'N'},
      {-2, 'n'},
      {3,  'B'},
      {-3, 'b'},
      {4,  'R'},
      {-4, 'r'},
      {5,  'Q'},
      {-5, 'q'},
      {6,  'K'},
      {-6, 'k'}
    };

    std::map<std::string, int> whitePlayerPromote = {
      {"n",  2},
      {"b",  3},
      {"r",  4},
      {"q",  5}
    };

    std::map<std::string, int> blackPlayerPromote = {
      {"n",  -2},
      {"b",  -3},
      {"r",  -4},
      {"q",  -5}
    };

    std::map<int, std::string> promoteNumberToStr = {
      {2,  "n"},
      {3,  "b"},
      {4,  "r"},
      {5,  "q"},
      {-2,  "n"},
      {-3,  "b"},
      {-4,  "r"},
      {-5,  "q"}
    };
};

#endif // _BOARD_H_