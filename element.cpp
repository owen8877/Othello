#include "base.h"
#include "element.h"
#include "game.h"

const char* Pattern[4] = {" ", "✻", "●", "○"};
int dir[8][2] = {{0,1}, {0,-1}, {1,0}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1}};

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
void board::refreshValid(){
    for (int i = 1; i <= size; ++i) {
        for (int j = 1; j <= size; ++j) {
            if (pieces[i][j] > Valid) continue;
            pieces[i][j] = (isValid(i, j, Game::getSideFlag()) ? Valid : Empty);
        }
    }
    return;
}

void board::refreshCount(){
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

//Implements of puclic methods
piece::piece(int _x, int _y, Status _status) {
    x = _x;
    y = _y;
    status = _status;
}

board::board(){
    blackcount = 2;
    whitecount = 2;
    vsize = BOARD_SIZE + 2;
    size = BOARD_SIZE;
    vector<Status> temp(vsize, Empty);
    vector<vector<Status> > tempp(vsize, temp);
    pieces = tempp;
    pieces[4][4] = White;
    pieces[5][5] = White;
    pieces[4][5] = Black;
    pieces[5][4] = Black;
    pieces[3][4] = Valid;
    pieces[4][3] = Valid;
    pieces[5][6] = Valid;
    pieces[6][5] = Valid;
}

piece board::getPiece(int x, int y) {
    return piece(x, y, pieces[x][y]);
}

//The member function returns 0 if it did set the piece
int board::setPiece(piece p) {
    if (!((p.getX() >= 1)&&(p.getX() <= size)&&(p.getY() >= 1)&&(p.getY() <= size))) return 1;
    if (pieces[p.getX()][p.getY()] != Valid) return 2;
    if (p.getStatus() == Black) blackcount++;
    if (p.getStatus() == White) whitecount++;
    pieces[p.getX()][p.getY()] = p.getStatus();
    overturn(p);
    refresh();
    return 0;
}

bool board::isValid(int x, int y, bool gameSide){
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

void board::refresh(){
    refreshValid();
    refreshCount();
}

int board::overturn(piece p){
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
                for (int mm = 1; mm < m; ++mm) pieces[x+mm*dx][y+mm*dy] = flipcolor;
            }
        }
    }
    //Success!!!
    return 0;
}

bool board::full() {
    return (blackcount + whitecount == size * size);
}

void board::print() {
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
            printf("│ %s ", Pattern[pieces[i][j]]);
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
}
