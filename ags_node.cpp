///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 4
/// DATE: 2/22/18
/// DESC: Function definition file for "A* Graph Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "ags_node.h"
using namespace std;

//Constructor
AGS_Node::AGS_Node(int max_score, int max_swaps, MMPuzzle & initial_state)
{
    CoordPair temp(-1,-1,-1,-1);
    m_state = initial_state;
    m_prev_action = temp;
    m_parent = NULL;
    m_pathcost = 0;
    m_estimated_cost = max_swaps;
    m_goal_score = max_score;
    m_swap_limit = max_swaps;
}

//Copy Constructor
AGS_Node::AGS_Node(const AGS_Node & rhs)
{
    m_state = rhs.m_state;
    m_prev_action = rhs.m_prev_action;
    m_parent = rhs.m_parent;
    m_pathcost = rhs.m_pathcost;
    m_estimated_cost = rhs.m_estimated_cost;
    m_goal_score = rhs.m_goal_score;
    m_swap_limit = rhs.m_swap_limit;
}

//Heuristic Function
int AGS_Node::getHeuristic()
{
    const int accuracy_offset = 0; //Fixed Offset; Increase to improve solution optimality

    float ex_swap_val = m_goal_score / (float)m_swap_limit;
    int anticipated_score = ((accuracy_offset + ex_swap_val) * m_pathcost);
    int headway = ((m_state.getScore() - anticipated_score) / ex_swap_val);
    return ((m_swap_limit - headway))*(m_goal_score > m_state.getScore());
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// > HEURISTIC DOCUMENTATION ///
    ////////////////////////////////
    /// doot
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//GOAL Function
bool AGS_Node::GOAL()
{
    return (m_state.getScore() >= m_goal_score);
}

//ACTIONS Function
vector<CoordPair> AGS_Node::ACTIONS()
{
    return m_state.validMoves();
}

AGS_Node::AGS_Node(AGS_Node & parent, CoordPair & action)
{
    m_state = parent.m_state;
    m_state.swap(action);
    m_state.match();
    m_prev_action = action;
    m_parent = &parent;
    m_pathcost = parent.m_pathcost + 1;
    m_goal_score = parent.m_goal_score;
    m_swap_limit = parent.m_swap_limit;
    m_estimated_cost = m_pathcost + getHeuristic(); //f(n) = g(n) + h(n)
}
