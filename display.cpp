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
#include "math3d.h"

int screenSize = 80 * BOARD_SIZE, screenWidth = 80 * BOARD_SIZE, screenHeight = 80 * BOARD_SIZE;
double zoom = 11;
double theta = -90.5, fai = 20.0;
double chosenx = 0.0, choseny = 0.0;
GLfloat light_0_position [] = {0.0, 0.0, 40.0, 1.0};
GLfloat light_0_dir [] = {0.0, 0.0, -1.0};

extern double xcenter, ycenter, zcenter;
extern vector<Stone> stones;
extern void initModel();
extern void timerCallback(int index);
extern void keyboardCallback(unsigned char key, int _x, int _y);
extern void keyboardUpCallback(unsigned char key, int _x, int _y);

inline void setVertexColor(double x, double y) {
    //((x <= FLOOR_SHADE_SIZE)&&(x >= -FLOOR_SHADE_SIZE)&&(y <= FLOOR_SHADE_SIZE)&&(y >= -FLOOR_SHADE_SIZE)) ? glColor3d(0.0, 0.0, 0.0) : glColor3d(1.0, 1.0, 1.0);
    double p = pow(((pow(x, 8) + pow(y, 8)) / 2.0), 0.125) / FLOOR_SHADE_SIZE;
    //double temp = -(p-1.1) * (p-2.05) * 2.5 + 0.17;
    //double temp = (p < 1.834) ? (-(p-1.1) * (p-2.05) * 2.5 + 0.17) : (0.56636 + (p - 1.834) * 5);
    double temp = (p < 2) ? (-(p-2)*(p-2) + 1) : 1.0;
    glColor3d(temp, temp, temp);
}

void initDisplay(){
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_NORMALIZE);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);

    GLfloat ambient [] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuse [] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specref[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specref);
    glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 10);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void Lights(){
    GLfloat light_1_position [] = {1.0, 0.0, 0.0, 0.0};
    GLfloat light_2_position [] = {0.0, 1.0, 0.0, 0.0};
    GLfloat light_3_position [] = {0.0, 0.0, 1.0, 0.0};

    GLfloat color_white [] = {1.0, 1.0, 1.0, 1.0};
    GLfloat color_black [] = {0.0, 0.0, 0.0, 1.0};
    GLfloat ambientLight [] = {0.3f, 0.3f, 0.3f, 1.0f};
    GLfloat diffuseLight [] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat globalAmbient [] = {0.1, 0.1, 0.1, 1.0};

    glLightfv(GL_LIGHT1, GL_POSITION, light_1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT2, GL_POSITION, light_2_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT3, GL_POSITION, light_3_position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_0_dir);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0.5);
    glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0.5);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 90.0);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 0.5);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 30);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

    //glEnable(GL_LIGHT1);
    //glEnable(GL_LIGHT2);
    //glEnable(GL_LIGHT3);
    glEnable(GL_LIGHT0);
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
        if ((stone.getColor() != Black) && (stone.getColor() != White)) continue;
        glPushMatrix();
        glTranslated(stone.getX(), stone.getY(), stone.getZ());
        glRotated((atan2(stone.getAxisy(), stone.getAxisx() / M_PI) * 180 - 90), 0.0, 0.0, 1.0);
        glRotated((atan2(stone.getAxisz(), sqrt(stone.getAxisx()*stone.getAxisx()+stone.getAxisy()*stone.getAxisy()))) * 180 / M_PI, stone.getAxisy(), -stone.getAxisx(), 0.0);
        glRotated(stone.getAngle(), 0.0, 1.0, 0.0);
        glTranslated(0.0, 0.0, STONE_HEIGHT / 4);
        if (stone.getColor() == White) { glScaled(1.0, 1.0, -1.0); glTranslated(0.0, 0.0, -STONE_HEIGHT / 2); }
            glPushMatrix();
            glTranslated(0.0, 0.0, STONE_HEIGHT / 4);
            glColor3d(0.0, 0.0, 0.0);
            glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 5);
            glPopMatrix();

            glTranslated(0.0, 0.0, -STONE_HEIGHT / 4);
            glColor3d(1.0, 1.0, 1.0);
            glutSolidCylinder(STONE_RADIUS, STONE_HEIGHT / 2, 20, 5);
        glPopMatrix();
    }
}

