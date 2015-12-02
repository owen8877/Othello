#include "base.h"
#include "element.h"
#include "game.h"

int dir[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

string Pattern(Status s){
    switch (s) {
        case Black : return "●";
        case White : return "○";
        default :
            return ((getValidTag(Game::getSideFlag()) & s) ? "✻" : " ");
    }
}

Status getValidTag(bool side) {
    return (side == BLACK_SIDE) ? BlackValid : WhiteValid;
}

Status getSideTag(bool side) {
    return (side == BLACK_SIDE) ? Black : White;
}

//handy tools
Status rev(Status s){
    if (s == Black) return White;
    if (s == White) return Black;
    return Empty;
}

inline bool inRange(int x, int y){
    return (x>=1)&&(y>=1)&&(x<=BOARD_SIZE)&&(y<=BOARD_SIZE);
}

//Implements of private methods
void Board::refreshValid(){
    blackvalid = 0;
    whitevalid = 0;
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            if (pieces[i][j] > Valid) continue;
            pieces[i][j] = Empty;
            if (isValidCal(i, j, BLACK_SIDE)) {
                pieces[i][j] = (Status) (pieces[i][j] | BlackValid);
                blackvalid++;
            }
            if (isValidCal(i, j, WHITE_SIDE)) {
                pieces[i][j] = (Status) (pieces[i][j] | WhiteValid);
                whitevalid++;
            }
        }
    }
    return;
}

void Board::refreshCount(){
    blackcount = 0;
    whitecount = 0;
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            switch (pieces[i][j]) {
                case Black :
                    blackcount++;
                    break;
                case White :
                    whitecount++;
                    break;
                default :
                    break;
            }
        }
    }
}

bool Board::isValidCal(int x, int y, bool gameSide){
    Status r = (gameSide == BLACK_SIDE) ? White : Black;
    for (int l = 0; l < 8; ++l) {
        int dx = dir[l][0], dy = dir[l][1];
        if (pieces[x+dx][y+dy] == r) {
            for (int m = 2; inRange(x+m*dx, y+m*dy); ++m) {
                if (pieces[x+m*dx][y+m*dy] == r) continue;
                if (pieces[x+m*dx][y+m*dy] <= Valid) break;
                return true;
            }
        }
    }
    //Maybe this cell is really not valid
    return false;
}

unsigned long long Board::getBlackLong(){
    unsigned long long temp = 0;
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            temp = (temp << 1) | ((pieces[i][j] >> 2) & 1);
        }
    }
    return temp;
}

unsigned long long Board::getWhiteLong(){
    unsigned long long temp = 0;
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            temp = (temp << 1) | ((pieces[i][j] >> 3) & 1);
        }
    }
    return temp;
}

//Implements of puclic methods
Piece::Piece(int _x, int _y, Status _status) {
    x = _x;
    y = _y;
    status = _status;
}

void Piece::print(){
    printf("x is %d, y is %d, Status is %d\n", x, y, status);
}

Board::Board(){
    vsize = BOARD_SIZE + 2;
    size = BOARD_SIZE;
    vector<Status> temp(vsize, Empty);
    vector<vector<Status> > tempp(vsize, temp);
    pieces = tempp;
    pieces[4][4] = White;
    pieces[5][5] = White;
    pieces[4][5] = Black;
    pieces[5][4] = Black;
    refresh();
}

Piece Board::getPiece(int x, int y) {
    return Piece(x, y, pieces[x][y]);
}

//The member function returns 0 if it did set the piece
int Board::setPiece(Piece p) {
    if (!inRange(p.getX(), p.getY())) { printf("out!\n"); return 1; }
    if (!((pieces[p.getX()][p.getY()]&3) & getValidTag(Game::getSideFlag()))) { printf("not valid!\n"); return 2; }
    //First, let me record the board.
    sequence.push_back(make_pair(getBlackLong(), getWhiteLong()));
    //printf("%d %016llx %016llx\n", sequence.size(), sequence.back().first, sequence.back().second);
    //Ok, you can place the piece.
    pieces[p.getX()][p.getY()] = p.getStatus();
    overturn(p);
    refresh();
    return 0;
}

bool Board::isValid(int x, int y, bool gameSide) {
    //printf("now x(%d) y(%d) has Status %d.\n", x, y, pieces[x][y]);
    return (pieces[x][y] <= Valid) && (pieces[x][y] & getValidTag(gameSide));
}

void Board::refresh(){
    refreshValid();
    refreshCount();
}

int Board::overturn(Piece p){
    int x = p.getX();
    int y = p.getY();
    Status flipcolor = p.getStatus();
    Status othercolor = rev(flipcolor);

    for (int l = 0; l < 8; ++l) {
        int dx = dir[l][0], dy = dir[l][1];
        if (pieces[x+dx][y+dy] == othercolor) {
            bool flag = false;
            int m;
            for (m = 2; inRange(x+m*dx, y+m*dy); ++m) {
                if (pieces[x+m*dx][y+m*dy] == othercolor) continue;
                if (pieces[x+m*dx][y+m*dy] <= Valid) break;
                flag = true;
                break;
            }
            if (flag) {
                for (int mm = 1; mm < m; ++mm) {
                    //sequence.back().push_back(Piece(x+mm*dx, y+mm*dy, pieces[x+mm*dx][y+mm*dy] xor flipcolor));
                    pieces[x+mm*dx][y+mm*dy] = flipcolor;
                }
            }
        }
    }
    //Success!!!
    return 0;
}

bool Board::full() {
    return (blackcount + whitecount == size * size);
}

void Board::print() {
    clear();

    printf("┌");
    for (int i = 0; i < size; ++i) printf("───┬");
    printf("───┐\n");

    printf("│   ");
    for (int i = 0; i < size; ++i) printf("│ %c ", 'A'+i);
    printf("│\n");

    printf("├───");
    for (int i = 0; i < size; ++i) printf("┼───");
    printf("┤\n");

    for (int i = 1; i <= size; ++i) {
        printf("│ %d ", i);
        for (int j = 1; j <= size; ++j) {
            printf("│ %s ", Pattern(pieces[i][j]).c_str());
        }
        printf("│\n");
        if (i==size) break;
        printf("├───");
        for (int i = 0; i < size; ++i) printf("┼───");
        printf("┤\n");
    }
    printf("└───");
    for (int i = 0; i < size; ++i) printf("┴───");
    printf("┘\n");
    printf("\tBlack count : %d\n\tWhite count : %d\n", blackcount, whitecount);
    printf("\tBlack valid : %d\n\tWhite valid : %d\n", blackvalid, whitevalid);
}
