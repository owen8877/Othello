#ifndef GAME_H
#define GAME_H

#include "base.h"
#include "element.h"

class Game{
public:
    static int setPiece(piece p);
    static board getBoard();
    static bool getSideFlag();
    static int switchSide();

private:
    static board b;
    static bool sideFlag;
};

#endif // GAME_H
