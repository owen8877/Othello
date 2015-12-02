#include "display.h"
#include "element.h"
#include "base.h"
#include "game.h"
#include <cmath>

int CIRCLE_MAX = 50;
int screenSize = 50 * BOARD_SIZE;

void initDisplay(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void drawCircle(double x, double y, double r, Status s){
    switch (s) {
        case Black:
            glColor3d(0.0, 0.0, 0.0);
            break;
        case White:
            glColor3d(1.0, 1.0, 1.0);
            break;
        default : ;
    }
    glBegin(GL_POLYGON);
    for (int i = 0; i < CIRCLE_MAX; ++i)
        glVertex2d(x + r*cos(2 * M_PI*i / CIRCLE_MAX), y + r*sin(2 * M_PI*i / CIRCLE_MAX));
    glEnd();
}

void drawHint(double x, double y, double r){
    glColor3d(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    for (int i = 0; i < 2; ++i) {
        glVertex2d(x + r*cos(i*M_PI / 2), y + r*sin(i*M_PI / 2));
        glVertex2d(x + r*cos(i*M_PI / 2 + M_PI), y + r*sin(i*M_PI / 2 + M_PI));
    }
    glEnd();
}

void drawBoard(){
    //Draw background
    glClearColor(0.54f, 0.14f, 0.0f, 0.8f);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3d(0.0, 0.0, 0.0);
    glLineWidth(3);
    glBegin(GL_LINES);
    for (int i = 1; i < BOARD_SIZE; ++i) {
        glVertex2d(-1.0, i * step - 1.0);
        glVertex2d(1.0, i * step - 1.0);
    }
    for (int i = 1; i < BOARD_SIZE; ++i) {
        glVertex2d(i * step - 1.0, -1.0);
        glVertex2d(i * step - 1.0, 1.0);
    }
    glEnd();
}

void drawPiece(){
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j) {
            double x = -1 - hstep + step*j;
            double y = 1 + hstep - step*i;
            //printf("i %d, j %d, x %lf, y %lf\n", i, j, x, y);
            switch (Game::getBoard().getPiece(i,j).getStatus()) {
                case Black :
                    drawCircle(x, y, 0.85 / BOARD_SIZE, Black);
                    break;
                case White :
                    drawCircle(x, y, 0.85 / BOARD_SIZE, White);
                    break;
                case Empty :
                    break;
                case BlackValid :
                    if ((getValidTag(Game::getSideFlag()) & BlackValid) && Game::PIECE_ASSISTANCE) drawHint(x, y, 0.75 / BOARD_SIZE);
                    break;
                case WhiteValid :
                    if ((getValidTag(Game::getSideFlag()) & WhiteValid) && Game::PIECE_ASSISTANCE) drawHint(x, y, 0.75 / BOARD_SIZE);
                    break;
                case Valid :
                    if ((getValidTag(Game::getSideFlag()) & Valid) && Game::PIECE_ASSISTANCE) drawHint(x, y, 0.75 / BOARD_SIZE);
                    break;
                default : ;
            }
        }
    }
}

void display(){
    drawBoard();
    drawPiece();
    cout << "Hi!" << endl;
    glFlush();
}

void reshape(int width, int height){
    int screenSize = (width < height ? width : height);
    //cout << "   " << screenSize << endl;
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(-(GLfloat)width / screenSize, (GLfloat)width / screenSize,
        -(GLfloat)height / screenSize, (GLfloat)height / screenSize,
        -2.0f, 2.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();
}

void displayThread(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenSize, screenSize);
    glutCreateWindow("Othello");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(&display);
    glutMouseFunc(&mouseKey);
    glutReshapeFunc(&reshape);
    initDisplay();
    glutMainLoop();
}
