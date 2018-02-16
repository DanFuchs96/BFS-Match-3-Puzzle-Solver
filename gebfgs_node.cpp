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
GeBFGS_Node::GeBFGS_Node(int max_score, int max_swaps, MMPuzzle & initial_state)
{
    CoordPair temp(-1,-1,-1,-1);
    m_state = initial_state;
    m_prev_action = temp;
    m_parent = NULL;
    m_pathcost = 0;
    m_heuristic = max_swaps;
    m_goal_score = max_score;
    m_swap_limit = max_swaps;
}

//Copy Constructor
GeBFGS_Node::GeBFGS_Node(const GeBFGS_Node & rhs)
{
    m_state = rhs.m_state;
    m_prev_action = rhs.m_prev_action;
    m_parent = rhs.m_parent;
    m_pathcost = rhs.m_pathcost;
    m_heuristic = rhs.m_heuristic;
    m_goal_score = rhs.m_goal_score;
    m_swap_limit = rhs.m_swap_limit;
}

//Heuristic Function
int GeBFGS_Node::getHeuristic()
{
    const int accuracy_offset = 7; //Fixed Offset; Increase to improve solution optimality
    return (((accuracy_offset + (m_goal_score/m_swap_limit))*m_pathcost) - m_state.getScore()) - m_pathcost;
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// > HEURISTIC DOCUMENTATION ///
    ////////////////////////////////
    ///This heuristic tracks compares the current node's anticipated score and actual attained scores.
    ///
    /// m_goal_score / m_swap_limit     => (ex_swap_val) : Expected points gained per swap
    /// ex_swap_val * m_pathcost        => (ant_score)   : Anticipated amount of points gained
    /// ant_score - m_state.getScore()  => (net_diff)    : The net difference of point gain
    ///
    /// - If (net_diff) is positive, that means less points than desired were attained.
    /// - If (net_diff) is negative, that means the node is doing better than expected,
    ///   with even more points than anticipated.
    /// - Thus, lower heuristic values equate to nodes that are approaching the desired
    ///   goal score the fastest.
    /// - The (accuracy_offset) can be increased arbitrarily to sacrifice execution time
    ///   for greater accuracy.
    /// - Additionally, (m_pathcost) is subtracted from this result for two reasons:
    ///    - 1) To help serve as a tiebreaker, between nodes of similar states.
    ///    - 2) To give preferential evaluation to nodes that are nearly complete.
    ///
    ///This heuristic value can be linearly transformed into an "expected number of swaps needed to reach goal".
    ///However, for the sake of improving runtime, this has been omitted, since it does not change the order
    ///in which nodes are sorted.
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

//GOAL Function
bool GeBFGS_Node::GOAL()
{
    return (m_state.getScore() >= m_goal_score);
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
    m_goal_score = parent.m_goal_score;
    m_swap_limit = parent.m_swap_limit;
    m_heuristic = getHeuristic();
}
