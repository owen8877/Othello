#ifndef ELEMENT_H
#define ELEMENT_H

#include "base.h"
#include <vector>

enum Status {Empty, BlackValid, WhiteValid, Valid, Black, Reserved_5, Reserved_6, Reserved_7, White=8, Reserved_9, Reserved_10, Reserved_11, Reserved_12, Reserved_13, Reserved_14, Reserved_15};

using namespace std;

class Piece {
private:
    short int x, y;
    Status status;

public:
    Piece(int _x, int _y, Status _status);

    int getX() { return x; }
    int getY() { return y; }
    Status getStatus() { return status; }

    void print();
};

class Board {
private:
    int blackcount, whitecount;
    int blackvalid, whitevalid;
    int size, vsize;
    vector<vector<Status> > pieces;
    vector<pair<unsigned long, unsigned long> > sequence;

    void refreshValid();
    void refreshCount();
    bool isValidCal(int x, int y, bool gameSide);
    unsigned long long getBlackLong();
    unsigned long long getWhiteLong();

public:
    Board();
    int getSize() { return size; }
    Piece getPiece(int x, int y);
    int setPiece(Piece p);
    int getBlackcount() { return blackcount; }
    int getWhitecount() { return whitecount; }
    int getBlackvalid() { return blackvalid; }
    int getWhitevalid() { return whitevalid; }

    int getCount(bool side) { return ((side==BLACK_SIDE) ? blackcount : whitecount); }
    int getValid(bool side) { return ((side==BLACK_SIDE) ? blackvalid : whitevalid); }

    bool isValid(int x, int y, bool gameSide);

    void refresh();
    int overturn(Piece p);

    bool full();
    void print();
};

Status getValidTag(bool side);
Status getSideTag(bool side);

#endif //ELEMENT_H
