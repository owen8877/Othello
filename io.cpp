#include "base.h"
#include "element.h"
#include "game.h"
#include "io.h"
#include <ctime>
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
    scanf("%s", input);
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
    clear();
    printf("Under Constructions...\n");
    mypause();
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
        printf("\n");
        printf("-1.Back");
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
            case -1 :
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
    cout << output.is_open() << endl;
    vector<pair<unsigned long long, unsigned long long> > temp = Game::getBoard().getSequence();
    output << temp.size() << endl;
    for (unsigned int i = 0; i < temp.size(); ++i) output << temp[i].first << endl << temp[i].second << endl;
    output << Game::getBoard().getBlackLong() << endl << Game::getBoard().getWhiteLong() << endl;
    output.close();

    return 0;
}

int readrecord(){
    return Game::recoverGame();
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
    if (kbstat['\x1B']) {
        if (Game::getGameStatus() == Playing) Game::pauseGame();
        else if (Game::getGameStatus() == Pause) Game::resumeGame();
    }
    if (kbstat['\x11'] && (Game::getGameStatus() == Lifting) && Settings::btCtrl) isFocus = true;
    if (kbstat['\x0F'] && (Game::getGameStatus() == Lifting) && Settings::btShift) isFocus = true;
    if (!kbstat['\x11'] && Settings::btCtrl) isFocus = false;
    if (!kbstat['\x0F'] && Settings::btShift) isFocus = false;
}

// Keyboard Callback
void keyboardCallback(unsigned char key, int _x, int _y){
    switch (key) {
        //case '\x0D' :
        //case '\x1B' :
            //glutLeaveMainLoop();
            //break;
    }
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
    while (Game::getGameStatus() == Pause) msleep(100);
    while ((Game::getGameStatus() == Playing) && !hasMouseInput) msleep(100);
    hasMouseInput = false;
    //cout << "game status : " << Game::getGameStatus() << endl;
    if (Game::getGameStatus() != Playing) return Piece(0, 0, Game::getGameStatus());
    if (undo) { undo = false; return Piece(0, 0, Undo); }
    return Piece(xBuffer, yBuffer, getSideTag(side));
}
