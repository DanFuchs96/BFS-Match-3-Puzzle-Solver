///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 2
/// DATE: 2/7/18
/// DESC: Function definition file for "Depth Limited Tree Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "dlts_node.h"
using namespace std;

//Constructor
DLTS_Node::DLTS_Node(int max_score, MMPuzzle & initial_state)
{
    CoordPair temp(-1,-1,-1,-1);
    STATE = initial_state;
    PREV_ACTION = temp;
    PARENT = NULL;
    PATH_COST = 0;
    goal_score = max_score;
}

//Copy Constructor
DLTS_Node::DLTS_Node(const DLTS_Node & rhs)
{
    STATE = rhs.STATE;
    PREV_ACTION = rhs.PREV_ACTION;
    PARENT = rhs.PARENT;
    PATH_COST = rhs.PATH_COST;
    goal_score = rhs.goal_score;
    //The children vector is not copied over.
}

//Destructor
DLTS_Node::~DLTS_Node()
{
    for(int i = (int)children.size() - 1; i >= 0; i--)
    {
        delete children[i];
    }
}

//GOAL Function
bool DLTS_Node::GOAL()
{
    return (STATE.getScore() >= goal_score);
}

//ACTIONS Function
vector<CoordPair> DLTS_Node::ACTIONS()
{
    return STATE.validMoves();
}

//Child Constructor
DLTS_Node::DLTS_Node(DLTS_Node & parent, CoordPair & action)
{
    STATE = parent.STATE;
    STATE.swap(action);
    STATE.match();
    PREV_ACTION = action;
    PARENT = &parent;
    PATH_COST = parent.PATH_COST + 1;
    goal_score = parent.goal_score;
    parent.children.push_back(this);
}