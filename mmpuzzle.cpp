///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 4
/// DATE: 2/22/18
/// DESC: Function definition file for "Mechanical Matching Puzzle" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
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
    m_bonus_rules = false;
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
        m_shp_flags.push_back(temp2);
    }
}

//Explicit Constructor
MMPuzzle::MMPuzzle(int width, int height, int pool, int num_types, bool bonus)
{
    // ASSIGNMENT
    m_score = 0;
    m_width = width;
    m_height = height;
    m_pool = pool;
    m_num_types = num_types;
    m_bonus_rules = bonus;
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
        m_shp_flags.push_back(temp2);
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
    m_bonus_rules = rhs.m_bonus_rules;

    // GRID VALUE COPYING
    for(int i = 0; i < rhs.m_width; i++)
    {
        m_board.push_back(rhs.m_board[i]);
        m_rmv_flags.push_back(rhs.m_rmv_flags[i]);
        m_shp_flags.push_back(rhs.m_shp_flags[i]);
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
    m_bonus_rules = rhs.m_bonus_rules;

    // EMPTY PREVIOUS GRID
    for(int i = 0; i < old_width; i++)
    {
        m_board.pop_back();
        m_rmv_flags.pop_back();
        m_shp_flags.pop_back();
    }

    // COPY IN NEW GRID
    for(int i = 0; i < rhs.m_width; i++)
    {
        m_board.push_back(rhs.m_board[i]);
        m_rmv_flags.push_back(rhs.m_rmv_flags[i]);
        m_shp_flags.push_back(rhs.m_shp_flags[i]);
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
            if(temp > 0 && temp <= m_num_types) //If a valid part type is incoming, assign
            {
                m_board[j][i] = temp;
            }
            else if(temp > 5 && temp < 9 && m_bonus_rules) //If a console is incoming and m_bonus_rules = true
            {
                m_board[j][i] = temp;
            }
            else
            {
                cout << "Invalid part type, setting to empty space..." << endl; m_board[j][i] = 0;
            }
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
        if(m_bonus_rules) activateConsoles(x1, y1, x2, y2);
        //If bonus rules are enabled, attempt to activate any consoles that might have been swapped
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
    if (x1 + 1 == x2 || y1 + 1 == y2) { src_x = x1; src_y = y1; }
    else if(x1 - 1 == x2 || y1 - 1 == y2) { src_x = x2; src_y = y2; }
    else return false; //else tiles are not adjacent

    //Determine whether source location is swapping to the Right, or Downwards
    if (x1 != x2 && y1 == y2) { direction = 'R'; }
    else if (x1 == x2 && y1 != y2) { direction = 'D'; }
    else return false; //tiles are diagonal from each other, which is not a valid swap

    //Check for move validity within rules of game
    if  (  (src_x < 0 || src_y < 0 || src_x > m_width - 1 || src_y > m_height - 1) || (src_y < m_pool)
           || (direction == 'R' && x1 == m_width - 1) || (direction == 'D' && y1 == m_height - 1) )
    {
        //The above statement ensures that the swap operation occurs within the grid, but below the pool.
        return false; //swap goes out of bounds of game-space, invalid swap
    }

    //If the tiles are adjacent, bonus rules are enabled, and a console match occurs, return true
    if (m_board[x1][y1] > 5 && m_board[x1][y1] < 9 && m_bonus_rules) return true;
    if (m_board[x2][y2] > 5 && m_board[x2][y2] < 9 && m_bonus_rules) return true;
    //Otherwise, a swap must lead to a match to be valid

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

//Activate Consoles
void MMPuzzle::activateConsoles(int x1, int y1, int x2, int y2)
{
    if(!m_bonus_rules) return; //Consoles should not activate unless bonuses rules are enabled
    bool isConsoleL = (m_board[x1][y1] > 5 && m_board[x1][y1] < 9);
    bool isConsoleR = (m_board[x2][y2] > 5 && m_board[x2][y2] < 9);
    if(!isConsoleL && !isConsoleR) return; //If no consoles were given, console activation is complete

    if(isConsoleL) m_rmv_flags[x1][y1] = true; //If first coordinate is a console, mark for removal
    if(isConsoleR) m_rmv_flags[x2][y2] = true; //If second coordinate is a console, mark for removal

    int console_operation; //Stores the type of console operation being performed
    int parameter;         //Stores input parameter to console
    if(!isConsoleR || (m_board[x1][y1] < m_board[x2][y2] && isConsoleL))
    {
        console_operation = m_board[x1][y1];
        parameter = m_board[x2][y2];
    }
    else
    {
        console_operation = m_board[x2][y2];
        parameter = m_board[x1][y1];
    }

    if(console_operation == 6)
    {
        nuke(x1, y1);
        nuke(x2, y2);
    }
    else if(console_operation == 7)
    {
        if(parameter < 1 || parameter > 5) purge(1);
        else purge(parameter);
    }
    else if(console_operation == 8)
    {
        if(parameter == 8) purge(2);
        else if(parameter == 5) purge(1);
        else purge(parameter + 1);
    }
    return;
}

//Console Effect 6
void MMPuzzle::nuke(int x, int y)
{
    int x_dist;
    int y_dist;
    for(int i = m_pool; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            x_dist = abs(j - x);
            y_dist = abs(i - y);
            if(x_dist + y_dist <= 2) //If target is within a manhattan distance of 2 from the target
            {
                m_rmv_flags[j][i] = true;  //Mark part for removal
            }
        }
    }
    return;
}

//Console Effect 7/8
void MMPuzzle::purge(int part_type)
{
    for(int i = m_pool; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_board[j][i] == part_type) //If target type is detected
            {
                m_rmv_flags[j][i] = true;  //Mark part for removal
            }
        }
    }
    return;
}

