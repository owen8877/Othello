#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/freeglut.h>
#include "element.h"
#include "base.h"
#include "game.h"

const double M_PI = 3.14159265358979323846;
//bool drawable = true;
const double step = 2.0 / BOARD_SIZE;
const double hstep = 1.0 / BOARD_SIZE;

void initDisplay();
void drawCircle(double, double, double, Status);
void drawHint(double, double, double);
void drawBoard();
void drawPiece();
void display();
void reshape(int, int);
void displayThread(int, char**);

extern void mouseKey(int, int, int, int);

#endif
