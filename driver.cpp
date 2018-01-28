#include <iostream>
#include "mmpuzzle.h"
using namespace std;

int main()
{
    MMPuzzle NCC(5, 5, 2, 3);
    NCC.setBoard();
    NCC.draw();
    NCC.match();
    cout << "Exchange: ";
    int input[4];
    for(int i = 0; i < 4; i++)
    {
        cin >> input[i];
    }
    NCC.exchange(input[0], input[1], input[2], input[3]);
    NCC.match();
    cout << "Current score:" << NCC.getScore() << endl;
    return 0;
}