//Shape Handling Function
void MMPuzzle::processShape(int x, int y)
{
    if(m_rmv_flags[x][y] == false) return; //Shape head must be scheduled for removal
    crawlShape(x, y);                      //Forms shape using m_shp_flags.

    //Determine Shape Type of Highest Priority
    char subshape_type;    //Can equal T (T-Shape), L (L-Shape), B (Bar-Shape), and N (No Shape).
    char best_shape = 'N'; //Stores shape of highest priority found in all subshapes
    for(int i = m_pool; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_shp_flags[j][i]) //For all potential subshape roots
            {
                subshape_type = typeShape(j, i); //Identify best shape rooted on (j, i);
                if(best_shape != 'T')
                {
                    if (subshape_type == 'T') best_shape = 'T';
                    else if(subshape_type == 'L' && best_shape != 'L') best_shape = 'L';
                    else if(subshape_type == 'B' && best_shape == 'N') best_shape = 'B';
                }
            }
        }
    }

    //Remove Shape
    int lowestX = x; //Stores console insertion x
    int lowestY = y; //Stores console insertion y
    for(int i = m_pool; i < m_height; i++)
    {
        for(int j = 0; j < m_width; j++)
        {
            if(m_shp_flags[j][i]) //If part in shape
            {
                m_board[j][i] = 0; //Remove part
                m_rmv_flags[j][i] = false;
                m_shp_flags[j][i] = false;
                m_score++;
                lowestX = j;
                lowestY = i;
            }
        }
    }

    //Insert Console, if special shape detected
    if (best_shape == 'T') m_board[lowestX][lowestY] = 8;
    else if (best_shape == 'L') m_board[lowestX][lowestY] = 7;
    else if (best_shape == 'B') m_board[lowestX][lowestY] = 6;
    else return;
}

