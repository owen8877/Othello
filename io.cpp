#include "base.h"
#include "element.h"
#include "game.h"
#include "io.h"
#include <fstream>
#include <cmath>
#include <GL/freeglut.h>

extern int screenSize, screenHeight, screenWidth;
extern double theta, fai;
extern double zoom;
extern double floatingx, floatingy;
extern bool isFocus;

bool hasMouseInput = false;
bool canMouseInput = false;
bool undo = false;
short xBuffer, yBuffer;
int mouseButton = 0;
int kbstat[256] = {0};

Piece getPieceFromConsole(bool side){
    printf("Please input your piece :\n");
    char input[2];
    cin >> input;
    int x = 0, y = 0;
    if ((input[0]=='u')&&(input[1]=='u')) return Piece(0, 0, Undo);
    if (input[0]=='m') return Piece(0, 0, Menu);
    if ((input[0]=='s')&&(input[1]=='e')) return Piece(0, 0, Settings);
    if ((input[0]=='s')&&(input[1]=='a')) return Piece(0, 0, Save);
    if ((input[0]=='r')&&(input[1]=='e')) return Piece(0, 0, Recovery);
    if ((input[0]>='a')&&(input[0]<='h')) y = input[0] - 'a' + 1;
    if ((input[0]>='A')&&(input[0]<='H')) y = input[0] - 'A' + 1;
    if ((input[1]>='1')&&(input[1]<='8')) x = input[1] - '0';
    return Piece(x, y, getSideTag(side));
}

int menu(){
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
        cin >> input;
        switch (input) {
            case 1 :
                if (!save()) {
                    mypause();
                    Game::resumeGame();
                    return 0;
                }
                else {
                    mypause();
                    break;
                }
            case 2 :
                if (!readRecord()) {
                    mypause();
                    Game::resumeGame();
                    return 0;
                }
                else {
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
            default : ;
        }
    } while (true);
    return 0;
}

