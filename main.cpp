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

void Othello_game(int p0, int p1);
int Othello_main(int argc, char **argv);

int main(int argc, char **argv) {
    loadSettings();
    display_t = thread(displayThread, argc, argv);
    while (Othello_main(argc, argv));
    glutLeaveMainLoop();
    display_t.join();
    return 0;
}

int Othello_main(int argc, char **argv){
    clear();
    Game::reset();
    printf("Othello by xDroid\n");
    printf("\n");
    if (canReadRecord()) printf(" 0.Read SAVE file to resume game\n");
    printf(" 1.Play with Friends\n");
    printf(" 2.Play with Watch_Doge\n");
    printf(" 3.Watch_Doge plays with Watch_Doge\n");
    printf(" 4.Settings\n");
    printf("\n");
    printf("-1.Exit\n");

    int select = 0;
    int player0, player1;
    while (!scanf("%d", &select)) printf("Sorry, but your input is invalid!\n");
    fflush(stdin);
    switch (select) {
        case 0 :
            if (canReadRecord()) {
                tie (ignore, player0, player1) = initReadRecord();
                game = thread(Othello_game, player0, player1);
                game.join();
            }
            break;
        case 1 :
            game = thread(Othello_game, PLAYER_HUMAN, PLAYER_HUMAN);
            game.join();
            break;
        case 2 :
            clear();
            printf("Do you want to play black or white? (B/W)\n");
            char ch;
            scanf("%c", &ch);
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
            game = thread(Othello_game, PLAYER_AI, PLAYER_AI);
            game.join();
	        break;
        case 4 :
            settings();
            break;
        case -1 :
            return 0;
        default : ;
    }
    return -1;
}

void Othello_game(int p0, int p1){
    Player *player0, *player1;
    if (p0 == PLAYER_HUMAN) player0 = new Player(PLAYER_HUMAN, BLACK_SIDE);
    else player0 = new AI(BLACK_SIDE);

    if (p1 == PLAYER_HUMAN) player1 = new Player(PLAYER_HUMAN, WHITE_SIDE);
    else player1 = new AI(WHITE_SIDE);

    Player* player[2] = {player0, player1};
    Game::gameStart(player[0]->whoami(), player[1]->whoami());
    while (Game::canContinue()) {
        Game::getBoard().print();
        //player[Game::playerIsWho()]->print();
        if (Game::canPlayerPlay(Game::getSideFlag())) {
            while (Game::setPiece((player[Game::playerIsWho()])->getPiece()));
        }
        else {
            printf("You have no place to toss your piece!\n");
            Game::jump();
        }
        Game::switchSide();
    }

    if (!Game::isExceptionalQuit()) {
        if (Game::getBoard().getBlackcount() > Game::getBoard().getWhitecount()) printf("Black Wins!!!\n");
        else if (Game::getBoard().getBlackcount() < Game::getBoard().getWhitecount()) printf("White Wins!!!\n");
        else printf("Tie!!!\n");
        mypause();
    }
    delete player0;
    delete player1;
    Game::quitNormal();
}
