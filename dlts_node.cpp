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
    m_state = initial_state;
    m_prev_action = temp;
    m_parent = NULL;
    m_pathcost = 0;
    goal_score = max_score;
}

//Copy Constructor
DLTS_Node::DLTS_Node(const DLTS_Node & rhs)
{
    m_state = rhs.m_state;
    m_prev_action = rhs.m_prev_action;
    m_parent = rhs.m_parent;
    m_pathcost = rhs.m_pathcost;
    goal_score = rhs.goal_score;
    //The children vector is not copied over.
}

//Destructor
DLTS_Node::~DLTS_Node()
{
    for(int i = (int)m_children.size() - 1; i >= 0; i--)
    {
        delete m_children[i];
    }
}

//GOAL Function
bool DLTS_Node::GOAL()
{
    return (m_state.getScore() >= goal_score);
}

//ACTIONS Function
vector<CoordPair> DLTS_Node::ACTIONS()
{
    return m_state.validMoves();
}

//Child Constructor
DLTS_Node::DLTS_Node(DLTS_Node & parent, CoordPair & action)
{
    m_state = parent.m_state;
    m_state.swap(action);
    m_state.match();
    m_prev_action = action;
    m_parent = &parent;
    m_pathcost = parent.m_pathcost + 1;
    goal_score = parent.goal_score;
    parent.m_children.push_back(this);
}