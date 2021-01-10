#ifndef IO_H
#define IO_H

Piece getPieceFromConsole(bool side);

int menu();

int loadSettings();

int settings();

int save();

bool canReadRecord();

tuple<int, int, int> initReadRecord();

int readRecord();

void mouseKey(int button, int state, int x, int y);

void handleMouseMotion(int x, int y);

Piece getPieceFromMouse(bool side);

#endif // IO_H
