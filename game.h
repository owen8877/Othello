#ifndef GAME_H
#define GAME_H

#include "base.h"
#include "element.h"

class Game{
private:
    static Board b;
    static bool sideFlag;
    static Status gameStatus;
    static bool exceptionalQuit;
    static bool record;
    static int player0, player1;

    static int undo();

public:
    static bool recoveryAtStart;

    static int setPiece(Piece p);
    static bool gameStart(int p0, int p1);
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
    static bool quitGameByException();
    static bool quitNormal();
    static void endGame();
    static tuple<int, int, int> recoverGame();
    static bool isExceptionalQuit();
    static bool hasRecord();
    static int isPlayerAI(int number);
};

class Settings{
public:
    static bool pieceAssistance;
    static bool showBigBall;
    static bool inputMehod; // true stands for GUI Input
    static bool btCtrl;
    static bool btShift;
    static bool fancyLights;
    static bool showAxis;
};
#endif // GAME_H
