#include "game.h"
#include "io.h"
#include "model.h"

//Initialization of static variables of the class Game
Board Game::b = Board();
bool Game::sideFlag = BLACK_SIDE;
bool Game::PIECE_ASSISTANCE = true;
Status Game::gameStatus = Idle;

//Implements of static private Functions
int Game::undo(){
    return b.undo();
}

//Implements of static public Functions
bool Game::gameStart(){
    if (gameStatus == Idle) {
        gameStatus = Playing;
        return true;
    }
    return false;
}

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
        case Idle :
            return 0;
        case Lifting :
            while (gameStatus != End) msleep(100);
            return 0;
        case End :
            return 0;
        case Recovery :
            readrecord();
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
    return 0;
}

int Game::jump(){
    b.record();
    return 0;
}

int Game::reset(){
    b = Board();
    sideFlag = BLACK_SIDE;
    gameStatus = Idle;
    return 0;
}

bool Game::canPlayerPlay(bool player){
    return (getBoard().getValid(player) != 0);
}

bool Game::eitherCanPlay(){
    return ((getBoard().getValid(BLACK_SIDE) != 0) || (getBoard().getValid(WHITE_SIDE)) != 0);
}

Status Game::getGameStatus(){
    return gameStatus;
}

bool Game::canContinue(){
    if (gameStatus == Lifting) return false; // The game can no longer play!!!
    if (gameStatus == End) return false; // The game is ended.
    if (b.full()) return false;
    if (!eitherCanPlay()) printf("Both sides cannot toss their pieces!\n");
    return eitherCanPlay() && (gameStatus == Playing);
}

bool Game::liftTheTable(){
    if (gameStatus == Playing) {
        gameStatus = Lifting;
        return true;
    }
    return false;;
}

bool Game::pauseGame(){
    if (gameStatus == Playing) {
            gameStatus = Pause;
            return true;
    }
    return false;
}

bool Game::resumeGame(){
    if (gameStatus == Pause) {
            gameStatus = Playing;
            return true;
    }
    return false;
}

void Game::endGame(){
    gameStatus = End;
}

int Game::recoverGame(){
    int r = b.recovery();
    if ((r != 1) &&(r != 2)) return -1;
    sideFlag = ((r==1) ? BLACK_SIDE : WHITE_SIDE);
    return 0;
}
