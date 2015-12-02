#include "game.h"
#include "io.h"

//Initialization of static variables of the class Game
Board Game::b = Board();
bool Game::sideFlag = BLACK_SIDE;
bool Game::PIECE_ASSISTANCE = true;

//Implements of static private Functions
int Game::undo(){
    return b.undo();
}

//Implements of static public Functions
int Game::setPiece(Piece p){
    switch (p.getStatus()) {
        case Undo :
            do { undo(); } while (!canPlayerPlay(sideFlag));
            switchSide();
            return 0;
        case Menu :
            menu();
            switchSide();
            return 0;
        case Settings :
            settings();
            switchSide();
            return 0;
        case Save :
            save();
            switchSide();
            return 0;
        default :
            return b.setPiece(p);
    }
}

Board Game::getBoard(){
    return b;
}

bool Game::getSideFlag(){
    return sideFlag;
}

int Game::playerIsWho(){
    return (Game::getSideFlag()==BLACK_SIDE) ? 0 : 1;
}

int Game::switchSide(){
    sideFlag = !sideFlag;
    //b.refresh();
    return 0;
}

int Game::jump(){
    b.record();
    return 0;
}

int Game::reset(){
    b = Board();
    sideFlag = BLACK_SIDE;
}

bool Game::canPlayerPlay(bool player){
    return (getBoard().getValid(player) != 0);
}

bool Game::neitherCanPlay(){
    return (getBoard().getValid(BLACK_SIDE) == 0) && (getBoard().getValid(WHITE_SIDE)) == 0);
}
