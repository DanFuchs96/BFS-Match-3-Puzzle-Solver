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
    CoordPair() { m_x1 = 0; m_y1 = 0; m_x2 = 0; m_y2 = 0; }
    CoordPair(int x1, int y1, int x2, int y2)
    {
        m_x1 = x1;
        m_y1 = y1;
        m_x2 = x2;
        m_y2 = y2;
    }
};

#endif //NCC_COORDPAIR_H
