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
    //return getPieceFromConsole(side);
    return getPieceFromMouse(side);
}

void Player::print(){
    printf("I'm %s\n", (type == PLAYER_AI) ? "AI" : "human");
    printf("My side is %s\n", (side == BLACK_SIDE) ? "Black" : "White");
    return;
}
