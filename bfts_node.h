///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 1
/// DATE: 2/4/18
/// DESC: Header file for "Breadth First Tree Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "mmpuzzle.h"
using namespace std;

#ifndef BFTS_NODE_H
#define BFTS_NODE_H

//This class is used to represent each node of the BFTS Tree. They contain all information relevant to storing the
//  results of any particular swap. Note that while they contain a parent pointer, dynamic memory cleanup is handled
//  externally.
class BFTS_Node
{
private:
    int goal_score; //Score threshold to be reached

public:
    MMPuzzle STATE;        //Current instance of the puzzle grid
    CoordPair PREV_ACTION; //Action that parent performed to create this node
    BFTS_Node* PARENT;     //Pointer to parent node; NULL indicates node is root
    int PATH_COST;         //Number of swaps performed leading up to this state

    BFTS_Node(int max_score, MMPuzzle & initial_state); //Constructor
    BFTS_Node(const BFTS_Node & rhs);                   //Copy Constructor
    ~BFTS_Node() { PARENT = NULL; }                     //Destructor

    bool GOAL();                 //GOAL Function; returns true if score was reached, false otherwise.
    vector<CoordPair> ACTIONS(); //ACTIONS Function; returns set of all available valid swaps based on STATE.

    //DESCRIPTION: Child Constructor. This constructor copies an existing node, and then applies the specified action
    //  to that copy. This copy is denoted as a child of the passed node. The specified action becomes stored in
    //  PREV_ACTION, PARENT is set to the original node, and PATH_COST increments.
    //PRECONDITIONS: Action must be valid for parent's state. Can be verified with parent->STATE.checkSwap(action).
    //POSTCONDITIONS: New child node is created with passed node being identified as it's parent.
    BFTS_Node(BFTS_Node & parent, CoordPair & action);
};

#endif
