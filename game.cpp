#include "game.h"

//Initialization of static variables of the class Game
board Game::b = board();
bool Game::sideFlag = BLACK_SIDE;

//Implements of static Functions
int Game::setPiece(piece p){
    return b.setPiece(p);
}

board Game::getBoard(){
    return b;
}

bool Game::getSideFlag(){
    return sideFlag;
}

int Game::switchSide(){
    sideFlag = !sideFlag;
    b.refresh();
    return 0;
}

