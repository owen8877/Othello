#include "ai.h"
#include "game.h"

extern Piece getMove(bool isBlack, const Record &record);

Piece AI::getPiece() {
    msleep(300);
    Piece proposal = side == BLACK_SIDE ? getMove(true, Game::getBoard().blackRecord) : getMove(false, Game::getBoard().whiteRecord);
    return {proposal.getX(), proposal.getY(), getSideTag(side)};
}

bool AI::whoami() {
    return PLAYER_AI;
}

void AI::print() {
    printf("I'm AI\n");
    printf("My side is %s\n", (side == BLACK_SIDE) ? "Black" : "White");
    return;
}
