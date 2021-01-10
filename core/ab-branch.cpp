#include "ab-branch.h"

//#define NDEBUG
#define BLACK_SIDE true
#define WHITE_SIDE false

using namespace std;

int debug = 0;
int Piece2s = 4;
bool currBotColor;
bool sideFlag = BLACK_SIDE;
int maxvalue[64];
int maxindex = 0;
int placex = -1, placey = -1;

const int Empty = 0;
const int BlackValid = 1;
const int WhiteValid = 2;
const int Valid = 3;
const int Black = 4;
const int White = 8;
const int EVAL_START = -(1 << 30) + 1;

using namespace std;

class Piece2 {
private:
    short int x, y;
    int status;

public:
    Piece2(int _x, int _y, bool side);

    int getX() const { return x; }

    int getY() const { return y; }

    int getStatus() const { return status; }
};

class Board2 {
public:
    int blackcount, whitecount;
    int blackvalid, whitevalid;
    int count[500];
    int cindex;
    unsigned long long sequence[500];
    int index;
    unsigned long long black;
    unsigned long long white;
    unsigned long long blackv;
    unsigned long long whitev;

    void refreshValid();

    void refreshCount();

    bool isValidCal(int x, int y, bool gameSide);

    int getp(int x, int y) const;

    void setp(int x, int y, int s);

    Board2();

    int setPiece2(Piece2 p);

    int getBlackcount() const { return blackcount; }

    int getWhitecount() const { return whitecount; }

    int getValid(bool side) { return ((side == BLACK_SIDE) ? blackvalid : whitevalid); }

    bool isValid(int x, int y, bool gameSide);

    void refresh();

    int overturn(Piece2 p);

    void record();

    void undoOnce();
};

Board2 b;
const int dir[8][2] = {{0,  1},
                 {0,  -1},
                 {1,  0},
                 {-1, 0},
                 {1,  1},
                 {1,  -1},
                 {-1, 1},
                 {-1, -1}};

inline void switchSide2() {
    sideFlag = !sideFlag;
}

inline int getValidTag2(bool side) {
    return (side == BLACK_SIDE) ? BlackValid : WhiteValid;
}

inline int getSideTag2(bool side) {
    return (side == BLACK_SIDE) ? Black : White;
}

int rev(int s) {
    if (s == Black) return White;
    if (s == White) return Black;
    return Empty;
}

inline bool inRange2(int x, int y) {
    return (x >= 0) && (y >= 0) && (x < 8) && (y < 8);
}

void Board2::refreshValid() {
    blackvalid = 0;
    whitevalid = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (getp(i, j) > Valid) continue;
            setp(i, j, Empty);
            if (isValidCal(i, j, BLACK_SIDE)) {
                setp(i, j, (getp(i, j) | BlackValid));
                blackvalid++;
            }
            if (isValidCal(i, j, WHITE_SIDE)) {
                setp(i, j, (getp(i, j) | WhiteValid));
                whitevalid++;
            }
        }
    }
}

void Board2::refreshCount() {
    blackcount = 0;
    whitecount = 0;
    for (int i = 0; i < 64; ++i) {
        blackcount += (black >> i) & 1;
        whitecount += (white >> i) & 1;
    }
}

bool Board2::isValidCal(int x, int y, bool gameSide) {
    int r = (gameSide == BLACK_SIDE) ? White : Black;
    for (auto l : dir) {
        int dx = l[0], dy = l[1];
        if ((inRange2(x + dx, y + dy)) && (getp(x + dx, y + dy) == r)) {
            for (int m = 2; inRange2(x + m * dx, y + m * dy); ++m) {
                if (getp(x + m * dx, y + m * dy) == r) continue;
                if (getp(x + m * dx, y + m * dy) <= Valid) break;
                return true;
            }
        }
    }
    //Maybe this cell is really not valid
    return false;
}

Piece2::Piece2(int _x, int _y, bool side) {
    x = _x;
    y = _y;
    status = (side == BLACK_SIDE) ? Black : White;
}

int Board2::getp(int x, int y) const {
    int p = 63 - 8 * x - y;
    int b = (black >> p) & 1;
    int w = (white >> p) & 1;
    int bv = (blackv >> p) & 1;
    int wv = (whitev >> p) & 1;
    return ((w << 3) + (b << 2) + (wv << 1) + bv);
}

