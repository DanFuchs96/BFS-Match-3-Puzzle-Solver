#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include "mmpuzzle.h"
using namespace std;

MMPuzzle::MMPuzzle(int width, int height, int pool, int num_types)
{
    //Assignment
    m_score = 0;
    m_width = width;
    m_height = height;
    m_pool = pool;
    m_num_types = num_types;
    vector<int> temp1;
    vector<bool> temp2;

    //Error Correction
    if(m_width < 3) m_width = 3;
    if(m_height < 3) m_height = 3;
    if(m_pool < 0) m_pool = 0;
    if(m_pool > m_height) { cout << "Error, pool exceeds height. Setting to 1." << endl; m_pool = 1; }
    if(m_num_types < 2) m_num_types = 2;
    if(m_num_types > 5) m_num_types = 5;

    //Game-space Initialization
    for(int i = 0; i < height; i++)
    {
        temp1.push_back(0);
        temp2.push_back(false);
    }
    for(int i = 0; i < width; i++)
    {
        m_board.push_back(temp1);
        m_rmv_flags.push_back(temp2);
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

void MMPuzzle::match()
{
    int match_target = -1; //Stores target value for match

    //Part 1: Checking for matches
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(!m_rmv_flags[j][i]) //If current part has not been matched
            {
                match_target = m_board[j][i]; //Note target match value

                //Part 1.1: Checking for horizontal matches
                if(j + 2 < m_width) //If enough spaces remain for a potential match
                {
                    if(m_board[j+1][i] == match_target && m_board[j+2][i] == match_target) //If sequence of at least 3
                    {
                        for(int k = j; k < m_width; k++) //Begin marking parts in match for removal
                        {
                            if(m_board[k][i] == match_target) m_rmv_flags[k][i] = true; //Mark part for removal
                            else break; //End matching sequence
                        }
                    }
                }
                //Part 1.2: checking for vertical matches
                if(i + 2 < m_height) //If enough spaces remain for a potential match
                {
                    if(m_board[j][i+1] == match_target && m_board[j][i+1] == match_target) //If sequence of at least 3
                    {
                        for(int k = i; k < m_height; k++) //Begin marking parts in match for removal
                        {
                            if(m_board[j][k] == match_target) m_rmv_flags[j][k] = true; //Mark part for removal
                            else break; //End matching sequence
                        }
                    }
                }
                //Match checking complete, move to next part
            }
        }
    }

    //Part 2: Removing Matches
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_rmv_flags[j][i]) //Part marked for removal
            {
                m_board = 0; //Remove part
                m_rmv_flags[j][i] = false;
                m_score++; //Record removal
            }
        }
    }
    return;
}
