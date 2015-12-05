#ifndef MODEL_H
#define MODEL_H

#include "element.h"
#include "base.h"
#include <vector>
#include <GL/freeglut.h>

class Stone{
private:
    Status color;
    double x, y, z;
    double vx, vy, vz;
    double angle, vangle;
    double axisx, axisy, axisz;

public:
    Stone() : color(Black), x(0.0), y(0.0), z(0.0), vx(0.0), vy(0.0), vz(0.0), angle(0.0), vangle(0.0), axisx(0.0), axisy(0.0), axisz(1.0){};
    Stone(Piece p);
    Status getColor() { return color; }
    double getX() { return x; }
    double getY() { return y; }
    double getZ() { return z; }
    double getAngle() { return angle; }
    double getAxisx() { return axisx; }
    double getAxisy() { return axisy; }
    double getAxisz() { return axisz; }

    void fly();
    void move(double dt);
    void print();
};

#endif // MODEL_H
