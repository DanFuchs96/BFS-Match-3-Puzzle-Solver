///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 2
/// DATE: 2/7/18
/// DESC: Header file for "Depth Limited Tree Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "mmpuzzle.h"
using namespace std;

#ifndef DLTS_NODE_H
#define DLTS_NODE_H

//This class is used to represent each node of the DLTS Tree. They contain all information relevant to storing the
//  results of any particular swap. Dynamically created children are stored in the children vector, and cleanup is
//  handled by the destructor.
class DLTS_Node
{
private:
    int goal_score; //Score threshold to be reached

public:
    MMPuzzle m_state;               //Current instance of the puzzle grid
    CoordPair m_prev_action;        //Action that parent performed to create this node
    DLTS_Node* m_parent;            //Pointer to parent node; NULL indicates node is root
    int m_pathcost;                //Number of swaps performed leading up to this state
    vector<DLTS_Node*> m_children; //Stores pointers to children nodes

    DLTS_Node(int max_score, MMPuzzle & initial_state); //Constructor
    DLTS_Node(const DLTS_Node & rhs);                   //Copy Constructor
    ~DLTS_Node();                                       //Destructor

    bool GOAL();                 //GOAL Function; returns true if score was reached, false otherwise.
    vector<CoordPair> ACTIONS(); //ACTIONS Function; returns set of all available valid swaps based on m_state.

    //DESCRIPTION: Child Constructor. This constructor copies an existing node, and then applies the specified action
    //  to that copy. This copy is denoted as a child of the passed node. The specified action becomes stored in
    //  m_prev_action, m_parent is set to the original node, and m_pathcost increments. The parent inserts the child's
    //  address into the parent's children vector.
    //PRECONDITIONS: Action must be valid for parent's state. Can be verified with parent->m_state.checkSwap(action).
    //POSTCONDITIONS: New child node is created with passed node being identified as it's parent.
    DLTS_Node(DLTS_Node & parent, CoordPair & action);
};

#endif
