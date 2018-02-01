#include <iostream>
using namespace std;

#ifndef NCC_COORDPAIR_H
#define NCC_COORDPAIR_H

class CoordPair
{
public:
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    CoordPair();
    CoordPair(int x1, int y1, int x2, int y2);
    friend ostream & operator<<(ostream & os, const CoordPair & rhs);
};

#endif //NCC_COORDPAIR_H
