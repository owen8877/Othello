#ifndef BASE_H
#define BASE_H

#define BLACK_SIDE true
#define WHITE_SIDE false

#define BOARD_SIZE 8

#define GRAVITY 500
#define DEFAULT_ZOOM 11
#define MAX_ZOOM 20
#define MIN_ZOOM 7
#define lambda 0.2
#define RESTITUTION 0.8
#define STONE_INTERVAL 1.0
#define STONE_RADIUS 0.4
#define STONE_HEIGHT 0.2
#define TABLE_SIZE 10
#define TABLE_HEIGHT 3
#define TABLE_THICKNESS 0.7
#define FLOOR_SIZE 45
#define FLOOR_SHADE_SIZE 5
#define FLOOR_CENTER_SIZE (2 * FLOOR_SHADE_SIZE)
#define FLOOR_GRID_SIZE 0.25
#define FPS 30
#define TIME_UNIT (1.0 / (100 * FPS))
#define REFRESH_UNIT (1.0 / FPS)

#define PLAYER_HUMAN 0
#define PLAYER_AI 1

#define ON "On"
#define OFF "Off"

#define LEFT_MOUSE_BUTTON    0x0001
#define MIDDLE_MOUSE_BUTTON  0x0002
#define RIGHT_MOUSE_BUTTON   0x0004

#define GLUT_WHEEL_UP        3
#define GLUT_WHEEL_DOWN      4

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

using namespace std;

inline void clear() { system("clear"); }
inline void mypause() { system("read"); }
inline void msleep(int millsec) { usleep(1000*millsec); }

#endif // BASE_H
