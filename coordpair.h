///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 4
/// DATE: 2/4/18
/// DESC: Header file for "Coordinate Pair" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
using namespace std;

#ifndef COORDPAIR_H
#define COORDPAIR_H

//This class represents a pair of coordinates. It is used to simplify puzzle swapping operations.
class CoordPair
{
public:
    int m_x1;
    int m_y1;
    int m_x2;
    int m_y2;
    CoordPair();                                  //Default Constructor
    CoordPair(int x1, int y1, int x2, int y2);    //Explicit Constructor
    CoordPair(const CoordPair & rhs);             //Copy Constructor
    CoordPair & operator=(const CoordPair & rhs); //Operator Equals Overload
    void setCord(int x1, int y1, int x2, int y2); //Direct Assignment
    friend ostream & operator<<(ostream & os, const CoordPair & rhs); //Stream Output
};

#endif
