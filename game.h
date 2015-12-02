#ifndef GAME_H
#define GAME_H

#include "base.h"
#include "element.h"

class Game{
private:
    static Board b;
    static bool sideFlag;

    static int undo();

public:
    static bool PIECE_ASSISTANCE;

    static int setPiece(Piece p);
    static Board getBoard();
    static bool getSideFlag();
    static int playerIsWho();
    static int switchSide();
    static int jump();
    static int reset();
    static bool canPlayerPlay(bool player);
    static bool neitherCanPlay();
};

#endif // GAME_H
