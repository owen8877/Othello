#include "base.h"
#include "element.h"
#include "game.h"

using namespace std;

int main(int argc, char **argv) {
    printf("Othello by xDroid\n");

    while (!Game::getBoard().full()) {
        Game::getBoard().print();
        printf("the side is : %s\n", Game::getSideFlag() ? "Black" : "White");
        int m, n;
        do {
            scanf("%d%d", &m, &n);
        }
        while (Game::setPiece(piece(m, n, (Game::getSideFlag() ? Black : White))));
        Game::switchSide();
    }

    if (Game::getBoard().getBlackcount() > Game::getBoard().getBlackcount()) printf("You Win!!!\n");
    else if (Game::getBoard().getBlackcount() < Game::getBoard().getBlackcount()) printf("You Lose!!!\n");
    else printf("Tie!!!\n");

    return 0;
}
