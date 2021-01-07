#include "player.h"
#include "game.h"
#include "../control/io.h"

Player::Player(int _type, bool _side) {
    type = _type;
    side = _side;
}

Piece Player::getPiece() {
    return (Settings::inputMehod) ? getPieceFromMouse(side) : getPieceFromConsole(side);
}

void Player::print() {
    printf("My side is %s\n", (side == BLACK_SIDE) ? "Black" : "White");
    return;
}

bool Player::whoami() {
    return PLAYER_HUMAN;
}
