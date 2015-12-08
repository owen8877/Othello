#ifndef GAME_H
#define GAME_H

#include "base.h"
#include "element.h"

class Game{
private:
    static Board b;
    static bool sideFlag;
    static Status gameStatus;

    static int undo();

public:
    static int setPiece(Piece p);
    static bool gameStart();
    static Board getBoard();
    static bool getSideFlag();
    static int playerIsWho();
    static int switchSide();
    static int jump();
    static int reset();
    static bool canPlayerPlay(bool player);
    static bool eitherCanPlay();
    static Status getGameStatus();
    static bool canContinue();
    static bool liftTheTable();
    static bool pauseGame();
    static bool resumeGame();
    static void endGame();
    static int recoverGame();
};

class Settings{
public:
    static bool pieceAssistance;
    static bool showBigBall;
    static bool inputMehod; // true stands for GUI Input
    static bool btCtrl;
    static bool btShift;
};
#endif // GAME_H
