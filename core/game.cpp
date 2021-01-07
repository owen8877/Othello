#include "game.h"
#include "../control/io.h"

//Initialization of static variables of the class Game
Board Game::b = Board();
bool Game::sideFlag = BLACK_SIDE;
Status Game::gameStatus = Idle;
bool Game::exceptionalQuit = false;
bool Game::record = false;
int Game::player0 = PLAYER_HUMAN;
int Game::player1 = PLAYER_HUMAN;

//Initialization of static variables of the class Settings
bool Settings::pieceAssistance = true;
bool Settings::showBigBall = false;
bool Settings::inputMehod = true; // true stands for GUI Input
bool Settings::btCtrl = true;
bool Settings::btShift = false;
bool Settings::fancyLights = false;
bool Settings::showAxis = false;;

//Implements of static private Functions
int Game::undo() {
    if (isPlayerAI(1 - playerIsWho()) == PLAYER_AI) return b.undo(2);
    if (!b.undo(1)) {
        switchSide();
        return 0;
    } else return -1;
}

//Implements of static public Functions
bool Game::gameStart(int p0, int p1) {
    if (gameStatus == Idle) {
        gameStatus = Playing;
        player0 = p0;
        player1 = p1;
        if (!exceptionalQuit) sideFlag = BLACK_SIDE;
        exceptionalQuit = false;
        return true;
    }
    return false;
}

int Game::setPiece(Piece p) {
    switch (p.getStatus()) {
        case Undo :
            do { undo(); }
            while (!canPlayerPlay(sideFlag));
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
            readRecord();
            switchSide();
            return 0;
        default :
            return b.setPiece(p);
    }
}

Board Game::getBoard() {
    return b;
}

bool Game::getSideFlag() {
    return sideFlag;
}

int Game::playerIsWho() {
    return (Game::getSideFlag() == BLACK_SIDE) ? 0 : 1;
}

int Game::switchSide() {
    sideFlag = !sideFlag;
    return 0;
}

int Game::jump() {
    b.record();
    return 0;
}

int Game::reset() {
    b = Board();
    sideFlag = BLACK_SIDE;
    gameStatus = Idle;
    return 0;
}

bool Game::canPlayerPlay(bool player) {
    return (getBoard().getValid(player) != 0);
}

bool Game::eitherCanPlay() {
    return ((getBoard().getValid(BLACK_SIDE) != 0) || (getBoard().getValid(WHITE_SIDE)) != 0);
}

Status Game::getGameStatus() {
    return gameStatus;
}

bool Game::canContinue() {
    if (gameStatus == Lifting) return false; // The game can no longer play!!!
    if (gameStatus == End) return false; // The game is ended.
    if (b.full()) return false;
    if (!eitherCanPlay()) printf("Both sides cannot toss their pieces!\n");
    return eitherCanPlay() && (gameStatus == Playing);
}

bool Game::liftTheTable() {
    if (gameStatus == Playing) {
        gameStatus = Lifting;
        return true;
    }
    return false;;
}

bool Game::pauseGame() {
    if (gameStatus == Playing) {
        gameStatus = Pause;
        return true;
    }
    return false;
}

bool Game::resumeGame() {
    if (gameStatus == Pause) {
        gameStatus = Playing;
        return true;
    }
    return false;
}

bool Game::quitGameByException() {
    if (exceptionalQuit) return false;
    exceptionalQuit = true;
    return true;
}

bool Game::quitNormal() {
    if (!exceptionalQuit) return false;
    exceptionalQuit = false;
    return true;
}

void Game::endGame() {
    gameStatus = End;
}

tuple<int, int, int> Game::recoverGame() {
    tuple<int, int, int> r = b.recovery();
    if ((get<0>(r) != 1) && (get<0>(r) != 2)) return make_tuple(-1, 0, 0);
    sideFlag = ((get<0>(r) == 1) ? BLACK_SIDE : WHITE_SIDE);
    printf("The game has been correctedly recovered.\n");
    get<0>(r) = 0;
    return r;
}

bool Game::isExceptionalQuit() {
    return exceptionalQuit;
}

bool Game::hasRecord() {
    record = canReadRecord();
    return record;
}

int Game::isPlayerAI(int player) {
    switch (player) {
        case 0 :
            return player0;
        case 1 :
            return player1;
        default :
            printf("isPlayerAI(int player) : Cannot get the specific player.\n");
            return PLAYER_HUMAN;
    }
}
