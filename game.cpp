#include "game.h"

//Initialization of static variables of the class Game
Board Game::b = Board();
bool Game::sideFlag = BLACK_SIDE;

//Implements of static Functions
int Game::setPiece(Piece p){
    return b.setPiece(p);
}

Board Game::getBoard(){
    return b;
}

bool Game::getSideFlag(){
    return sideFlag;
}

int Game::switchSide(){
    sideFlag = !sideFlag;
    //b.refresh();
    return 0;
}

bool Game::canPlayerPlay(bool player){
    return (getBoard().getValid(player) != 0);
}
