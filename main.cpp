///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Main file. Implements GrBeFGS Algorithm and applies it solve an instance of the Match-3
///       based "Mechanical Matching Puzzle". Takes a puzzle instance file as input, outputs the
///       file and a sequence of valid moves that lead to solution, or specifies no solution.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include "coordpair.h"
#include "mmpuzzle.h"
#include "grbefgs_node.h"
#include "ghp_queue.h"
using namespace std;


/// TABLE OF CONTENTS
/// 1) PROBLEM-ABSTRACTION STRUCTS
/// 2) GrBeFGS ALGORITHM
/// 3) MAIN PROGRAM


//////////////////////////////////
///PROBLEM-ABSTRACTION STRUCTS///
////////////////////////////////

struct Solution //Stores data relevant to tracking the solution
{
    bool success;                    //Stores if solution was found
    long double runtime;             //Stores runtime in seconds
    vector<CoordPair> move_sequence; //Action Sequence to possible goal state
};

struct Problem //Stores data relevant to representing the problem
{
    MMPuzzle puzzle; //Instance of the Mechanical Matching Puzzle
    int goal_score;  //Minimum score to attain
    int swap_limit;  //Maximum allowed moves to reach minimum score
};



///////////////////////////////////////////////
///GREEDY BEST FIRST GRAPH SEARCH ALGORITHM///
/////////////////////////////////////////////

Solution GrBeFGS_Algorithm(Problem & info)
{
    // SOLUTION SETUP
    Solution results;        //Stores results
    results.success = false; //Tracks if solution has yet been found
    clock_t t_time;          //Stores start and end time

    // TREE-STRUCTURE SETUP
    t_time = clock();      //Algorithm-relevant declarations begin, so timer starts
    GrBeFGS_Node* rootnode; //Store root node
    rootnode = new GrBeFGS_Node(info.goal_score, info.swap_limit, info.puzzle); //Create root node for tree structure

    // FRONTIER / EXPLORED / SUCCESSORS INITIALIZATION
    GHP_Queue FRONTIER;        //Priority Queue, stores pointers to frontier nodes, also contains EXPLORED partition
    GHP_Queue SUCCESSORS;      //Priority Queue, stores and sorts intermediate child nodes
    FRONTIER.insert(rootnode); //Add root, or the initial state, to the frontier
    GrBeFGS_Node* current_node; //Pointer used to track frontier node being evaluated
    GrBeFGS_Node* child_node;   //Pointer used to store newly-created children
    //The GHP_Queue is a special Priority Queue I designed to support an internal EXPLORED queue. In addition, it
    //designed specifically to operate with GrBeFGS, and has many additions that allow it to operate very quickly.
    //Please refer to "qhp_queue.h" for functionality info, and refer to (main.cpp:105) for an explanation as to
    //how using two of these queues simplifies exploration and speed up execution time.

    // ACTION-RELATED DECLARATIONS
    vector<CoordPair> action_list; //Stores list of available actions to a particular node
    int num_possible_moves = 0;    //Stores number of available actions to a particular node
    CoordPair selected_action;     //Stores action currently being evaluated


    // BEGIN GrBeFGS EXECUTION
    if(FRONTIER.q_front->m_node->GOAL()) results.success = true; //Check if root state is a goal state
    while(!results.success && !FRONTIER.isEmpty())               //While goal state not found and frontier is nonempty
    {
        current_node = FRONTIER.pop(); //Select node for exploration, add to EXPLORED
        if(current_node->GOAL()) //Check for goal state
        {
            results.success = true;
        }
        else if(current_node->m_pathcost < info.swap_limit) //If current node is within reachable number of swaps
        {
            action_list = current_node->ACTIONS();          //Execute ACTIONS(), store valid swaps
            num_possible_moves = (int)action_list.size();   //Store number of valid swaps
            for(int i = 0; i < num_possible_moves; i++)     //For each valid swap
            {
                selected_action = action_list[i];
                child_node = new GrBeFGS_Node(*current_node, selected_action); //Create new child amd execute swap
                if (SUCCESSORS.contains(child_node) || FRONTIER.contains(child_node)) delete child_node;
                else SUCCESSORS.insert(child_node); //If child not in EXPLORED, then store as a SUCCESSOR
                //Since EXPLORED is a partition stored in FRONTIER, checking for the child_node being in FRONTIER
                //ensures that the new child has not already been EXPLORED, nor has it been scheduled to be
                //explored. Likewise, checking SUCCESSORS ensures no duplicate states will be added.

                // > Proof of Exploration Functionality (main.cpp:105)
                //Now, it is worth noting that no state can ever be "improved". The reason for this twofold:
                //First, at any given iteration, all incoming children in SUCCESSORS will have the same path cost,
                //so no state can improve a "duplicate". So, SUCCESSORS will never add redundant states to FRONTIER.
                //Second, since the heuristic that guides expansion values lower path costs, any nodes that contain
                //the same board and current score but have different path costs will have their cheapest variant
                //explored first. Thus, suboptimal duplicates will appear in different iterations, but will never
                //be added since the cheapest variant was already marked as explored. Therefore, once a node reaches
                //the frontier, it cannot be improved; that is, there is no sequence of moves to that state such
                //that the path cost would be lower. This also means for that for checking node equality, we only
                //need to compare board states and their current score, since path-cost variance will either be zero,
                //or the incoming node will have a worse path-cost; in both cases that incoming node would be
                //thrown out.
            }
            FRONTIER.merge(SUCCESSORS); //Drain SUCCESSORS into FRONTIER
            //Note that this function empties SUCCESSORS. By sorting all children and inserting them at the same
            //time, we can greatly reduce the time complexity of inserting into FRONTIER. As the FRONTIER grows
            //larger, the effective time saved becomes greater.
        }
    }
    t_time = clock() - t_time;                                     //Execution Complete, mark finish time
    results.runtime = static_cast<float>(t_time) / CLOCKS_PER_SEC; //Compute runtime in seconds

    // STORE SOLUTION PATH
    if(results.success)
    {
        while(current_node->m_parent != NULL)
        {
            results.move_sequence.push_back(current_node->m_prev_action);
            current_node = current_node->m_parent;
        }
    }
    return results;
}



