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
    printf("Displaying Board\n");
    drawDivider();
    for(int i = 0; i < m_pool; i++)
    {
        printf("| ");
        for(int j = 0; j < m_width; j++)
        {
            printf("%i ", m_board[j][i]);
        }
        printf("|\n");
    }
    drawDivider();
    for(int i = m_pool; i < m_height; i++)
    {
        printf("| ");
        for(int j = 0; j < m_width; j++)
        {
            printf("%i ", m_board[j][i]);
        }
        printf("|\n");
    }
    drawDivider();
    return;
}

void MMPuzzle::drawDivider()
{
    printf("X");
    for(int i = 0; i < m_width; i++)
    {
        printf("--");
    }
    printf("-X\n");
    return;
}

void MMPuzzle::fall()
{
    int replace_count = 0; //Number of parts replaced during current cycle
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_board[j][i] == 0) //If empty space is detected
            {
                replace_count++; //Replacement process begins
                for(int k = i; k > 1; k--) //Cascade falling parts upwards
                {
                    m_board[j][k] = m_board[j][k-1]; //Simulate falling part
                }
                if(i > 0) m_board[j][1] = m_board[j][0]; //If at least one part falls, open up top slot
                m_board[j][0] = ((m_board[j][1] + j + replace_count) % m_num_types) + 1; //Generate new part
            }
        }
    }
    draw(); //DEBUG
    return;
}
