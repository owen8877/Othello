#ifndef IO_H
#define IO_H

#include "base.h"
#include "element.h"

Piece getPieceFromConsole(bool side);

int menu();
int settings();
int save();
void mouseKey(int button, int state, int x, int y);
void mouseMotion(int x, int y);
Piece getPieceFromMouse(bool side);

#endif // IO_H
