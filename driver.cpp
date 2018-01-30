#include <iostream>
#include <fstream>
#include <string>
#include "mmpuzzle.h"
using namespace std;

int main(int argc, char* argv[])
{
    //Declarations
    ifstream fin;
    string filename = argv[1];
    int quota = -1;
    int max_swaps = -1;
    int part_types = -1;
    int board_width = -1;
    int board_height = -1;
    int pool_height = -1;
    int bonus_rule_id = -1;
    vector< vector<int> > initial_state;
    vector<int> row_store;

    //File Opening
    fin.open(filename.c_str());
    if(argc != 2) { cout << "Error, please supply a file." << endl; exit(1); }
    if(!fin) { cout << "Error, file not found." << endl; exit(1);}

    //File Reading
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

    //Puzzle Setup
    MMPuzzle NCC(board_width, board_height, pool_height, part_types);
    NCC.setBoard(initial_state);
    NCC.draw();

    //Ready Play-test
    NCC.match();
    cout << "Exchange: " << endl;
    int a, b, c, d(0);
    while(d != -1)
    {
        cin >> a; cin >> b; cin >> c; cin >> d;
        NCC.swap(a, b, c, d);
        NCC.match();
    }
    cout << "Current score:" << NCC.getScore() << endl;
    return 0;
}