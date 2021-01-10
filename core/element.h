#ifndef ELEMENT_H
#define ELEMENT_H

#include "../base.h"
#include "ab-branch.h"
#include <fstream>
#include <vector>
#include <tuple>

enum Status {Empty, BlackValid, WhiteValid, Valid, Black, Undo, Menu, Settings, White=8, Save, Idle, Playing, Lifting, End, Pause, Recovery, Waiting};

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
    vector<pair<unsigned long long, unsigned long long> > sequence;

    void refreshValid();
    void refreshCount();
    bool isValidCal(int x, int y, bool gameSide);
    int buildFromPair(pair<unsigned long long, unsigned long long> p);

public:
    Record blackRecord, whiteRecord;

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
    vector<pair<unsigned long long, unsigned long long> > getSequence() { return sequence; }

    bool isValid(int x, int y, bool gameSide);

    void refresh();
    int overturn(Piece p);
    int record();
    int undo(unsigned int steps);

    unsigned long long getBlackLong();
    unsigned long long getWhiteLong();

    bool full();
    void print();

    tuple<int, int, int> recovery();
};

Status getValidTag(bool side);
Status getSideTag(bool side);

#endif //ELEMENT_H
