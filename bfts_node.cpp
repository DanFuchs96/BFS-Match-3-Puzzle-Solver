///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 1
/// DATE: 2/4/18
/// DESC: Function definition file for "Breadth First Tree Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "bfts_node.h"
using namespace std;

//Constructor
BFTS_Node::BFTS_Node(int max_score, MMPuzzle & initial_state)
{
    CoordPair temp(-1,-1,-1,-1);
    STATE = initial_state;
    PREV_ACTION = temp;
    PARENT = NULL;
    PATH_COST = 0;
    goal_score = max_score;
}

//Copy Constructor
BFTS_Node::BFTS_Node(const BFTS_Node & rhs)
{
    STATE = rhs.STATE;
    PREV_ACTION = rhs.PREV_ACTION;
    PARENT = rhs.PARENT;
    PATH_COST = rhs.PATH_COST;
    goal_score = rhs.goal_score;
}

//GOAL Function
bool BFTS_Node::GOAL()
{
    return (STATE.getScore() >= goal_score);
}

//ACTIONS Function
vector<CoordPair> BFTS_Node::ACTIONS()
{
    return STATE.validMoves();
}

//Child Constructor
BFTS_Node::BFTS_Node(BFTS_Node & parent, CoordPair & action)
{
    STATE = parent.STATE;
    STATE.swap(action);
    STATE.match();
    PREV_ACTION = action;
    PARENT = &parent;
    PATH_COST = parent.PATH_COST + 1;
    goal_score = parent.goal_score;
}