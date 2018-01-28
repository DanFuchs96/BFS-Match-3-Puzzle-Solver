#include <iostream>
#include <vector>
using namespace std;

#ifndef NCC_MMPUZZLE_H
#define NCC_MMPUZZLE_H

class MMPuzzle
{
private:
    vector< vector<int> > m_board; //Board to store parts
    int m_width; //Width of the board
    int m_height; //Height of the board
    int m_pool; //Height of the pool
    int m_num_types; //Number of distinct part types
    void drawDivider();

public:
    MMPuzzle(int width, int height, int pool, int num_types);
    void setBoard();
    void draw();
    void fall();
};

#endif
