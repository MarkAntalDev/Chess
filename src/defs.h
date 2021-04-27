#ifndef _DEFS_H_
#define _DEFS_H_

/// a programban használt definíciók

#define WHITESIDE		true
#define BLACKSIDE		false
#define WHITE			1
#define BLACK			0

#define SIZE_OF_MOVES_ARRAY     1120
#define MAXIMUM_DEPTH           32
#define SIZE_OF_HISTORY         400

struct legalMove{
  char from;
  char to;
  char takenPiece;
  char value;
};

#define CAPTURE     1
#define CASTLE      2
#define ENPASSANT   4
#define DOUBLEPUSH  8
#define PAWNMOVE    16
#define PROMOTION   32

/////////////////////////////////////
/// Egy lépés bájtjai
/// a moveType egy bájt amely meghatározza milyen mozgás fajta az alapján hogy melyik bitje aktív
///
/// 1 ütés
/// 2 sánc
/// 4 en passant 
/// 8 dupla gyalog tolás
/// 16 gyalog mozgás
/// 32 promóció (gyalog átalakítás)
///
/// promote hasonló módszerrel
///
/// 1 ló
/// 2 futó
/// 4 bástya
/// 8 királynő
///
/////////////////////////////////////
struct moveBytes{
    char from;
    char to;
    char promote;
    char moveType;
};


union move{
    moveBytes b;
    int u;
};

/// egy lépés teljes leírása
/// észben kell tartanunk, hogy ez a lépés még nem biztosan legális
/// ezt a lépés megtételekor vizsgáljuk majd (Board.cpp)
/// a score tartja észben egy adott lépésnek a az értékét 
/// így a kereső funkció sorba tudja rendezni a iértékelendő ütéseket

struct genMove{
    move m;
    int score;
};

/// hogy egy lépést vissza tudjunk csinálni, tárolnunk kell az eddig megtett lépéseket
/// egy lépés visszavonásához szükséges adatokat a következő struktúra tárolja

struct history{
    move m;
    int capture;
    int castleFlag;
    int ep;
    int fifthy;
    int hash;
    int boardFrom;
    int toAllPieces;
};

///typusdefiníciók a könnyebb létrehozás és kezelés érdekében
typedef struct moveBytes moveBytes;
typedef union move move;
typedef struct genMove genMove;
typedef struct history history;

#endif