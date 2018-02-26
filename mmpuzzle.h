///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 4
/// DATE: 2/22/18
/// DESC: Header file for "Mechanical Matching Puzzle" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
using namespace std;

#ifndef MMPUZZLE_H
#define MMPUZZLE_H

//This class implements the "Mechanical Matching" Puzzle, a connect-three style game. This class provides support
//  for all the basic puzzle operations, such as matching, swapping, and replacing parts. It also provides additional
//  boolean functions for detecting the presence of matches and determining the validity of swaps. It also can
//  determine a set of all valid moves from the current state, which is used to simplify ACTIONS(). This
//  implementation does not store past swaps or the maximum allowed scores; these are to be managed externally.
class MMPuzzle
{
private:
    int m_score;        //Stores number of parts removed ("matched")
    int m_width;        //Width of the grid
    int m_height;       //Height of the grid
    int m_pool;         //Height of the pool
    int m_num_types;    //Number of distinct part types
    bool m_bonus_rules; //Marks if bonus rules are being used
    vector< vector<bool> > m_rmv_flags; //Used to mark parts for removal
    vector< vector<bool> > m_shp_flags; //Used to determine bounds of a "shape"


    void drawDivider(); //Outputs dividing line to screen; length is based on m_width.
    void fall();        //Settle all floating parts to bottom of board, replacing parts as necessary.
    void swap(int x1, int y1, int x2, int y2);             //Attempts to perform specified swap. Utilizes checkSwap().
    bool checkSwap(int x1, int y1, int x2, int y2);        //Returns true if swap is valid, false otherwise.
    void activateConsoles(int x1, int y1, int x2, int y2); //Trigger special console effects
    void nuke(int x, int y);         //Remove all parts within a 2-tile manhattan radius of the specified coordinate
    void purge(int part_type);       //Remove all instances of part_type outside the pool
    void processShape(int x, int y); //Process and remove a match from the board, while following "shape rules"
    void crawlShape(int x, int y);   //Crawl across board, setting m_shp_flags to group shape locations together
    char typeShape(int x, int y);    //Returns T/L/B(ar)/N(one) based on what shapes were found at (x,y).

public:
    vector< vector<int> > m_board;      //Stores entire puzzle grid (pool region + game-space)

    MMPuzzle();                                                             //Default Constructor (Minimal Gameboard)
    MMPuzzle(int width, int height, int pool, int num_types, bool bonus);   //Explicit Constructor
    MMPuzzle(const MMPuzzle & rhs);                                         //Copy Constructor
    MMPuzzle & operator=(const MMPuzzle & rhs);                             //Operator Equals Overload
    void setBoard(const vector< vector<int> > & initial_setup);             //Set Grid (m_board) Values
    int getScore()const { return m_score; };                                //Accessor function for m_score

    vector<CoordPair> validMoves();             //Returns all valid swaps based on m_board.
    void draw();                                //Outputs current board state. Utilizes drawDivider().
    void match();                               //Execute matching process. Recursive. Utilizes fall() / checkMatch().
    bool checkMatch();                          //Returns true is match is possible, false otherwise.
    void swap(const CoordPair & targets);       //Simple wrapping function for swap(). Ensures valid input structure.
    bool checkSwap(const CoordPair & targets);  //Simple wrapping function for checkSwap().
};

#endif
