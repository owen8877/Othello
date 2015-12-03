#include "display.h"
#include "element.h"
#include "base.h"
#include "game.h"
#include "model.h"
#include <cmath>
#include <vector>
#include <GL/freeglut.h>
#include <GL/glut.h>
#include <GL/glu.h>

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
extern double xcenter, ycenter, zcenter;
double theta = 30.0, fai = 45.0;
extern vector<Stone> stones;
extern void initModel();
extern void timerCallback(int index);

void initDisplay(){

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    GLfloat light_0_position [] = {1.0, 0.0, 0.0, 0.0};
    GLfloat light_1_position [] = {0.0, 1.0, 0.0, 0.0};
    GLfloat light_2_position [] = {0.0, 0.0, 1.0, 0.0};
    GLfloat light_0_dir [] = {-2.0, -2.0, 0.0};
    GLfloat color_white [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat color_black [] = {0.0, 0.0, 0.0, 1.0};

    glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, color_black);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, color_white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, color_white);
    glLightfv(GL_LIGHT1, GL_POSITION, light_1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, color_black);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, color_white);
    glLightfv(GL_LIGHT1, GL_SPECULAR, color_white);
    glLightfv(GL_LIGHT2, GL_POSITION, light_2_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, color_black);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, color_white);
    glLightfv(GL_LIGHT2, GL_SPECULAR, color_white);
    //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_0_dir);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0.5);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.5);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
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

void drawStone(){
    glMatrixMode(GL_MODELVIEW);
    int i = 0;
    for (auto stone : stones) {
        glPushMatrix();
        glTranslated(stone.getX(), stone.getY(), stone.getZ());
        glRotated((atan2(stone.getAxisy(), stone.getAxisx() / M_PI) * 180 - 90), 0.0, 0.0, 1.0);
        glRotated((atan2(stone.getAxisz(), sqrt(stone.getAxisx()*stone.getAxisx()+stone.getAxisy()*stone.getAxisy()))) * 180 / M_PI, stone.getAxisy(), -stone.getAxisx(), 0.0);
        glRotated(stone.getAngle(), 0.0, 1.0, 0.0);
            glPushMatrix();
            glTranslated(0.0, 0.0, STONE_HEIGHT / 4);
            glColor3d(0.0, 0.0, 0.0);
            glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 5);
            glPopMatrix();

            //glPushMatrix();
            glTranslated(0.0, 0.0, -STONE_HEIGHT / 4);
            glColor3d(1.0, 1.0, 1.0);
            glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 5);
            //glPopMatrix();

            //glPushMatrix();
            //glTranslated(0.0, 0.0, -STONE_HEIGHT * 3 / 4);
            //glColor3d(0.0, 0.0, 0.0);
            //glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 5);
            //glPopMatrix();
        //glutSolidSphere(STONE_RADIUS, 30, 30);
        glPopMatrix();
    }
}

void drawBackGround(){
    glLineWidth(2);
    glBegin(GL_LINES);
        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(1000.0, 0.0, 0.0);

        glColor3d(0.0, 1.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 1000.0, 0.0);

        glColor3d(0.0, 0.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 1000.0);
    glEnd();

    glBegin(GL_LINES);
    glColor3d(1.0, 1.0, 1.0);
    for (int i = 0; i < 20; ++i) {
        glVertex3d(0.0, 0.5 * (i+1), 0.0);
        glVertex3d(10.0, 0.5 * (i+1), 0.0);
    }
    for (int i = 0; i < 20; ++i) {
        glVertex3d(0.5 * (i+1), 0.0, 0.0);
        glVertex3d(0.5 * (i+1), 10.0, 0.0);
    }
    glEnd();

    glBegin(GL_POINT);
        glPointSize(3);
        glColor3d(1.0, 1.0, 1.0);
        glVertex3d(0.0, 0.0, 0.0);
    glEnd();
}

void display(){
    //printf("display : theta %lf, fai %lf\n", theta, fai);
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);
    //glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.5, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20.0 * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + xcenter,
        20.0 * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + ycenter,
        20.0 * cos(fai / 180.0 * M_PI) + zcenter,
        xcenter, ycenter, zcenter,
        0.0,0.0,1.0);
    //printf("%lf %lf %lf\n", xcenter, ycenter, zcenter);
    glDisable(GL_LIGHTING);
    drawBackGround();
    //glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
    drawStone();

    /*
    glPushMatrix();
    glRotated(60.0, 0.0, 0.0, 1.0);
    glColor3d(1.0, 0.0, 0.0);
    glutSolidCube(1.0);
    glPopMatrix();
    */


    glutSwapBuffers();
}

void reshape(int width, int height){
    /*int screenSize = (width < height ? width : height);
    //cout << "   " << screenSize << endl;
    glViewport(0, 0, width, height);
    glLoadIdentity();
    glOrtho(-(GLfloat)width / screenSize, (GLfloat)width / screenSize,
        -(GLfloat)height / screenSize, (GLfloat)height / screenSize,
        -2.0f, 2.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutPostRedisplay();*/

    glViewport (0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glutPostRedisplay();
}

void displayThread(int argc, char **argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(screenSize, screenSize);
    glutCreateWindow("Othello");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutDisplayFunc(&display);
    glutMouseFunc(&mouseKey);
    glutMotionFunc(&mouseMotion);
    glutReshapeFunc(&reshape);
    initDisplay();
    initModel();
    glutTimerFunc(0, &timerCallback, 0);
    glutMainLoop();
}