//Crawling Function to explore Shapes
void MMPuzzle::crawlShape(int x, int y)
{
    if(m_pool > y) return; //cout << "I shouldn't be up here..." << y << endl;
    m_shp_flags[x][y] = true; //Add current cell to shape

    //Directional Booleans; simply check there at least as many tiles in specified direction
    bool UP_1 = y > m_pool;
    bool UP_2 = y > m_pool + 1;
    bool DN_1 = y < m_height - 1;
    bool DN_2 = y < m_height - 2;
    bool LT_1 = x > 0;
    bool LT_2 = x > 1;
    bool RT_1 = x < m_width - 1;
    bool RT_2 = x < m_width - 2;
    int shape_value = m_board[x][y];

    //Begin computing "Equivalence"; used to avoid frequently recomputing cell values
    vector<bool> equivalence;
    equivalence.push_back(UP_1); //  Diagram: Let X be the position (x,y)
    equivalence.push_back(UP_2); //
    equivalence.push_back(RT_1); //  - - 1 - -   equivalence[i] returns true
    equivalence.push_back(RT_2); //  - - 0 - -   if cell i (shown right) is
    equivalence.push_back(DN_1); //  7 6 X 2 3   of same part type as cell X.
    equivalence.push_back(DN_2); //  - - 4 - -
    equivalence.push_back(LT_1); //  - - 5 - -
    equivalence.push_back(LT_2); //
    if(equivalence[0]) equivalence[0] = (m_board[x][y-1] == shape_value);
    if(equivalence[1]) equivalence[1] = (m_board[x][y-2] == shape_value);
    if(equivalence[2]) equivalence[2] = (m_board[x+1][y] == shape_value);
    if(equivalence[3]) equivalence[3] = (m_board[x+2][y] == shape_value);
    if(equivalence[4]) equivalence[4] = (m_board[x][y+1] == shape_value);
    if(equivalence[5]) equivalence[5] = (m_board[x][y+2] == shape_value);
    if(equivalence[6]) equivalence[6] = (m_board[x-1][y] == shape_value);
    if(equivalence[7]) equivalence[7] = (m_board[x-2][y] == shape_value);

    //Begin crawling
    if(equivalence[0]) //Upwards
    {
        if(m_shp_flags[x][y-1] == false)
        {
            if(equivalence[1] || equivalence[4]) crawlShape(x,y-1);
        }
    }
    if(equivalence[2]) //Rightwards
    {
        if(m_shp_flags[x+1][y] == false)
        {
            if(equivalence[3] || equivalence[6]) crawlShape(x+1,y);
        }
    }
    if(equivalence[4]) //Downwards
    {
        if(m_shp_flags[x][y+1] == false)
        {
            if(equivalence[5] || equivalence[0]) crawlShape(x,y+1);
        }
    }
    if(equivalence[6]) //Leftwards
    {
        if(m_shp_flags[x-1][y] == false)
        {
            if(equivalence[7] || equivalence[2]) crawlShape(x-1,y);
        }
    }
    return;
}

