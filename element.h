#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>

enum Status {Empty, Valid, Black, White};

using namespace std;

class piece {
private:
    short int x, y;
    Status status;

public:
    piece(int _x, int _y, Status _status);

    int getX() { return x; }
    int getY() { return y; }
    Status getStatus() { return status; }
};

class board {
private:
    int blackcount, whitecount;
    int size, vsize;
    vector<vector<Status> > pieces;

    void refreshValid();
    void refreshCount();

public:
    board();
    int getSize() { return size; }
    piece getPiece(int x, int y);
    int setPiece(piece p);
    int getBlackcount() { return blackcount; }
    int getWhitecount() { return whitecount; }

    bool isValid(int x, int y, bool gameSide);

    void refresh();
    int overturn(piece p);

    bool full();
    void print();
};

#endif //BOARD_H
