#include <iostream>
#include "mmpuzzle.h"
using namespace std;

int main()
{
    MMPuzzle NCC(4, 4, 1, 4);
    NCC.setBoard();
    NCC.draw();
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