//Type Identifying Function for Shapes
char MMPuzzle::typeShape(int x, int y)
{
    //This function's return can equal T (T-Shape), L (L-Shape), B (Bar-Shape), and N (No Shape).
    int x_length = 0;  //Stores length of root horizontal match
    int y_length = 0;  //Stores length of root vertical match

    //Directional Booleans; simply check there at least as many tiles in specified direction
    bool UP_1 = y > m_pool;
    bool UP_2 = y > m_pool + 1;
    bool DN_1 = y < m_height - 1;
    bool DN_2 = y < m_height - 2;
    bool LT_1 = x > 0;
    bool RT_1 = x < m_width - 1;
    bool RT_2 = x < m_width - 2;

    //Compute lengths of matches sourced at root
    if(RT_2) //Compute horizontal match length
    {
        if(m_shp_flags[x+1][y] && m_shp_flags[x+2][y]) //If sequence of at least 3
        {
            for(int k = x; k < m_width; k++)
            {
                if(m_shp_flags[k][y]) x_length++;
                else break; //End matching sequence
            }
        }
    }
    if(DN_2) //Compute vertical match length
    {
        if(m_shp_flags[x][y+1] && m_shp_flags[x][y+2]) //If sequence of at least 3
        {
            for(int k = y; k < m_height; k++)
            {
                if(m_shp_flags[x][k]) y_length++;
                else break; //End matching sequence
            }
        }
    }

    //With this information, we can now begin testing for special shapes.

    //T-Shape Tests
    if(x_length > 0)
    {
        if(UP_1 && DN_1)
        {
            if(m_shp_flags[x+(x_length-1)][y+1] && m_shp_flags[x+(x_length-1)][y-1])
            {
                if(x+x_length == m_width) return 'T';
                else if(!m_shp_flags[x+x_length][y]) return 'T';
            }
        }
        if(DN_2)
        {
            for (int i = 1; i < x_length - 1; i++)
            {
                if(m_shp_flags[x+i][y+1] && m_shp_flags[x+i][y+2])
                {
                    if(!UP_1) return 'T';
                    else if(!m_shp_flags[x+i][y-1]) return 'T';
                }
            }
        }
    }
    if(y_length > 0)
    {
        if(LT_1 && RT_1)
        {
            if(m_shp_flags[x+1][y+(y_length-1)] && m_shp_flags[x-1][y+(y_length-1)])
            {
                if(y+y_length == m_height) return 'T';
                else if(!m_shp_flags[x][y+y_length]) return 'T';
            }
        }
        if(RT_2)
        {
            for (int i = 1; i < y_length - 1; i++)
            {
                if(m_shp_flags[x+1][y+i] && m_shp_flags[x+2][y+i])
                {
                    if(!LT_1) return 'T';
                    else if(!m_shp_flags[x-1][y+i]) return 'T';
                }
            }
        }
    }

    //L-Shape Tests
    if(x_length > 0)
    {
        if(DN_2)
        {
            if(m_shp_flags[x][y+1] && m_shp_flags[x][y+2]) return 'L';
            else if(m_shp_flags[x+(x_length-1)][y+1] && m_shp_flags[x+(x_length-1)][y+2]) return 'L';
        }
        if(UP_2)
        {
            if(m_shp_flags[x+(x_length-1)][y-1] && m_shp_flags[x+(x_length-1)][y-2]) return 'L';
        }
    }
    if(y_length > 0)
    {
        if(RT_2)
        {
            if(m_shp_flags[x+1][y+(y_length-1)] && m_shp_flags[x+2][y+(y_length-1)]) return 'L';
        }
    }

    //Bar-Shape Test
    if (x_length > 4) return 'B';
    if (y_length > 4) return 'B';

    return 'N'; //No special shapes were found
}


//Return all Valid Swaps
vector<CoordPair> MMPuzzle::validMoves()
{
    vector<CoordPair> possible_moves;
    CoordPair temp;
    for(int i = m_pool; i < m_height; i++) //Begin by finding possible horizontal swaps
    {
        for(int j = 0; j < m_width - 1; j++)
        {
            if(checkSwap(j, i, j+1, i))
            {
                temp.setCord(j, i, j+1, i); //Create coordinate pair representing swap
                possible_moves.push_back(temp); //Add to possible moves
            }
        }
    }
    for(int i = m_pool; i < m_height - 1; i++) //Find possible vertical swaps
    {
        for(int j = 0; j < m_width; j++)
        {
            if(checkSwap(j, i, j, i+1))
            {
                temp.setCord(j, i, j, i+1);
                possible_moves.push_back(temp);
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
            if(m_rmv_flags[j][i]) //If part marked for removal
            {
                if(m_bonus_rules)
                {
                    processShape(j, i);
                    match_count++;
                }
                else
                {
                    m_board[j][i] = 0; //Remove part
                    m_rmv_flags[j][i] = false;
                    match_count++; //Record removal
                }
            }
        }
    }

    //Part 3: Checking for new matches caused by falling parts
    if(match_count > 0)
    {
        if(!m_bonus_rules) m_score += match_count; //Update score
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