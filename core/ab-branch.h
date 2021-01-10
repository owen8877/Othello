//
// Created by xdroid on 1/9/21.
//

#ifndef OTHELLO_AB_BRANCH_H
#define OTHELLO_AB_BRANCH_H

#include <vector>
#include <ostream>

using namespace std;

struct XY {
    int x;
    int y;

    friend ostream &operator<<(ostream &os, const XY &xy) {
        os << "{x=" << xy.x << ", y=" << xy.y << "}";
        return os;
    }
};

struct Record {
    std::vector<XY> request;
    std::vector<XY> response;

    friend ostream &operator<<(ostream &os, const Record &r) {
        os << "request [";
        for (auto const &xy : r.request) {
            os << xy << ',';
        }
        os << "]" << endl;
        os << "response [";
        for (auto const &xy : r.response) {
            os << xy << ',';
        }
        os << "]" << endl;
        return os;
    }
};


#endif //OTHELLO_AB_BRANCH_H
