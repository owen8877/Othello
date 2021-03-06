#include "../base.h"
#include "../core/game.h"
#include "io.h"
#include <fstream>
#include <GLFW/glfw3.h>

extern int screenSize, screenHeight, screenWidth;
extern double floatingx, floatingy;
extern bool isFocus;

bool hasMouseInput = false;
bool canMouseInput = false;
bool undo = false;
short xBuffer, yBuffer;
int mouseButton = 0;

Piece getPieceFromConsole(bool side) {
    printf("Please input your piece :\n");
    char input[2];
    scanf("%s", input);
    int x = 0, y = 0;
    if ((input[0] == 'u') && (input[1] == 'u')) return Piece(0, 0, Undo);
    if (input[0] == 'm') return Piece(0, 0, Menu);
    if ((input[0] == 's') && (input[1] == 'e')) return Piece(0, 0, Settings);
    if ((input[0] == 's') && (input[1] == 'a')) return Piece(0, 0, Save);
    if ((input[0] == 'r') && (input[1] == 'e')) return Piece(0, 0, Recovery);
    if ((input[0] >= 'a') && (input[0] <= 'h')) y = input[0] - 'a' + 1;
    if ((input[0] >= 'A') && (input[0] <= 'H')) y = input[0] - 'A' + 1;
    if ((input[1] >= '1') && (input[1] <= '8')) x = input[1] - '0';
    return Piece(x, y, getSideTag(side));
}

int menu() {
    do {
        clear();
        Game::pauseGame();
        printf("-->Menu<--\n");
        printf(" 1.Save the game\n");
        printf(" 2.Load previous SAVE file\n");
        printf(" 3.Save and quit\n");
        printf(" 4.Quit without Saving\n");
        printf(" 5.Settings\n");
        printf("\n");
        printf("-1.Back to game\n");

        int input;
        scanf("%d", &input);
        switch (input) {
            case 1 :
                if (!save()) {
                    mypause();
                    Game::resumeGame();
                    return 0;
                } else {
                    mypause();
                    break;
                }
            case 2 :
                if (!readRecord()) {
                    mypause();
                    Game::resumeGame();
                    return 0;
                } else {
                    mypause();
                    break;
                }
                return 0;
            case 3 :
                if (!save()) {
                    Game::quitGameByException();
                    Game::endGame();
                    return 0;
                }
                break;
            case 4 :
                printf("Are you sure to quit without saving? (Progress will be lost!) (y/n)");
                {
                    string s;
                    cin >> s;
                    if (s[0] == 'y') {
                        Game::quitGameByException();
                        Game::endGame();
                        return 0;
                    }
                }
                break;
            case 5 :
                settings();
                break;
            case -1 :
                Game::resumeGame();
                return 0;
            default :;
        }
    } while (true);
    return 0;
}

int loadSettings() {
    char buffer[] = "Settings";
    string fileName(buffer);
    ifstream input((fileName).c_str());
    if (!input.is_open()) {
        printf("Cannot open Settings file.\n");
        Settings::btCtrl = true;
        Settings::btShift = false;
        Settings::fancyLights = true;
        Settings::inputMehod = false;
        Settings::pieceAssistance = true;
        Settings::showAxis = true;
        Settings::showBigBall = false;
        return 0;
    }
    input >> Settings::btCtrl;
    input >> Settings::btShift;
    input >> Settings::fancyLights;
    input >> Settings::inputMehod;
    input >> Settings::pieceAssistance;
    input >> Settings::showAxis;
    input >> Settings::showBigBall;
    input.close();
    return 0;
}

int writeSettings() {
    char buffer[] = "Settings";
    string fileName(buffer);
    ofstream output((fileName).c_str());
    if (!output.is_open()) {
        printf("Cannot open Settings file.\n");
        return -1;
    }
    output << Settings::btCtrl << endl;
    output << Settings::btShift << endl;
    output << Settings::fancyLights << endl;
    output << Settings::inputMehod << endl;
    output << Settings::pieceAssistance << endl;
    output << Settings::showAxis << endl;
    output << Settings::showBigBall << endl;
    output.close();
    //printf("Settings has been correctedly saved.\n");
    return 0;
}

int settings() {
    do {
        clear();
        printf("-->Settings<--\n");
        printf(" 1.Game Assistance\t%s\n", Settings::pieceAssistance ? ON : OFF);
        printf(" 2.Show Big Balls(Cool)\t%s\n", Settings::showBigBall ? ON : OFF);
        printf(" 3.Input by GUI\t\t%s\n", Settings::inputMehod ? ON : OFF);
        printf(" 4.Ctrl bullet time\t%s\n", Settings::btCtrl ? ON : OFF);
        printf(" 5.Shift bullet time\t%s\n", Settings::btShift ? ON : OFF);
        printf(" 6.Fancy updateLights\t\t%s\n", Settings::fancyLights ? ON : OFF);
        printf(" 7.Show Axises\t\t%s\n", Settings::showAxis ? ON : OFF);
        printf("\n");
        printf("-1.Back\n");
        int input;
        scanf("%d", &input);
        switch (input) {
            case 1 :
                Settings::pieceAssistance = !Settings::pieceAssistance;
                break;
            case 2 :
                Settings::showBigBall = !Settings::showBigBall;
                break;
            case 3 :
                Settings::inputMehod = !Settings::inputMehod;
                break;
            case 4 :
                Settings::btCtrl = !Settings::btCtrl;
                break;
            case 5 :
                Settings::btShift = !Settings::btShift;
                break;
            case 6 :
                Settings::fancyLights = !Settings::fancyLights;
                break;
            case 7 :
                Settings::showAxis = !Settings::showAxis;
                break;
            case -1 :
                writeSettings();
                return 0;
            default :;
        }
    } while (true);
    return 0;
}

