///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 2
/// DATE: 2/4/18
/// DESC: Function definition file for "Mechanical Matching Puzzle" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cstdio>
#include "coordpair.h"
#include "mmpuzzle.h"
using namespace std;

//Default Constructor
MMPuzzle::MMPuzzle() //This constructor creates a puzzle of absolute minimum size
{
    // ASSIGNMENT
    m_score = 0;
    m_width = 3;
    m_height = 4;
    m_pool = 1;
    m_num_types = 2;
    vector<int> temp1;
    vector<bool> temp2;

    // GRID INITIALIZATION
    for(int i = 0; i < m_height; i++)
    {
        temp1.push_back(0);
        temp2.push_back(false);
    }
    for(int i = 0; i < m_width; i++)
    {
        m_board.push_back(temp1);
        m_rmv_flags.push_back(temp2);
    }
}

//Explicit Constructor
MMPuzzle::MMPuzzle(int width, int height, int pool, int num_types)
{
    // ASSIGNMENT
    m_score = 0;
    m_width = width;
    m_height = height;
    m_pool = pool;
    m_num_types = num_types;
    vector<int> temp1;
    vector<bool> temp2;

    // ERROR CORRECTION
    if(m_width < 3) m_width = 3; //
    if(m_height < 4) m_height = 4; //Board size must be at least 3x3
    if(m_pool < 1) m_pool = 1;
    if(m_pool > m_height) { cout << "Error, pool exceeds height. Setting to 1." << endl; m_pool = 1; }
    if(m_num_types < 2) m_num_types = 2;
    if(m_num_types > 5) m_num_types = 5;

    // GRID INITIALIZATION
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

//Copy Constructor
MMPuzzle::MMPuzzle(const MMPuzzle & rhs)
{
    // ASSIGNMENT
    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_pool = rhs.m_pool;
    m_num_types = rhs.m_num_types;

    // GRID VALUE COPYING
    for(int i = 0; i < rhs.m_width; i++)
    {
        m_board.push_back(rhs.m_board[i]);
        m_rmv_flags.push_back(rhs.m_rmv_flags[i]);
    }
}

//Operator Equals Overload
MMPuzzle & MMPuzzle::operator=(const MMPuzzle & rhs)
{
    if(this == &rhs) return *this; //Identity Test
    int old_width = m_width;

    // ASSIGNMENT
    m_score = rhs.m_score;
    m_width = rhs.m_width;
    m_height = rhs.m_height;
    m_pool = rhs.m_pool;
    m_num_types = rhs.m_num_types;

    // EMPTY PREVIOUS GRID
    for(int i = 0; i < old_width; i++)
    {
        m_board.pop_back();
        m_rmv_flags.pop_back();
    }

    // COPY IN NEW GRID
    for(int i = 0; i < rhs.m_width; i++)
    {
        m_board.push_back(rhs.m_board[i]);
        m_rmv_flags.push_back(rhs.m_rmv_flags[i]);
    }
    return *this;
}

//Set Grid Values
void MMPuzzle::setBoard(const vector< vector<int> > & initial_setup)
{
    int temp;
    for(int i = 0; i < m_height; i++) //Copy all values of "initial_setup" into the grid
    {
        for(int j = 0; j < m_width; j++)
        {
            temp = initial_setup[i][j]; //Note the the values are transposed from the grid's coordinates
            if(temp > 0 && temp <= m_num_types) m_board[j][i] = temp;
            else { cout << "Invalid part type, setting to empty space..." << endl; m_board[j][i] = 0; }
            //When an invalid part is detected, it is set to empty space.
        }
    }
    match(); //Settle any floating parts and clear all preliminary matches
    return;
}

//Draw Dividing Line
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

//Part Replacement / Settling
void MMPuzzle::fall()
{
    int replace_count = 0; //Number of parts replaced during current cycle
    for(int i = 0; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_board[j][i] == 0) //If empty space is detected
            {
                replace_count++;           //Replacement process begins, increment number of parts replaced
                for(int k = i; k > 1; k--) //Cascade falling parts upwards
                {
                    m_board[j][k] = m_board[j][k-1]; //Shift part downwards
                }
                if(i > 0) m_board[j][1] = m_board[j][0]; //Complete falling cascade, opening up top row
                m_board[j][0] = ((m_board[j][1] + j + replace_count) % m_num_types) + 1; //Generate new part
            }
        }
    }
    return;
}

