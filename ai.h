#ifndef AI_H
#define AI_H

#include "element.h"
#include "base.h"
#include "player.h"

class AI : public Player{
public:
    AI(bool side) : Player(PLAYER_AI, side) {};
    AI(Player p) : Player(p) {};
    Piece getPiece();
};

#endif // AI_H
