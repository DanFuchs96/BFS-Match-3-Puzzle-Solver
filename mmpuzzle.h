#include <iostream>
#include <vector>
#include "coordpair.h"
using namespace std;

#ifndef NCC_MMPUZZLE_H
#define NCC_MMPUZZLE_H

class MMPuzzle
{
private:
    vector< vector<int> > m_board; //Board to store parts
    vector< vector<bool> > m_rmv_flags; //Used to mark parts for removal
    int m_score;     //Stores number of parts removed ("matched")
    int m_width;     //Width of the board
    int m_height;    //Height of the board
    int m_pool;      //Height of the pool
    int m_num_types; //Number of distinct part types
    void drawDivider();
    void fall();
    bool checkSwap(const CoordPair & targets);
    bool checkSwap(int x1, int y1, int x2, int y2);

public:
    MMPuzzle(int width, int height, int pool, int num_types);
    MMPuzzle(const MMPuzzle & rhs);
    int getScore()const { return m_score; };
    void setBoard(const vector< vector<int> > & initial_setup);
    void draw();
    void match();
    void swap(int x1, int y1, int x2, int y2);
    void swap(const CoordPair & targets);

    bool checkMatch();
    vector<CoordPair> validMoves();
};

#endif
