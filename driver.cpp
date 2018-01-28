#include <iostream>
#include "mmpuzzle.h"
using namespace std;

int main()
{
    MMPuzzle NCC(3, 3, 1, 3);
    NCC.setBoard();
    NCC.draw();
    return 0;
}