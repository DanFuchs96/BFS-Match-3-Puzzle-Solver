#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "mmpuzzle.h"
using namespace std;

MMPuzzle::MMPuzzle(int width, int height, int pool, int num_types)
{
    m_width = width;
    m_height = height;
    m_pool = pool;
    m_num_types = num_types;
    vector<int> temp;
    for(int i = 0; i < height; i++)
    {
        temp.push_back(0);
    }
    for(int i = 0; i < width; i++)
    {
        m_board.push_back(temp);
    }
}

void MMPuzzle::setBoard()
{
    int temp;
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            cin >> temp;
            if(temp > 0 && temp <= m_num_types) m_board[j][i] = temp;
            else { cout << "Invalid part type, setting to empty space..." << endl; m_board[j][i] = 0; }
        }
    }
    return;
}

void MMPuzzle::draw()
{
    printf("Displaying Board\n|");
    for(int i = 0; i < m_height; i++)
    {
        printf("|");
        for(int j = 0; j < m_width; j++)
        {
            printf(" %i |", m_board[j][i]);
        }
        printf("\n");
    }
}
