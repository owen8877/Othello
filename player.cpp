#include "base.h"
#include "element.h"
#include "player.h"
#include "ai.h"
#include "io.h"

Player::Player(int _type, bool _side){
    type = _type;
    side = _side;
}

Piece Player::getPiece(){
    if (type == PLAYER_HUMAN) return getPieceFromIO(side);
    return ((AI)*this).getPiece();
}

void Player::print(){
    printf("I'm %s\n", (type == PLAYER_AI) ? "AI" : "human");
    printf("My side is %s\n", (side == BLACK_SIDE) ? "Black" : "White");
    return;
}
