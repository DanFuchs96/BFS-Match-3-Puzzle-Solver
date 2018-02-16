///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Main file. Implements GeBFGS Algorithm and applies it solve an instance of the Match-3
///       based "Mechanical Matching Puzzle". Takes a puzzle instance file as input, outputs the
///       file and a sequence of valid moves that lead to solution, or specifies no solution.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include "coordpair.h"
#include "mmpuzzle.h"
#include "gebfgs_node.h"
#include "ghp_queue.h"
using namespace std;


/// TABLE OF CONTENTS
/// 1) PROBLEM-ABSTRACTION STRUCTS
/// 2) GeBFGS ALGORITHM
/// 3) MAIN PROGRAM


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



///////////////////////////////////////////////
///GREEDY BEST FIRST GRAPH SEARCH ALGORITHM///
/////////////////////////////////////////////

//Solution GeBFGS_Algorithm(Problem & info);
void STATE(GHP_Queue & SHADOW)
{
    cout << "QUEUE POINT: " << SHADOW.q_front << " " << SHADOW.q_memory_tail << " " << SHADOW.q_memory_head << endl;
    GHPQ_Cell* reader = SHADOW.q_memory_head;
    if(reader == NULL)
    {
        cout << "Queue is unused." << endl;
    }
    else
    {
        bool hasReachedLive = (reader != SHADOW.q_memory_tail && reader == SHADOW.q_front);
        hasReachedLive = (hasReachedLive || SHADOW.q_memory_tail == NULL);
        cout << "QUEUE CELLS: ";
        while(reader != NULL)
        {
            if(hasReachedLive) cout << "L"; else cout << "D";
            if (reader == SHADOW.q_memory_tail && !hasReachedLive) hasReachedLive = true;
            reader = reader->m_next;
        }
        cout << endl;
    }
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
    GeBFGS_Node* x1 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x2 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x3 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x4 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x5 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x6 = new GeBFGS_Node(quota, puzzle_grid);
    GeBFGS_Node* x7 = new GeBFGS_Node(quota, puzzle_grid);
    GHP_Queue SHADOW; STATE(SHADOW);
    SHADOW.insert(x1); STATE(SHADOW);
    SHADOW.insert(x2); STATE(SHADOW);
    SHADOW.insert(x3); STATE(SHADOW);
    SHADOW.insert(x4); STATE(SHADOW);
    SHADOW.insert(x5); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.insert(x6); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.pop(); STATE(SHADOW);
    SHADOW.insert(x7); STATE(SHADOW);
    SHADOW.clear(); STATE(SHADOW);
/*
    // FORMULATE PROBLEM
    Problem scenario;
    scenario.puzzle = puzzle_grid;
    scenario.goal_score = quota;
    scenario.swap_limit = max_swaps;

    // EXECUTE ID-DFTS ALGORITHM
    Solution result = GeBFGS_Algorithm(scenario); //Executes ID-DFTS Algorithm, storing outcome

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
*/
    return 0;
}