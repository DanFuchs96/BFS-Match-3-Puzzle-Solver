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
            if(m_board[j][i] == 0) printf("  ");
            else printf("%i ", m_board[j][i]);
        }
        printf("|\n");
    }
    drawDivider();
    for(int i = m_pool; i < m_height; i++)
    {
        printf("| ");
        for(int j = 0; j < m_width; j++)
        {
            if(m_board[j][i] == 0) printf("  ");
            else printf("%i ", m_board[j][i]);
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
    return;
}

void MMPuzzle::match()
{
    fall(); //Ensure no parts are floating
    printf("Now settling..\n"); draw(); //DEBUG
    int match_target = -1; //Stores target value for match
    int match_count = 0; //Number of matches found this cycle

    //Part 1: Checking for matches
    for(int i = m_pool; i < m_height; i++) //Note all matches occur below pool
    {
        for(int j = 0; j < m_width; j++)
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
                if(m_board[j][i+1] == match_target && m_board[j][i+2] == match_target) //If sequence of at least 3
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

    //Part 2: Removing Matches
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_rmv_flags[j][i]) //Part marked for removal
            {
                m_board[j][i] = 0; //Remove part
                m_rmv_flags[j][i] = false;
                match_count++; //Record removal
            }
        }
    }

    //Part 3: Checking for new matches caused by falling parts
    if(match_count > 0)
    {
        m_score += match_count; //Update score
        draw(); printf("Added %i points\n", match_count); //DEBUG
        match(); //Recursively continue to check for matches
    }
    return;
}

void MMPuzzle::swap(int x1, int y1, int x2, int y2)
{
    if(checkSwap(x1, y1, x2, y2))
    {
        int temp = m_board[x1][y1];
        m_board[x1][y1] = m_board[x2][y2];
        m_board[x2][y2] = temp;
    }
    else
    {
        cout << "Error, invalid swap requested." << endl;
    }
    return;
}

bool MMPuzzle::checkSwap(int x1, int y1, int x2, int y2)
{
    int src_x = -1; //Stores x closest to origin (0,0), used to simplify bound-checking
    int src_y = -1; //Stores y closest to origin
    char direction; //Indicates direction the tile closest to the origin is swapping

    if(x1 + 1 == x2 || y1 + 1 == y2) { src_x = x1; src_y = y1; }
    else if(x1 - 1 == x2 || y1 - 1 == y2) { src_x = x2; src_y = y2; }
    else return false; //else tiles are not adjacent

    if(x1 != x2 && y1 == y2) { direction = 'R'; }
    else if(x1 == x2 && y1 != y2) { direction = 'D'; }
    else return false; //else tiles are not adjacent

    //Check for move validity within rules of game
    if  (  (src_x < 0 || src_y < 0 || src_x > m_width - 1 || src_y > m_height - 1) || (src_y < m_pool)
           || (direction == 'R' && x1 == m_width - 1) || (direction == 'D' && y1 == m_height - 1) )
    {
        return false; //move is out of bounds
    }

    //Perform the swap
    int temp = m_board[x1][y1];
    m_board[x1][y1] = m_board[x2][y2];
    m_board[x2][y2] = temp;

    //Ensure a match was found
    bool isValid = checkMatch();

    //Revert the swap
    m_board[x2][y2] = m_board[x1][y1];
    m_board[x1][y1] = temp;

    return isValid;
}

bool MMPuzzle::checkMatch()
{
    for(int i = m_pool; i < m_height; i++) //All matches occur below pool
    {
        for(int j = 0; j < m_width; j++)
        {
            if(j + 2 < m_width) //Check for a horizontal match
            {
                if(m_board[j+1][i] == m_board[j][i] && m_board[j+2][i] == m_board[j][i])
                {
                    return true; //Match Found
                }
            }
            if(i + 2 < m_height) //Check for a vertical match
            {
                if(m_board[j][i+1] == m_board[j][i] && m_board[j][i+2] == m_board[j][i])
                {
                    return true; //Match Found
                }
            }
        }
    }
    return false; //If loops exits, no matches were found
}
