#include "base.h"
#include "element.h"
#include "game.h"
#include "ai.h"
#include "io.h"
#include "display.h"
#include <thread>

using namespace std;

extern void refreshModel(bool lifting);

thread display_t, game;

void Othello_game(int player1, int player2);
int Othello_main(int argc, char **argv);

int main(int argc, char **argv) {
    display_t = thread(displayThread, argc, argv);
    //display_t.join();
    while (Othello_main(argc, argv));
    return 0;
}

int Othello_main(int argc, char **argv){
    clear();
    Game::reset();
    printf("Othello by xDroid\n");
    printf("\n");
    printf(" 1.Play with Friends\n");
    printf(" 2.Play with Watch_Doge\n");
    printf(" 3.Settings\n");
    printf("\n");
    printf("-1.Exit\n");

    int select = 0;
    while (!scanf("%d", &select)) printf("Sorry, but your input is invalid!\n");
    fflush(stdin);
    switch (select) {
        case 1 :
            game = thread(Othello_game, PLAYER_HUMAN, PLAYER_HUMAN);
            game.join();
            break;
        case 2 :
            clear();
            printf("Do you want to play black or white? (B/W)\n");
            char ch;
            while (scanf("%c", &ch)&&(ch!='b')&&(ch!='B')&&(ch!='w')&&(ch!='W')) printf("Sorry, but your input is invalid!\n");
            fflush(stdin);
            switch (ch) {
                case 'b' :
                case 'B' :
                    game = thread(Othello_game, PLAYER_HUMAN, PLAYER_AI);
                    game.join();
                    break;
                case 'w' :
                case 'W' :
                    game = thread(Othello_game, PLAYER_AI, PLAYER_HUMAN);
                    game.join();
                    break;
                default : ;
            }
            break;
        case 3 :
            settings();
            break;
        case -1 :
            return 0;
        default : ;
    }
    return -1;
}

void Othello_game(int player1, int player2){
    Player player[2] = {(player1 == PLAYER_AI) ? AI(BLACK_SIDE) : Player(player1, BLACK_SIDE), (player2 == PLAYER_AI) ? AI(WHITE_SIDE) : Player(player2, WHITE_SIDE)};
    Game::gameStart();
    while (Game::canContinue()) {
        Game::getBoard().print();
        printf("The side is : %s\n", Game::getSideFlag() ? "Black" : "White");
        if (Game::canPlayerPlay(Game::getSideFlag())) {
            if (player[Game::playerIsWho()].getType()==PLAYER_AI) while (Game::setPiece(((AI)player[Game::playerIsWho()]).getPiece()));
            else while (Game::setPiece(player[Game::playerIsWho()].getPiece()));
        }
        else {
            printf("You have no place to toss your piece!\n");
            Game::jump();
        }
        Game::switchSide();
    }

    if (Game::getBoard().getBlackcount() > Game::getBoard().getWhitecount()) printf("Black Wins!!!\n");
    else if (Game::getBoard().getBlackcount() < Game::getBoard().getWhitecount()) printf("White Wins!!!\n");
    else printf("Tie!!!\n");

    //while (Game::getGameStatus() != End) msleep(100)
}
