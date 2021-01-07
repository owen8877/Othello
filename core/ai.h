#ifndef AI_H
#define AI_H

#include "player.h"

class AI : public Player {
public:
    AI(bool side) : Player(PLAYER_AI, side) {}

    AI(Player p) : Player(p) {}

    ~AI() {}

    Piece getPiece();

    bool whoami();

    void print();
};

#endif // AI_H