void Board2::setp(int x, int y, int s) {
    int p = (63 - 8 * x - y);
    unsigned long long m = 1;
    m = m << p;
    (s & 1) ? (blackv |= m) : (blackv &= ~m);
    ((s >> 1) & 1) ? (whitev |= m) : (whitev &= ~m);
    ((s >> 2) & 1) ? (black |= m) : (black &= ~m);
    ((s >> 3) & 1) ? (white |= m) : (white &= ~m);
}

Board2::Board2() {
    black = 0x0000000810000000;
    white = 0x0000001008000000;
    blackv = 0x0;
    whitev = 0x0;
    index = 0;
    cindex = 0;
    refresh();
}

int Board2::setPiece2(Piece2 p) {
    //if (!inRange2(p.getX(), p.getY())) return 1;
    //if (!((getp(p.getX(),p.getY())&3) & getValidTag2(sideFlag))) return 2;
    record();
    setp(p.getX(), p.getY(), p.getStatus());
    overturn(p);
    refresh();
    switchSide2();
    return 0;
}

inline bool Board2::isValid(int x, int y, bool gameSide) {
    return (getp(x, y) & getValidTag2(gameSide));
}

inline void Board2::refresh() {
    refreshValid();
    refreshCount();
}

int Board2::overturn(Piece2 p) {
    int x = p.getX();
    int y = p.getY();
    int flipcolor = p.getStatus();
    int othercolor = rev(flipcolor);

    for (auto l : dir) {
        int dx = l[0], dy = l[1];
        if ((inRange2(x + dx, y + dy)) && (getp(x + dx, y + dy) == othercolor)) {
            bool flag = false;
            int m;
            for (m = 2; inRange2(x + m * dx, y + m * dy); ++m) {
                if (getp(x + m * dx, y + m * dy) == othercolor) continue;
                if (getp(x + m * dx, y + m * dy) <= Valid) break;
                flag = true;
                break;
            }
            if (flag) {
                for (int mm = 1; mm < m; ++mm) {
                    setp(x + mm * dx, y + mm * dy, flipcolor);
                }
            }
        }
    }
    return 0;
}

void Board2::record() {
    sequence[index++] = black;
    sequence[index++] = white;
    sequence[index++] = blackv;
    sequence[index++] = whitev;
    count[cindex++] = blackcount;
    count[cindex++] = whitecount;
    count[cindex++] = blackvalid;
    count[cindex++] = whitevalid;
}

void Board2::undoOnce() {
    whitev = sequence[--index];
    blackv = sequence[--index];
    white = sequence[--index];
    black = sequence[--index];
    whitevalid = count[--cindex];
    blackvalid = count[--cindex];
    whitecount = count[--cindex];
    blackcount = count[--cindex];
    switchSide2();
}

const int corner = 0x1 << 24;
const int subcorner = 0x1;
const int edge = 0x1 << 20;
const int edgecenter = 0x1 << 16;
const int subedge = 0x1 << 4;
const int center = 0;
const int subcenter = 0x1 << 10;


const int value[8][8] = {
        {corner,     subcorner,  edge,       edgecenter, edgecenter, edge,       subcorner,  corner},
        {subcorner,  subcorner,  edgecenter, subedge,    subedge,    edgecenter, subcorner,  subcorner},
        {edge,       edgecenter, subcenter,  subcenter,  subcenter,  subcenter,  edgecenter, edge},
        {edgecenter, subedge,    subcenter,  center,     center,     subcenter,  subedge,    edgecenter},
        {edgecenter, subedge,    subcenter,  center,     center,     subcenter,  subedge,    edgecenter},
        {edge,       edgecenter, subcenter,  subcenter,  subcenter,  subcenter,  edgecenter, edge},
        {subcorner,  subcorner,  edgecenter, subedge,    subedge,    edgecenter, subcorner,  subcorner},
        {corner,     subcorner,  edge,       edgecenter, edgecenter, edge,       subcorner,  corner}
};