///////////////////
///MAIN PROGRAM///
/////////////////

int main(int argc, char* argv[]) //Expects filename to be passed as an argument
{
    // DECLARATIONS
    ifstream fin;
    string filename = argv[1];
    int quota = -1; //Score threshold
    int max_swaps = -1;
    int part_types = -1;
    int board_width = -1;
    int board_height = -1;
    int pool_height = -1;
    int bonus_rule_id = -1;
    vector< vector<int> > initial_state;
    vector<int> row_store;

    // OPEN INPUT FILE
    fin.open(filename.c_str());
    if(argc != 2) { cerr << "Error, please supply a file." << endl; exit(1); }
    if(!fin) { cerr << "Error, file not found." << endl; exit(1);}

    // READ FILE
    fin >> quota;
    fin >> max_swaps;
    fin >> part_types;
    fin >> board_width;
    fin >> board_height;
    fin >> pool_height;
    fin >> bonus_rule_id;
    for(int i = 0; i < board_width; i++) row_store.push_back(0);
    for(int i = 0; i < board_height; i++)
    {
        int temp;
        for(int j = 0; j < board_width; j++)
        {
            fin >> temp;
            row_store[j] = temp;
        }
        initial_state.push_back(row_store);
    }
    fin.close();

    // INITIALIZE PUZZLE
    MMPuzzle puzzle_grid(board_width, board_height, pool_height, part_types);
    puzzle_grid.setBoard(initial_state);

    // FORMULATE PROBLEM
    Problem scenario;
    scenario.puzzle = puzzle_grid;
    scenario.goal_score = quota;
    scenario.swap_limit = max_swaps;

    // EXECUTE GrBeFGS ALGORITHM
    Solution result = GrBeFGS_Algorithm(scenario); //Executes GrBeFGS Algorithm, storing outcome

    // OUTPUT FILE GENERATION
    cout << quota << endl;
    cout << max_swaps << endl;
    cout << part_types << endl;
    cout << board_width << endl;
    cout << board_height << endl;
    cout << pool_height << endl;
    cout << bonus_rule_id << endl;
    for(int i = 0; i < board_height; i++) //Output puzzle's initial state
    {
        for(int j = 0; j < board_width; j++)
        {
            cout << initial_state[i][j];
            if(j != board_width - 1) cout << " ";
            else cout << endl;
        }
    }
    if(result.success) //If solution was found, output sequence of moves
    {
        for(int i = (int)result.move_sequence.size() - 1; i >= 0; i--)
        {
            cout << result.move_sequence[i] << endl;
        }
    }
    else cout << "This puzzle has no solution." << endl;
    cout << result.runtime << endl; //Display execution time
    return 0;
}