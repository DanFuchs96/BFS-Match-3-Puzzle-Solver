///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 2
/// DATE: 2/7/18
/// DESC: Main file. Implements ID-DFTS Algorithm and applies it solve an instance of the Match-3
///       based "Mechanical Matching Puzzle". Takes a puzzle instance file as input, outputs the
///       file and a sequence of valid moves that lead to solution, or specifies no solution.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <sys/time.h>
#include "coordpair.h"
#include "mmpuzzle.h"
#include "bfts_node.h"
#include "dlts_node.h"
using namespace std;


/// TABLE OF CONTENTS
/// 1) PROBLEM-ABSTRACTION STRUCTS
/// 2) ID-DFTS ALGORITHM
///    a) DLTS_Recursive()
///    b) DLTS_Algorithm()
///    c) ID_DFTS_Algorithm()
/// 3) BFTS ALGORITHM
/// 4) MAIN PROGRAM


//////////////////////////////////
///PROBLEM-ABSTRACTION OBJECTS///
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

enum DLTS_Status { Success, Failure, Cutoff }; //Possible returns for DLTS

struct DLTSReturn //Stores data relevant to tracking the solution
{
    DLTS_Status status;                    //Stores if solution was found
    vector<CoordPair> move_sequence; //Action Sequence to possible goal state
};



///////////////////////////////////////////////////////
///ITERATIVE DEPTH-FIRST TREE SEARCH IMPLEMENTATION///
/////////////////////////////////////////////////////

///DESCRIPTION: This is the recursive component used in the DLTS Algorithm. It returns a DLTS_Status object after
///  evaluating the parent node given. It then recursively calls itself for all that parent's children.
///PRECONDITIONS: Limit must be non-negative. Max_swaps must be non-negative. Solution_node will be modified to store
///  the goal node, if found. Function must be called from within DLTS_Algorithm(). Tree root must be deleted
///  externally.
///POSTCONDITIONS: A DLTS_Status object is returned, which propagates up through recursive calls. If the returned
///  value is "Success", then solution_node will be set to equal the address of the relevant child node.

DLTS_Status DLTS_Recursive(DLTS_Node & parent, const int limit, const int max_swaps, DLTS_Node* & solution_node)
{
    if(parent.GOAL()) { solution_node = &parent; return Success; }  //Goal state found, store address
    else if(limit == 0) { return Cutoff; }                          //Limit was reached, cutoff has occurred
    else if(parent.PATH_COST >= max_swaps) { return Failure; }      //Swap limit reached, no possible children
    else
    {
        DLTS_Status status;
        DLTS_Node* child_node;
        CoordPair* selected_action;
        bool cutoff_occurred = false;
        vector<CoordPair> action_list = parent.ACTIONS();
        int num_possible_moves = (int)action_list.size();
        for(int i = 0; i < num_possible_moves; i++)
        {
            selected_action = &action_list[i];
            child_node = new DLTS_Node(parent, *selected_action); //Create new child amd execute swap
            status = DLTS_Recursive(*child_node, limit - 1, max_swaps, solution_node);
            if(status == Success) return Success;
            if(status == Cutoff) cutoff_occurred = true;
        }
        if(cutoff_occurred) return Cutoff; //Cutoff has occurred
        else return Failure; //Failure has occurred, no solution can exist
    }
}



///DESCRIPTION: This is the implementation of the DLTS Algorithm. It returns a "DLTSReturn" type, which contains the
///  solution information.
///PRECONDITIONS: None. Negatives values for depth still function as expected. Puzzle validity is ensured in the
///  MMPuzzle constructor. Intended for use within the ID-DFTS Algorithm.
///POSTCONDITIONS: A completed DLTSReturn object is returned. If status = Failure, vector move_sequence remains empty.
///  If status = Success, the sequence of swaps that lead to a goal state will be stored in reverse order. If
///  status = Cutoff, the solution cannot be found within the current depth limit.

DLTSReturn DLTS_Algorithm(Problem & info, int depth)
{
    // DECLARATIONS / TREE SETUP
    DLTSReturn results;
    DLTS_Node* root_node;                                    //Tracks tree root
    DLTS_Node* solution_node;                                //Tracks goal node
    root_node = new DLTS_Node(info.goal_score, info.puzzle); //Create tree root

    // INITIALIZE DLTS RECURSION
    if(info.swap_limit < 0) results.status = Failure;
    else if(info.swap_limit == 0)
    {
        if(root_node->GOAL()) results.status = Success;
        else results.status = Failure;
    }
    else results.status = DLTS_Recursive(*root_node, depth, info.swap_limit, solution_node);

    // READ SOLUTION
    if(results.status == Success)
    {
        DLTS_Node* current_node = solution_node;
        while(current_node->PARENT != NULL)
        {
            results.move_sequence.push_back(current_node->PREV_ACTION);
            current_node = current_node->PARENT;
        }
    }

    delete root_node; //Disassembles Tree
    return results;
}



///DESCRIPTION: This is the implementation of the ID-DFTS Algorithm. Instead of using a solution function, a Solution
///  object is created which stores the results of the Algorithm. This implementation also tracks it's runtime;
///  the period measured begins with the first execution-relevant declaration and ends when a goal node is found
///  and the solution path is determined. Alternatively, it ends when "no solution" is confirmed.
///PRECONDITIONS: None. Negatives values for goal_score and swap_limit still function as expected. Puzzle validity
///  is ensured in the MMPuzzle constructor.
///POSTCONDITIONS: A completed Solution object is returned. If success = false, vector move_sequence remains empty.
///  Otherwise, the sequence of swaps that lead to a goal state will be stored in reverse order.