void drawBackGround(){
    glDisable(GL_LIGHTING);
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

    glEnable(GL_LIGHTING);
    glBegin(GL_QUAD_STRIP);
        glColor3d(1.0, 1.0, 1.0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_SIZE, -FLOOR_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_SIZE, FLOOR_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_CENTER_SIZE, FLOOR_CENTER_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(FLOOR_SIZE, FLOOR_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(FLOOR_CENTER_SIZE, FLOOR_CENTER_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(FLOOR_SIZE, -FLOOR_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_SIZE, -FLOOR_SIZE, 0);
        glNormal3d(0.0, 0.0, 1.0);
        glVertex3i(-FLOOR_CENTER_SIZE, -FLOOR_CENTER_SIZE, 0);
    glEnd();

    glBegin(GL_QUADS);
        for (double i = -FLOOR_CENTER_SIZE; i < FLOOR_CENTER_SIZE; i += FLOOR_GRID_SIZE) {
            for (double j = -FLOOR_CENTER_SIZE; j < FLOOR_CENTER_SIZE; j += FLOOR_GRID_SIZE) {
                setVertexColor(i, j);
                glVertex3d(i, j, 0);
                setVertexColor(i, j+FLOOR_GRID_SIZE);
                glVertex3d(i, j+FLOOR_GRID_SIZE, 0);
                setVertexColor(i+FLOOR_GRID_SIZE, j+FLOOR_GRID_SIZE);
                glVertex3d(i+FLOOR_GRID_SIZE, j+FLOOR_GRID_SIZE, 0);
                setVertexColor(i+FLOOR_GRID_SIZE, j);
                glVertex3d(i+FLOOR_GRID_SIZE, j, 0);
            }
        }
    glEnd();
    glDisable(GL_LIGHTING);

}

void drawCursor(){
    glEnable(GL_LIGHTING);
    glTranslated(0.0, 0.0, 0.01);

    double alignx = int(chosenx * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
    double aligny = int(choseny * BOARD_SIZE + BOARD_SIZE) / (BOARD_SIZE + 0.0) - 1.0;
    double tiny = 1.0 / BOARD_SIZE;
    if (alignx < -0.5) alignx = -0.5;
    if (aligny < -0.5) aligny = -0.5;
    if (alignx > 0.5 - 1.0 / BOARD_SIZE) alignx = 0.5 - 1.0 / BOARD_SIZE;
    if (aligny > 0.5 - 1.0 / BOARD_SIZE) aligny = 0.5 - 1.0 / BOARD_SIZE;
    printf("alignx : %lf, aligny : %lf\n", alignx, aligny);
    glBegin(GL_QUAD_STRIP);
        glColor3d(1.0, 0.0, 0.0);
        glVertex3d(alignx, aligny, 0.0);
        glVertex3d(alignx+tiny, aligny, 0.0);
        glVertex3d(alignx, aligny+tiny, 0.0);
        glVertex3d(alignx+tiny, aligny+tiny, 0.0);
    glEnd();
}

void drawTable(){
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);

    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0);
    glPushMatrix();
    glTranslated(0.0, 0.0, TABLE_HEIGHT);
    glScaled(TABLE_SIZE , TABLE_SIZE , TABLE_THICKNESS);
    glColor3d(1.0, 1.0, 1.0);
    glutSolidCube(1);
    glTranslated(0.0, 0.0, 0.5);
    double scaleCoeffiency = BOARD_SIZE * STONE_INTERVAL / TABLE_SIZE;
    glScaled(scaleCoeffiency, scaleCoeffiency, scaleCoeffiency);
    glLineWidth(3);
    glBegin(GL_LINES);
        for (double i = 1.0; i < BOARD_SIZE; ++i) {
            glVertex3d(i / BOARD_SIZE - 0.5, -0.5, 0.0);
            glVertex3d(i / BOARD_SIZE - 0.5, 0.5, 0.0);
        }
    glEnd();

    glBegin(GL_LINES);
        for (double i = 1.0; i < BOARD_SIZE; ++i) {
            glVertex3d(-0.5, i / BOARD_SIZE - 0.5, 0.0);
            glVertex3d(0.5, i / BOARD_SIZE - 0.5, 0.0);
        }
    glEnd();
    drawCursor();
    glPopMatrix();
}

void display(){
    double backGroundColor = 0.104;
    glClearColor(backGroundColor, backGroundColor, backGroundColor, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(90.0, 1, 0.5, 1000);
    //glOrtho(-(GLfloat)screenWidth / screenSize, (GLfloat)screenWidth / screenSize,
    //    -(GLfloat)screenHeight / screenSize, (GLfloat)screenHeight / screenSize,
    //    -1.0f, 10.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (Game::areTheyPlaying() == LIFTING) gluLookAt(zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + xcenter,
        zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI) + ycenter,
        zoom * cos(fai / 180.0 * M_PI) + zcenter,
        xcenter, ycenter, zcenter,
        0.0,0.0,1.0);
    else gluLookAt(zoom * cos(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
        zoom * sin(theta / 180.0 * M_PI) * sin(fai / 180.0 * M_PI),
        zoom * cos(fai / 180.0 * M_PI),
        0.0,0.0,TABLE_HEIGHT + TABLE_THICKNESS / 2,
        0.0,0.0,1.0);
    Lights();
    drawBackGround();
    drawTable();
    drawStone();

    glutSwapBuffers();
}

void reshape(int width, int height){
    screenSize = (width < height ? width : height);
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-(GLfloat)screenWidth / screenSize, (GLfloat)screenWidth / screenSize,
        -(GLfloat)screenHeight / screenSize, (GLfloat)screenHeight / screenSize,
        -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT);
    //glutPostRedisplay();


    //glLoadIdentity ();
    //gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 20.0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
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
    glutKeyboardFunc(&keyboardCallback);
    glutKeyboardUpFunc(&keyboardUpCallback);
    initDisplay();
    initModel();
    glutTimerFunc(0, &timerCallback, 0);
    printf("%d\n", FLOOR_CENTER_SIZE);
    glutMainLoop();
}
