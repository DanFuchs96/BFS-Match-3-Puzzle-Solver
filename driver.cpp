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
    int a, b;
    char c;
    while(true)
    {
        cin >> a; cin >> b; cin >> c;
        NCC.exchange(a, b, c);
        NCC.match();
        if(c == 'z') break;
    }
    cout << "Current score:" << NCC.getScore() << endl;
    return 0;
}