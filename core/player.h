#ifndef PLAYER_H
#define PLAYER_H

#include "../base.h"
#include "element.h"

class Player{
protected:
    int type;
    bool side;
public:
    Player(int _type, bool _side);
    virtual ~Player() {}
    int getType() const { return type; }
    bool getSide() const { return side; }
    virtual Piece getPiece();
    virtual bool whoami();

    virtual void print();
};

#endif // PLAYER_H
