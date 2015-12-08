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

void Othello_game(Player* p1, Player* p2);
int Othello_main(int argc, char **argv);

int main(int argc, char **argv) {
    display_t = thread(displayThread, argc, argv);
    //display_t.join();
    while (Othello_main(argc, argv));
    glutLeaveMainLoop();
    return 0;
}

int Othello_main(int argc, char **argv){
    clear();
    Game::reset();
    printf("Othello by xDroid\n");
    printf("\n");
    printf(" 1.Play with Friends\n");
    printf(" 2.Play with Watch_Doge\n");
    printf(" 3.Watch_Doge plays with Watch_Doge\n");
    printf(" 4.Settings\n");
    printf("\n");
    printf("-1.Exit\n");

    int select = 0;
    while (!scanf("%d", &select)) printf("Sorry, but your input is invalid!\n");
    fflush(stdin);
    switch (select) {
        case 1 :
        	{
				Player h1(PLAYER_HUMAN, BLACK_SIDE), h2(PLAYER_HUMAN, WHITE_SIDE);
            	game = thread(Othello_game, &h1, &h2);
            	game.join();
            }
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
                    {
						Player h(PLAYER_HUMAN, BLACK_SIDE);
						AI a(WHITE_SIDE);
            			game = thread(Othello_game, &h, &a);
                    	game.join();
                    }
                    break;
                case 'w' :
                case 'W' :
                	{
	                    Player h(PLAYER_HUMAN, WHITE_SIDE);
						AI a(BLACK_SIDE);
	            		game = thread(Othello_game, &a, &h);
	                    game.join();
	                }
                    break;
                default : ;
            }
            break;
        case 3 :
        	{
	        	AI a1(BLACK_SIDE), a2(WHITE_SIDE);
	        	game = thread(Othello_game, &a1, &a2);
	        	game.join();
	        }
	        mypause();
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

void Othello_game(Player* p1, Player* p2){
    Player* player[2] = {p1, p2};
    Game::gameStart();
    while (Game::canContinue()) {
        Game::getBoard().print();
        printf("The side is : %s\n", Game::getSideFlag() ? "Black" : "White");
        if (Game::canPlayerPlay(Game::getSideFlag())) {
            while (Game::setPiece((player[Game::playerIsWho()])->getPiece()));
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
}
