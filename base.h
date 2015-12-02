#ifndef BASE_H
#define BASE_H

#define BLACK_SIDE true
#define WHITE_SIDE false

#define BOARD_SIZE 8

#define PLAYER_HUMAN 0
#define PLAYER_AI 1

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <strings.h>

using namespace std;

inline void clear() { system("clear"); }
inline void pause() { system("read"); }

#endif // BASE_H
