#ifndef DISPLAY_H
#define DISPLAY_H

#define NDEBUG

#include <GL/freeglut.h>
#include <GL/glut.h>
#include "element.h"
#include "base.h"
#include "game.h"

const double M_PI = 3.14159265358979323846;
const double step = 2.0 / BOARD_SIZE;
const double hstep = 1.0 / BOARD_SIZE;
const int CIRCLE_MAX = 30;

extern double theta, fai;
extern int screenSize, screenWidth, screenHeight;

void initDisplay();
void drawCircle(double, double, double, Status);
void drawHint(double, double, double);
void drawBoard();
void drawPiece();
void drawStone();
void drawBackGround();
void display();
void reshape(int, int);
void displayThread(int, char**);

extern void mouseKey(int, int, int, int);
extern void mouseMotion(int, int);

#endif