//Swapping Function
void MMPuzzle::swap(int x1, int y1, int x2, int y2)
{
    if(checkSwap(x1, y1, x2, y2)) //If swap is valid, then perform swap
    {
        int temp = m_board[x1][y1];
        m_board[x1][y1] = m_board[x2][y2];
        m_board[x2][y2] = temp;
    }
    else //Note that this version of swap is private, so invalid swaps here should never occur
    {
        cout << "Error, invalid swap requested." << endl;
    }
    return;
}

//Check for Valid Swaps
bool MMPuzzle::checkSwap(int x1, int y1, int x2, int y2)
{
    int src_x = -1; //Stores x closest to origin (0,0), used to simplify bound-checking
    int src_y = -1; //Stores y closest to origin
    char direction; //Indicates direction the tile closest to the origin is swapping

    //Determine which location is closer to the origin and set as "src"; also ensure one space between each location
    if(x1 + 1 == x2 || y1 + 1 == y2) { src_x = x1; src_y = y1; }
    else if(x1 - 1 == x2 || y1 - 1 == y2) { src_x = x2; src_y = y2; }
    else return false; //else tiles are not adjacent

    //Determine whether source location is swapping to the Right, or Downwards
    if(x1 != x2 && y1 == y2) { direction = 'R'; }
    else if(x1 == x2 && y1 != y2) { direction = 'D'; }
    else return false; //tiles are diagonal from each other, which is not a valid swap

    //Check for move validity within rules of game
    if  (  (src_x < 0 || src_y < 0 || src_x > m_width - 1 || src_y > m_height - 1) || (src_y < m_pool)
           || (direction == 'R' && x1 == m_width - 1) || (direction == 'D' && y1 == m_height - 1) )
    {
        //The above statement ensures that the swap operation occurs within the grid, but below the pool.
        return false; //swap goes out of bounds of game-space, invalid swap
    }

    //Perform the swap
    int temp = m_board[x1][y1];
    m_board[x1][y1] = m_board[x2][y2];
    m_board[x2][y2] = temp;

    //Check if a match now exists
    bool isValid = checkMatch();

    //Revert the swap
    m_board[x2][y2] = m_board[x1][y1];
    m_board[x1][y1] = temp;

    return isValid;
}

//Return all Valid Swaps
vector<CoordPair> MMPuzzle::validMoves()
{
    vector<CoordPair> possible_moves;
    CoordPair* temp;
    for(int i = m_pool; i < m_height; i++) //Begin by finding possible horizontal swaps
    {
        for(int j = 0; j < m_width - 1; j++)
        {
            if(checkSwap(j, i, j+1, i))
            {
                temp = new CoordPair(j, i, j+1, i); //Create coordinate pair representing swap
                possible_moves.push_back(*temp); //Add to possible moves
                delete temp;
                temp = NULL;
            }
        }
    }
    for(int i = m_pool; i < m_height - 1; i++) //Find possible vertical swaps
    {
        for(int j = 0; j < m_width; j++)
        {
            if(checkSwap(j, i, j, i+1))
            {
                temp = new CoordPair(j, i, j, i+1);
                possible_moves.push_back(*temp);
                delete temp;
                temp = NULL;
            }
        }
    }
    return possible_moves;
}

//Display Grid Values
void MMPuzzle::draw() //This function is mostly used for debugging; it is not used in the final code
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

//Matching Function
void MMPuzzle::match()
{
    fall(); //Ensure no parts are floating
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
        match(); //Recursively continue to check for matches
    }
    return;
}

//Check for a Match
bool MMPuzzle::checkMatch()
{
    for(int i = m_pool; i < m_height; i++) //All matches must occur below pool
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

//Wrapper for swap()
void MMPuzzle::swap(const CoordPair & targets)
{
    swap(targets.m_x1, targets.m_y1, targets.m_x2, targets.m_y2);
    return;
}

//Wrapper for checkSwap()
bool MMPuzzle::checkSwap(const CoordPair & targets)
{
    return checkSwap(targets.m_x1, targets.m_y1, targets.m_x2, targets.m_y2);
}