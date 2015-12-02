#ifndef BASE_H
#define BASE_H

#define BLACK_SIDE true
#define WHITE_SIDE false

#define BOARD_SIZE 8

#define PLAYER_HUMAN 0
#define PLAYER_AI 1

#define ON "On"
#define OFF "Off"

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