Solution ID_DFTS_Algoritm(Problem & info)
{
    // SOLUTION SETUP
    Solution results;               //Stores results
    results.success = false;        //Tracks if solution has yet been found
    struct timeval t_start, t_end;  //Stores start and end time

    // INITIAL SETUP
    gettimeofday(&t_start, NULL);      //Algorithm begins, timer starts
    DLTSReturn iteration_results;      //Stores results of current iteration of DLTS
    iteration_results.status = Cutoff; //Signals to increase depth and continue iterating
    int depth = 0;                     //Tracks current allowed depth

    // BEGIN ITERATIVE DLTS
    while(iteration_results.status != Failure && !results.success)
    {
        iteration_results = DLTS_Algorithm(info, depth);
        if(iteration_results.status == Success)
        {
            results.success = true;
            results.move_sequence = iteration_results.move_sequence;
        }
        else depth++;
    }

    // EVALUATE RUNTIME
    gettimeofday(&t_end, NULL); //Execution Complete, mark finish time
    results.runtime = t_end.tv_usec - t_start.tv_usec;
    results.runtime /= 1000000.0;
    results.runtime += (t_end.tv_sec - t_start.tv_sec);

    return results;
}



//////////////////////////////////////////
///BREADTH-FIRST TREE SEARCH ALGORITHM///
////////////////////////////////////////

///DESCRIPTION: This is the implementation of the BFTS Algorithm. Rather than an using a solution function, a Solution
///  object is created which stores the results of the Algorithm. This implementation also tracks it's runtime;
///  the period measured begins with the first execution-relevant declaration and ends when a goal node is found.
///  Note that this implementation checks the root node explicitly for being a solution, but checks the rest of the
///  states for goal status right before placing them in the frontier.
///PRECONDITIONS: None. Negatives values for goal_score and swap_limit still function as expected. Puzzle validity
///  is ensured in the MMPuzzle constructor.
///POSTCONDITIONS: A completed Solution object is returned. If success = false, vector move_sequence remains empty.
///  Otherwise, the sequence of swaps that lead to a goal state will be stored in reverse order.

Solution BFTS_Algorithm(Problem & info)
{
    // SOLUTION SETUP
    Solution results;              //Stores results
    results.success = false;       //Tracks if solution has yet been found
    struct timeval t_start, t_end; //Stores start and end time

    // TREE-STRUCTURE SETUP
    gettimeofday(&t_start, NULL);                      //Algorithm-relevant declarations begin, so timer starts
    BFTS_Node root_node(info.goal_score, info.puzzle); //Create root node for tree structure
    vector<BFTS_Node*> NODE_MEMORY;                    //Tracks all children nodes; used in memory cleanup

    // FRONTIER INITIALIZATION
    deque<BFTS_Node*> FRONTIER;                 //FIFO Queue, stores pointers to all nodes in the frontier
    FRONTIER.push_back(&root_node);             //Add root, or the initial state, to the frontier
    BFTS_Node* current_node = FRONTIER.front(); //Pointer used to track frontier node being evaluated
    BFTS_Node* child_node;                      //Pointer used to store newly-created children

    // ACTION-RELATED DECLARATIONS
    vector<CoordPair> action_list; //Stores list of available actions to a particular node
    int num_possible_moves = 0;    //Stores number of available actions to a particular node
    CoordPair selected_action;     //Stores action currently being evaluated

    // BEGIN BFTS EXECUTION
    if(current_node->GOAL()) results.success = true;  //Check if root state is a goal state
    while(!results.success && !FRONTIER.empty())      //While goal state not found and frontier is nonempty
    {
        current_node = FRONTIER.front();
        FRONTIER.pop_front();
        if(current_node->PATH_COST < info.swap_limit)     //If current node is within reachable number of swaps
        {
            action_list = current_node->ACTIONS();        //Execute ACTIONS(), store valid swaps
            num_possible_moves = (int)action_list.size(); //Store number of valid swaps
            for(int i = 0; i < num_possible_moves; i++)   //For each valid swap
            {
                selected_action = action_list[i];
                child_node = new BFTS_Node(*current_node, selected_action); //Create new child amd execute swap
                if(child_node->GOAL())
                {
                    results.success = true;
                    current_node = child_node;
                    i = num_possible_moves; //Causes loop to exit after storing child node
                }
                FRONTIER.push_back(child_node);
                NODE_MEMORY.push_back(child_node);
            }
        }
    }
    gettimeofday(&t_end, NULL); //Execution Complete, mark finish time
    results.runtime = t_end.tv_usec - t_start.tv_usec;
    results.runtime /= 1000000.0;
    results.runtime += (t_end.tv_sec - t_start.tv_sec);

    // STORE SOLUTION PATH
    if(results.success)
    {
        while(current_node->PARENT != NULL)
        {
            results.move_sequence.push_back(current_node->PREV_ACTION);
            current_node = current_node->PARENT;
        }
    }

    // DEALLOCATE TREE / MEMORY CLEANUP
    for(int i = ((int)NODE_MEMORY.size()) - 1; i >= 0; i--)
    {
        delete NODE_MEMORY[i];
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
    if(argc != 2) { cout << "Error, please supply a file." << endl; exit(1); }
    if(!fin) { cout << "Error, file not found." << endl; exit(1);}

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

    // EXECUTE ALGORITHM
    Solution result = BFTS_Algorithm(scenario); //Executes Algorithm, storing outcome
    //Candidates for execution are: BFTS_Algorithm(), ID-DFTS_Algorithm()

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