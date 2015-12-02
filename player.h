#ifndef PLAYER_H
#define PLAYER_H

#include "base.h"
#include "element.h"

class Player{
protected:
    int type;
    bool side;
public:
    Player(int _type, bool _side);
    int getType() const { return type; }
    bool getSide() const { return side; }
    virtual Piece getPiece();

    void print();
};

#endif // PLAYER_H
