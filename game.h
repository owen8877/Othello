#ifndef GAME_H
#define GAME_H

#include "base.h"
#include "element.h"

class Game{
private:
    static Board b;
    static bool sideFlag;

public:
    static int setPiece(Piece p);
    static Board getBoard();
    static bool getSideFlag();
    static int switchSide();

    static bool canPlayerPlay(bool player);
};

#endif // GAME_H
