#include <iostream>
#include "coordpair.h"
using namespace std;

CoordPair::CoordPair()
{
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
}

CoordPair::CoordPair(int x1, int y1, int x2, int y2)
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

ostream & operator<<(ostream & os, const CoordPair & rhs)
{
    os << "(" << rhs.m_x1 << "," << rhs.m_y1 << "),(" << rhs.m_x2 << "," << rhs.m_y2 << ")";
    return os;
}

