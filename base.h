#ifndef BASE_H
#define BASE_H

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <GLES3/gl3.h>

using namespace std;

const bool BLACK_SIDE = true;
const bool WHITE_SIDE = false;

const int BOARD_SIZE = 8;

const int GRAVITY = 500;
const int DEFAULT_ZOOM = 11;
const int MAX_ZOOM = 20;
const int MIN_ZOOM = 7;
const double lambda = 0.2;
const double RESTITUTION = 0.8;
const double STONE_INTERVAL = 1.0;
const double STONE_RADIUS = 0.4;
const double STONE_HEIGHT = 0.2;
const int TABLE_SIZE = 10;
const int TABLE_HEIGHT = 3;
const double TABLE_THICKNESS = 0.7;
const int FLOOR_SIZE = 45;
const int FLOOR_SHADE_SIZE = 5;
const int FLOOR_CENTER_SIZE = (2 * FLOOR_SHADE_SIZE);
const double FLOOR_GRID_SIZE = 0.25;
const int FPS = 60;
const double TIME_UNIT = (1.0 / (100 * FPS));
const double REFRESH_UNIT = (1.0 / FPS);

const int PLAYER_HUMAN = 0;
const int PLAYER_AI = 1;

const char ON [] = "On";
const char OFF [] = "Off";

const int LEFT_MOUSE_BUTTON    = 0x0001;
const int MIDDLE_MOUSE_BUTTON  = 0x0002;
const int RIGHT_MOUSE_BUTTON   = 0x0004;

const int GLUT_WHEEL_UP        = 3;
const int GLUT_WHEEL_DOWN      = 4;

inline void clear() { system("clear"); }
inline void mypause() { system("read"); }
inline void msleep(int millsec) { usleep(1000*millsec); }

#endif // BASE_H
