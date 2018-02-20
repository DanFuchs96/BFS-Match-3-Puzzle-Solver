///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Header file for "Greedy Best First Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "mmpuzzle.h"
using namespace std;

#ifndef GRBEFGS_NODE_H
#define GRBEFGS_NODE_H

//This class is used to represent each node of the GeBFGS Tree. They contain all information relevant to storing the
//  results of any particular swap.
class GrBeFGS_Node
{
private:
    int m_goal_score; //Score threshold to be reached
    int m_swap_limit; //Maximum number of allowed swaps

public:
    MMPuzzle m_state;              //Current instance of the puzzle grid
    CoordPair m_prev_action;       //Action that parent performed to create this node
    GrBeFGS_Node* m_parent;         //Pointer to parent node; NULL indicates node is root
    int m_pathcost;                //Number of swaps performed leading up to this state
    int m_heuristic;               //Stores heuristic value

    GrBeFGS_Node(int max_score, int max_swaps, MMPuzzle & initial_state); //Constructor
    GrBeFGS_Node(const GrBeFGS_Node & rhs);                                //Copy Constructor

    int getHeuristic();          //Heuristic Function; returns the heuristic value of the current state
    bool GOAL();                 //GOAL Function; returns true if score was reached, false otherwise.
    vector<CoordPair> ACTIONS(); //ACTIONS Function; returns set of all available valid swaps based on m_state.

    //DESCRIPTION: Child Constructor. This constructor copies an existing node, and then applies the specified action
    //  to that copy. This copy is denoted as a child of the passed node. The specified action becomes stored in
    //  m_prev_action, m_parent is set to the original node, and m_pathcost increments. New heuristic calculated.
    //PRECONDITIONS: Action must be valid for parent's state. Can be verified with parent->m_state.checkSwap(action).
    //POSTCONDITIONS: New child node is created with passed node being identified as it's parent.
    GrBeFGS_Node(GrBeFGS_Node & parent, CoordPair & action);
};

#endif
