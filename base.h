#ifndef BASE_H
#define BASE_H

#define BLACK_SIDE true
#define WHITE_SIDE false

#define BOARD_SIZE 8

#define GRAVITY 500
#define lambda 0.2
#define RESTITUTION 0.8
#define STONE_RADIUS 0.5
#define STONE_HEIGHT 0.2
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

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>

using namespace std;

inline void clear() { system("clear"); }
//inline void pause() { system("read"); }
inline void sleep(int millsec) { usleep(1000*millsec); }

#endif // BASE_H
