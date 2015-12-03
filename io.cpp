#include "base.h"
#include "element.h"
#include "game.h"
#include "io.h"
#include <ctime>
#include <fstream>
#include <GL/freeglut.h>

extern int screenSize, screenHeight, screenWidth;
extern double theta, fai;

int mouseMove = 0;
short xBuffer[10], yBuffer[10];
bool canMouseInput = false;
int mouseButton = 0;

Piece getPieceFromConsole(bool side){
    printf("Please input your piece :\n");
    char input[2];
    scanf("%s", input);
    int x = 0, y = 0;
    if ((input[0]=='u')&&(input[1]=='u')) return Piece(0, 0, Undo);
    if (input[0]=='m') return Piece(0, 0, Menu);
    if ((input[0]=='s')&&(input[1]=='e')) return Piece(0, 0, Settings);
    if ((input[0]=='s')&&(input[1]=='a')) return Piece(0, 0, Save);
    if ((input[0]>='a')&&(input[0]<='h')) y = input[0] - 'a' + 1;
    if ((input[0]>='A')&&(input[0]<='H')) y = input[0] - 'A' + 1;
    if ((input[1]>='1')&&(input[1]<='8')) x = input[1] - '0';
    return Piece(x, y, getSideTag(side));
}

int menu(){
    clear();
    printf("Under Constructions...\n");
    pause();
    return 0;
}

int settings(){
    do {
        clear();
        printf("-->Settings<--\n");
        printf(" 1.Game Assistance\t%s\n", Game::PIECE_ASSISTANCE ? ON : OFF);
        printf("\n");
        printf("-1.Back");
        int input;
        scanf("%d", &input);
        switch (input) {
            case 1 :
                Game::PIECE_ASSISTANCE = !Game::PIECE_ASSISTANCE;
                break;
            case -1 :
                return 0;
            default : ;
        }
    } while (true);
    return 0;
}

int save(){
    time_t t = time(nullptr);
    struct tm *tblock = localtime(&t);
    char buffer[14];
    //stringstream ss;
    //ss << tblock->tm_year << tblock->tm_mon << tblock->tm_mday << tblock->tm_hour << tblock->tm_min << tblock->tm_sec;
    //ss >>fileName;
    sprintf(buffer, "%d%02d%02d%02d%02d%02d", tblock->tm_year+1900, tblock->tm_mon+1, tblock->tm_mday, tblock->tm_hour, tblock->tm_min, tblock->tm_sec);
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

void renewMouseStat(double x, double y, int button){
    static int button_old = 0;
    static double x_old = 0, y_old = 0;
    if (LEFT_MOUSE_BUTTON & button & button_old) {
        theta -= 100 * (x - x_old);
        fai += 100 * (y - y_old);
        if (fai > 180.0) fai = 180.0;
        if (fai < 0.0) fai = 0.0;
        printf("renewMouseStat : theta %lf, fai %lf\n", theta, fai);
    }
    button_old = button;
    x_old = x; y_old = y;
    glutPostRedisplay();
}

// Mouse Callback
void mouseKey(int button, int state, int x, int y){
    //if (!canMouseInput) return;
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
    switch (mouseButton) {
        case LEFT_MOUSE_BUTTON : printf("mousekey() : Left Mouse!\n"); break;
        case MIDDLE_MOUSE_BUTTON : printf("mousekey() : Middle Mouse!\n"); break;
        case RIGHT_MOUSE_BUTTON : printf("mousekey() : Right Mouse!\n"); break;
    }
    renewMouseStat((double) (x - screenWidth/2.0)/screenSize,
                (double) (screenHeight/2.0 - y)/screenSize,
                mouseButton);
    //if (state != GLUT_DOWN) return;
    //cout << screenSize << endl;
    //yBuffer[mouseMove] = (x / (screenSize / BOARD_SIZE)) + 1;
    //xBuffer[mouseMove] = (y / (screenSize / BOARD_SIZE)) + 1;
    //mouseMove++;
    //canMouseInput = false;
    return;
}

//Mouse Passive Callback
void mouseMotion(int x, int y){
    printf("mouseMotion : %d, %d\n", x, y);
    renewMouseStat((double) (x - screenWidth/2.0)/screenSize,
                (double) (screenHeight/2.0 - y)/screenSize,
                mouseButton);
}

//For Mouse Input
Piece getPieceFromMouse(bool side){
    //inputFlag = false;
    canMouseInput = true;
    while (mouseMove == 0) { sleep(100); }
    mouseMove--;
    //inputFlag = true;
    return Piece(xBuffer[mouseMove], yBuffer[mouseMove], getSideTag(side));
}