int loadSettings(){
    char buffer[] = "Settings";
    string fileName(buffer);
    ifstream input((fileName).c_str());
    if (!input.is_open()) {
        printf("Cannot open Settings file.\n");
        return -1;
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

int writeSettings(){
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

int settings(){
    do {
        clear();
        printf("-->Settings<--\n");
        printf(" 1.Game Assistance\t%s\n", Settings::pieceAssistance ? ON : OFF);
        printf(" 2.Show Big Balls(Cool)\t%s\n", Settings::showBigBall ? ON : OFF);
        printf(" 3.Input by GUI\t\t%s\n", Settings::inputMehod ? ON : OFF);
        printf(" 4.Ctrl bullet time\t%s\n", Settings::btCtrl ? ON : OFF);
        printf(" 5.Shift bullet time\t%s\n", Settings::btShift ? ON : OFF);
        printf(" 6.Fancy Lights\t\t%s\n", Settings::fancyLights ? ON : OFF);
        printf(" 7.Show Axises\t\t%s\n", Settings::showAxis ? ON : OFF);
        printf("\n");
        printf("-1.Back\n");
        int input;
        cin >> input;
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
            default : ;
        }
    } while (true);
    return 0;
}

int save(){
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

bool canReadRecord(){
    char buffer[] = "Save";
    string fileName(buffer);
    ifstream input(fileName.c_str());
    bool flag = input.is_open();
    input.close();
    return flag;
}

tuple<int, int, int> initReadRecord(){
    Game::quitGameByException();
    return Game::recoverGame();
}

int readRecord(){
    return get<0>(Game::recoverGame());
}

void renewMouseStat(double x, double y, int button){
    static int button_old = 0;
    static double x_old = 0, y_old = 0;
    if (Game::getGameStatus() == Lifting) {
        if (LEFT_MOUSE_BUTTON & button & button_old) {
            theta -= 100 * (x - x_old);
            fai += 100 * (y - y_old);
            if (fai > 89.0) fai = 89.0;
            if (fai < 1.0) fai = 1.0;
        }
    }
    else if (Game::getGameStatus() == Playing) {
        //They are actually playing.
        floatingx = x;
        floatingy = y;
        if (RIGHT_MOUSE_BUTTON & button) undo = true;
    }
    else if ((Game::getGameStatus() == Pause) && Settings::inputMehod) {
        if (LEFT_MOUSE_BUTTON & button) {
            if ((x >= -0.15) && (x <= 0.15)) {
                if ((y >= 0.15) && (y <= 0.25)) {
                    if (!save()) Game::resumeGame();
                }
                else if ((y >= 0.0) && (y <= 0.1)) {
                    if (!readRecord()) Game::resumeGame();
                }
                else if ((y >= -0.15) && (y <= -0.05)) {
                    if (!save()) {
                        Game::quitGameByException();
                        Game::endGame();
                    }
                }
                else if ((y >= -0.3) && (y <= -0.2)) {
                    Game::quitGameByException();
                    Game::endGame();
                }
            }
        }
    }
    button_old = button;
    x_old = x; y_old = y;
}

double zoomScale(double zoom, bool zoomOut){
    if (zoomOut) return zoom * 0.95 + MAX_ZOOM * 0.05;
    else return zoom * 0.95 + MIN_ZOOM * 0.05;
}

// Mouse Callback
void mouseKey(int button, int state, int x, int y){
    if (GLUT_DOWN == state) switch (button) {
        case GLUT_LEFT_BUTTON:
            mouseButton |= LEFT_MOUSE_BUTTON;
            break;
        case GLUT_MIDDLE_BUTTON:
            mouseButton |= MIDDLE_MOUSE_BUTTON;
            break;
        case GLUT_RIGHT_BUTTON:
            mouseButton |= RIGHT_MOUSE_BUTTON;
            break;
    }
    else switch (button) {
        case GLUT_LEFT_BUTTON:
            mouseButton &= ~LEFT_MOUSE_BUTTON;
            break;
        case GLUT_MIDDLE_BUTTON:
            mouseButton &= ~MIDDLE_MOUSE_BUTTON;
            break;
        case GLUT_RIGHT_BUTTON:
            mouseButton &= ~RIGHT_MOUSE_BUTTON;
            break;
    }
    if (GLUT_WHEEL_DOWN == button) zoom = zoomScale(zoom, true);
    if (GLUT_WHEEL_UP == button) zoom = zoomScale(zoom, false);

    renewMouseStat((double) (x - screenWidth/2.0)/screenSize,
                (double) (screenHeight/2.0 - y)/screenSize,
                mouseButton);

    if (Game::getGameStatus() != Playing) return;
    if (state != GLUT_UP) return;
    if ((GLUT_WHEEL_DOWN == button)||(GLUT_WHEEL_UP == button)) return;

    int step = (screenSize / BOARD_SIZE);

    yBuffer = (x - screenWidth / 2 + screenSize / 2 + screenSize) / step - BOARD_SIZE + 1;
    xBuffer = (y - screenHeight / 2 + screenSize / 2 + screenSize) / step - BOARD_SIZE + 1;
    hasMouseInput = true;
    return;
}

//Mouse Passive Callback
void mouseMotion(int x, int y){
    renewMouseStat((double) (x - screenWidth/2.0)/screenSize,
                (double) (screenHeight/2.0 - y)/screenSize,
                mouseButton);
}

void update(){
    if (kbstat['l'] && (Game::getGameStatus() == Playing)) Game::liftTheTable();
    if (kbstat['\x1B'] && Settings::inputMehod) {
        if (Game::getGameStatus() == Playing) Game::pauseGame();
        else if (Game::getGameStatus() == Pause) Game::resumeGame();
    }
    if (kbstat['\x11'] && (Game::getGameStatus() == Lifting) && Settings::btCtrl) isFocus = true;
    if (kbstat['\x0F'] && (Game::getGameStatus() == Lifting) && Settings::btShift) isFocus = true;

    if (Settings::btCtrl && Settings::btShift) if (!kbstat['\x11'] && !kbstat['\x0F']) isFocus = false;
    if (Settings::btCtrl && !Settings::btShift) if (!kbstat['\x11']) isFocus = false;
    if (!Settings::btCtrl && Settings::btShift) if (!kbstat['\x0F']) isFocus = false;
    if (kbstat['w'] && (Game::getGameStatus() == Playing)) fai = 1.0 * 0.05 + fai * 0.95;
    if (kbstat['s'] && (Game::getGameStatus() == Playing)) fai = 50.0 * 0.05 + fai * 0.95;
}

// Keyboard Callback
void keyboardCallback(unsigned char key, int _x, int _y){
    kbstat[key] = 1;
    update();
}

void keyboardUpCallback(unsigned char key, int x, int y){
    kbstat[key] = 0;
    update();
}

// SpecialKeyboard Callback
void skeyboardCallback(int key, int _x, int _y){
    switch (key) {
        case GLUT_KEY_CTRL_L:
        case GLUT_KEY_CTRL_R:
            kbstat['\x11'] = 1;
            break;
        case GLUT_KEY_SHIFT_L:
        case GLUT_KEY_SHIFT_R:
            kbstat['\x0F'] = 1;
            break;
    }
    update();
}

void skeyboardUpCallback(int key, int x, int y){
    switch (key) {
        case GLUT_KEY_CTRL_L:
        case GLUT_KEY_CTRL_R:
            kbstat['\x11'] = 0;
            break;
        case GLUT_KEY_SHIFT_L:
        case GLUT_KEY_SHIFT_R:
            kbstat['\x0F'] = 0;
            break;
    }
    update();
}

//For Mouse Input
Piece getPieceFromMouse(bool side){
    while (Game::getGameStatus() == Pause) msleep(10);
    while ((Game::getGameStatus() == Playing) && !hasMouseInput) msleep(10);
    hasMouseInput = false;
    if (Game::getGameStatus() != Playing) return Piece(0, 0, Game::getGameStatus());
    if (undo) { undo = false; return Piece(0, 0, Undo); }
    return Piece(xBuffer, yBuffer, getSideTag(side));
}
