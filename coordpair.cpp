///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/4/18
/// DESC: Function definition file for "Coordinate Pair" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "coordpair.h"
using namespace std;

//Default Constructor
CoordPair::CoordPair()
{
    m_x1 = 0;
    m_y1 = 0;
    m_x2 = 0;
    m_y2 = 0;
}

//Explicit Constructor
CoordPair::CoordPair(int x1, int y1, int x2, int y2)
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
}

//Copy Constructor
CoordPair::CoordPair(const CoordPair & rhs)
{
    m_x1 = rhs.m_x1;
    m_y1 = rhs.m_y1;
    m_x2 = rhs.m_x2;
    m_y2 = rhs.m_y2;
}

//Operator Equals Overload
CoordPair & CoordPair::operator=(const CoordPair & rhs)
{
    if(this == &rhs) return *this;
    m_x1 = rhs.m_x1;
    m_y1 = rhs.m_y1;
    m_x2 = rhs.m_x2;
    m_y2 = rhs.m_y2;
    return *this;
}

//Direct Assignment
void CoordPair::setCord(int x1, int y1, int x2, int y2)
{
    m_x1 = x1;
    m_y1 = y1;
    m_x2 = x2;
    m_y2 = y2;
    return;
}

//Extraction Output Overload; used to output coordinate pairs cleanly
ostream & operator<<(ostream & os, const CoordPair & rhs)
{
    os << "(" << rhs.m_x1 << "," << rhs.m_y1 << "),(" << rhs.m_x2 << "," << rhs.m_y2 << ")";
    return os;
}
