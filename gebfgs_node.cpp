///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Function definition file for "Greedy Best First Search Node" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "gebfgs_node.h"
using namespace std;

//Constructor
GeBFGS_Node::GeBFGS_Node(int max_score, MMPuzzle & initial_state)
{
    CoordPair temp(-1,-1,-1,-1);
    m_state = initial_state;
    m_prev_action = temp;
    m_parent = NULL;
    m_pathcost = 0;
    m_heuristic = max_score;
    goal_score = max_score;
}

//Copy Constructor
GeBFGS_Node::GeBFGS_Node(const GeBFGS_Node & rhs)
{
    m_state = rhs.m_state;
    m_prev_action = rhs.m_prev_action;
    m_parent = rhs.m_parent;
    m_pathcost = rhs.m_pathcost;
    goal_score = rhs.goal_score;
}

//Heuristic Function
int GeBFGS_Node::getHeuristic()
{
    int remaining_score = goal_score - m_state.getScore();
    int remaining_moves = 17 - m_pathcost;
    int projected_score = remaining_moves*10;
    //return remaining_score;
    //return remaining_score*((m_pathcost+70));
    return remaining_score - projected_score;
    ///rem_score - (rem_moves * 10) did pretty well
}

//GOAL Function
bool GeBFGS_Node::GOAL()
{
    return (m_state.getScore() >= goal_score);
}

//ACTIONS Function
vector<CoordPair> GeBFGS_Node::ACTIONS()
{
    return m_state.validMoves();
}

//Child Constructor
GeBFGS_Node::GeBFGS_Node(GeBFGS_Node & parent, CoordPair & action)
{
    m_state = parent.m_state;
    m_state.swap(action);
    m_state.match();
    m_prev_action = action;
    m_parent = &parent;
    m_pathcost = parent.m_pathcost + 1;
    goal_score = parent.goal_score;
    m_heuristic = getHeuristic();
}