const int seq[60][2] = {
        {0, 0},
        {0, 7},
        {7, 0},
        {7, 7},
        {0, 2},
        {0, 5},
        {2, 0},
        {2, 7},
        {5, 0},
        {5, 7},
        {7, 2},
        {7, 5},
        {0, 3},
        {0, 4},
        {1, 2},
        {1, 5},
        {2, 1},
        {2, 6},
        {3, 0},
        {3, 7},
        {4, 0},
        {4, 7},
        {5, 1},
        {5, 6},
        {6, 2},
        {6, 5},
        {7, 3},
        {7, 4},
        {2, 2},
        {2, 3},
        {2, 4},
        {2, 5},
        {3, 2},
        {3, 5},
        {4, 2},
        {4, 5},
        {5, 2},
        {5, 3},
        {5, 4},
        {5, 5},
        {1, 3},
        {1, 4},
        {3, 1},
        {3, 6},
        {4, 1},
        {4, 6},
        {6, 3},
        {6, 4},
        {0, 1},
        {0, 6},
        {1, 0},
        {1, 1},
        {1, 6},
        {1, 7},
        {6, 0},
        {6, 1},
        {6, 6},
        {6, 7},
        {7, 1},
        {7, 6}
};

int getEvaluation(bool thisside) {
    int sum = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (b.getp(i, j) == getSideTag2(thisside))
                sum += value[i][j];
        }
    }
    return sum;
}

int eval(bool thisside, int level, bool write) {
    if (level == 0) {
        debug++;
        return getEvaluation(thisside);
    }

    int max = EVAL_START;
    int x = -1, y = -1;
    int e;
    maxindex++;
    maxvalue[maxindex] = max;
    if (!b.getValid(thisside)) {
        debug = b.getValid(thisside);
        switchSide2();
        max = -(1 << 28);
        switchSide2();
    } else {
        int i, j;
        for (auto l : seq) {
            i = l[0];
            j = l[1];
            if (!b.isValid(i, j, thisside)) continue;
            b.setPiece2(Piece2(i, j, thisside));

            if (!b.getValid(!thisside)) e = 1 << 28;
            else {
                //int diff = (b.getValid(thisside) - b.getValid(!thisside)) << 15;
                int diff = (-b.getValid(!thisside)) << 15;
                //e = getEvaluation(thisside) - eval(!thisside, level-1, false) + diff;
                e = -eval(!thisside, level - 1, false) + diff;
            }

            b.undoOnce();

            if (e > -(maxvalue[maxindex - 1])) {
                maxindex--;
                return e;
            }
            if (e >= max) {
                max = e;
                maxvalue[maxindex] = max;
                x = i;
                y = j;
            }
        }
    }
    if (write) {
        placex = x;
        placey = y;
    }
    maxindex--;
    return max;
}

void getPiece2() {
    int level = 5;
    if (Piece2s < 10) level = 5;
    else if (Piece2s > 54) level = 64 - Piece2s;
    maxvalue[0] = EVAL_START;
    eval(currBotColor, level, true);
}

Piece2 getMove(bool isBlack, const Record &record) {
    int x, y;

    b = Board2();
    debug = 0;
    Piece2s = 4;
    sideFlag = BLACK_SIDE;
    for (int & i : maxvalue) {
        i = 0;
    }
    maxindex = 0;
    placex = -1, placey = -1;

    // rebuild
    int turnID = record.response.size();
    currBotColor = isBlack ? BLACK_SIDE : WHITE_SIDE;
    if (currBotColor == BLACK_SIDE) switchSide2();
    for (int i = 0; i < turnID; i++) {
        // rebuild status
        x = record.request[i].x - 1;
        y = record.request[i].y - 1;
        if (x >= 0) {
            b.setPiece2(Piece2(x, y, !currBotColor)); //counterpart
            Piece2s++;
        }
        switchSide2();

        x = record.response[i].x - 1;
        y = record.response[i].y - 1;
        if (x >= 0) {
            b.setPiece2(Piece2(x, y, currBotColor)); //self
            Piece2s++;
        }
        switchSide2();
    }

    // look at opponent input at this stage
    x = record.request[turnID].x - 1;
    y = record.request[turnID].y - 1;
    if (x >= 0) {
        b.setPiece2(Piece2(x, y, !currBotColor)); //counterpart
        Piece2s++;
    }
    switchSide2();

    getPiece2();
    return {placex+1, placey+1, false};
}