int save() {
    char buffer[] = "Save";
    string fileName(buffer);
    ofstream output((fileName).c_str());
    if (!output.is_open()) {
        printf("Cannot write SAVE file.\n");
        return -1;
    }
    vector<pair<unsigned long long, unsigned long long> > temp = Game::getBoard().getSequence();
    output << temp.size() << endl;
    for (unsigned int i = 0; i < temp.size(); ++i) output << temp[i].first << endl << temp[i].second << endl;
    output << Game::getBoard().getBlackLong() << endl << Game::getBoard().getWhiteLong() << endl;
    output << Game::isPlayerAI(0) << endl << Game::isPlayerAI(1) << endl;
    output.close();
    printf("File has been correctedly saved.\n");
    return 0;
}

bool canReadRecord() {
    char buffer[] = "Save";
    string fileName(buffer);
    ifstream input(fileName.c_str());
    bool flag = input.is_open();
    input.close();
    return flag;
}

tuple<int, int, int> initReadRecord() {
    Game::quitGameByException();
    return Game::recoverGame();
}

int readRecord() {
    return get<0>(Game::recoverGame());
}

void renewMouseStat(double x, double y, int button) {
    static int button_old = 0;
    static double x_old = 0, y_old = 0;
    if (Game::getGameStatus() == Lifting) {
        if (LEFT_MOUSE_BUTTON & button & button_old) {
//            theta -= 100 * (x - x_old);
//            fai += 100 * (y - y_old);
//            if (fai > 89.0) fai = 89.0;
//            if (fai < 1.0) fai = 1.0;
        }
    } else if (Game::getGameStatus() == Playing) {
        //They are actually playing.
        floatingx = x;
        floatingy = y;
        if (RIGHT_MOUSE_BUTTON & button) undo = true;
    } else if ((Game::getGameStatus() == Pause) && Settings::inputMehod) {
        if (LEFT_MOUSE_BUTTON & button) {
            if ((x >= -0.15) && (x <= 0.15)) {
                if ((y >= 0.15) && (y <= 0.25)) {
                    if (!save()) Game::resumeGame();
                } else if ((y >= 0.0) && (y <= 0.1)) {
                    if (!readRecord()) Game::resumeGame();
                } else if ((y >= -0.15) && (y <= -0.05)) {
                    if (!save()) {
                        Game::quitGameByException();
                        Game::endGame();
                    }
                } else if ((y >= -0.3) && (y <= -0.2)) {
                    Game::quitGameByException();
                    Game::endGame();
                }
            }
        }
    }
    button_old = button;
    x_old = x;
    y_old = y;
}

// Mouse Callback
void handleMouseButton(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    if (action == GLFW_PRESS) switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButton |= LEFT_MOUSE_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButton |= MIDDLE_MOUSE_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
        default:
            mouseButton |= RIGHT_MOUSE_BUTTON;
            break;
    } else switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            mouseButton &= ~LEFT_MOUSE_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            mouseButton &= ~MIDDLE_MOUSE_BUTTON;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
        default:
            mouseButton &= ~RIGHT_MOUSE_BUTTON;
            break;
    }

    renewMouseStat((double) (x - screenWidth / 2.0) / screenSize,
                   (double) (screenHeight / 2.0 - y) / screenSize,
                   mouseButton);

    if (Game::getGameStatus() != Playing) return;
    if (button != GLFW_RELEASE) return;

    int step = (screenSize / BOARD_SIZE);

    yBuffer = (x - screenWidth / 2 + screenSize / 2 + screenSize) / step - BOARD_SIZE + 1;
    xBuffer = (y - screenHeight / 2 + screenSize / 2 + screenSize) / step - BOARD_SIZE + 1;
    hasMouseInput = true;
}

//Mouse Passive Callback
void handleMouseMotion(int x, int y) {
    renewMouseStat((double) (x - screenWidth / 2.0) / screenSize,
                   (double) (screenHeight / 2.0 - y) / screenSize,
                   mouseButton);
}

//For Mouse Input
Piece getPieceFromMouse(bool side) {
#ifdef __EMSCRIPTEN__
    if (Game::getGameStatus() == Pause || ((Game::getGameStatus() == Playing) && !hasMouseInput)) {
        return Piece(0, 0, Waiting);
    }
#else
    while (Game::getGameStatus() == Pause) msleep(10);
    while ((Game::getGameStatus() == Playing) && !hasMouseInput) msleep(10);
#endif
    hasMouseInput = false;
    if (Game::getGameStatus() != Playing) return Piece(0, 0, Game::getGameStatus());
    if (undo) {
        undo = false;
        return Piece(0, 0, Undo);
    }
    return Piece(xBuffer, yBuffer, getSideTag(side));
}
