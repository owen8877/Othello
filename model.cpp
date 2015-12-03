#include "element.h"
#include "base.h"
#include "model.h"
#include <cmath>
#include <vector>

using namespace std;

vector<Stone> stones;
extern double xcenter = 0.0, ycenter = 0.0, zcenter = 0.0;

Stone::Stone(Piece p){
    double rm = RAND_MAX;
    color = p.getStatus();
    x = p.getY() - 4.5;
    y = 4.5 - p.getX();
    z = 0.0;
    vx = -0.1 * p.getY() + 30.0 * rand() / rm;
    vy = 30.0 * rand() / rm;
    vz = 10.0 * p.getY() + 50.0 + 50.0 * rand() / rm;
    angle = 0.0;
    vangle = rand() / rm;
    axisx = rand() / rm;
    axisy = rand() / rm;
    axisz = rand() / rm;
    //vx = -4.5 + 1.0 * p.getY();
    //vy = 1.0;
    //vz = 10.0 * p.getY() + 50.0;
}

void Stone::move(double dt){
    x += vx * dt;
    y += vy * dt;
    z += vz * dt;
    vz -= GRAVITY * dt;
    angle += vangle;
    if (z < 0) {
        z = -z;
        vz = -vz * RESTITUTION;
        vangle = -vangle * RESTITUTION;
    }
}

void Stone::print(){
    printf("coordinate : (%lf, %lf, %lf)\nvelocity : (%lf, %lf, %lf)\naxis : (%lf, %lf, %lf)\ncolor : %s\n",
        x, y, z, vx, vy, vz, axisx, axisy, axisz, (color == Black) ? "Black" : "White");
}

void initModel(){
    Board b;
    for (int i = 1; i <= BOARD_SIZE; ++i) {
        for (int j = 1; j <= BOARD_SIZE; ++j) {
            stones.push_back(Stone(b.getPiece(i, j)));
            stones.back().print();
        }
    }
}

// Timer Callback
void timerCallback(int index){
    switch (index) {
        case 0:
            glutTimerFunc(33, &timerCallback, 0);
            double x = 0.0, y = 0.0, z = 0.0;
            for (int i = 0; i < 40; ++i)
                for (vector<Stone>::iterator it = stones.begin(); it != stones.end(); ++it)
                    (*it).move(TIME_UNIT);
            for (vector<Stone>::iterator it = stones.begin(); it != stones.end(); ++it) {
                x += (*it).getX();
                y += (*it).getY();
                z += (*it).getZ();
            }
            xcenter = x / stones.size() * lambda + xcenter * (1 - lambda);
            ycenter = y / stones.size() * lambda + ycenter * (1 - lambda);
            zcenter = z / stones.size() * lambda + zcenter * (1 - lambda);

            printf("timefunc update complete\n");
            glutPostRedisplay();
            break;
    }
}
