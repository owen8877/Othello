#include "ai.h"
#include "game.h"
#include <ctime>

Piece AI::getPiece(){
    int myvalid = Game::getBoard().getValid(side);
    srand((unsigned)time(0));
    int which = rand() % myvalid;
    if (!which) which = 1;
    //printf("which is %d\n", which);
    int i, j;
    for (int l = 0; ; ) {
        for (int i = 1; i <= BOARD_SIZE; ++i) {
            for (int j = 1; j <= BOARD_SIZE; ++j) {
                if (Game::getBoard().isValid(i, j, side)) l++;
                if (l == which) return Piece(i, j, getSideTag(side));
            }
        }
    }
}
