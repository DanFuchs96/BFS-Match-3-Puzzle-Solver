///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Function definition file for "Greedy Heuristic Priority Queue" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "grbefgs_node.h"
#include "ghp_queue.h"
using namespace std;

//Clearing Function
void GHP_Queue::clear()
{
    if (q_memory_head != NULL) //If any memory is present
    {
        GHPQ_Cell* temp = q_memory_head;  //Stores next address of next cell to be emptied
        while(temp != NULL)               //While elements remain
        {
            temp = q_memory_head->m_next; //Address of next element
            q_memory_head->m_next = NULL; //Disconnect current cell
            delete q_memory_head->m_node; //Delete stored node
            delete q_memory_head;         //Delete cell
            q_memory_head = temp;         //Update memory start
        }
        q_memory_tail = NULL;             //Rear partition is empty
        q_front = NULL;                   //Front partition is empty
    }
}

//Node Insertion Function
void GHP_Queue::insert(GrBeFGS_Node* & node)
{
    if (q_memory_head == NULL)     //If no memory is present
    {
        q_front = new GHPQ_Cell(); //Create head cell
        q_front->m_node = node;    //Mark as head of front partition
        q_memory_head = q_front;   //Mark as head of memory
        return;                    //Insert complete
    }
    else //Memory is present
    {
        int heur_val = node->m_heuristic; //Store heuristic value of incoming node
        GHPQ_Cell* cell;                  //Store new cell created to contain node
        if (q_memory_tail == q_front)     //If front partition is empty
        {
            cell = new GHPQ_Cell();       //Create new cell
            cell->m_node = node;          //Store node
            q_front->m_next = cell;       //Direct tail of back partition towards head of front partition
            q_front = cell;               //Update head of front partition
        }
        else if (heur_val < q_front->m_node->m_heuristic) //Else empty front partition;
        {                                                 //If heuristic value is less than head of front partition
            if (q_memory_tail == NULL)  //If rear partition is empty
            {
                cell = new GHPQ_Cell(); //Create new cell
                cell->m_node = node;    //Store node
                cell->m_next = q_front; //Push old head of front partition to second slot of front partition
                q_memory_head = cell;   //Mark as head of memory
            }
            else //Else rear partition is nonempty
            {
                cell = new GHPQ_Cell(q_memory_tail, node);
                //This uses the "Attachment Constructor". This creates a new cell, which is stored in "cell",
                //stores node in that cell, and also addends it as the tail cell of q_memory_tail, which is the
                //end of the rear partition. So, cell stores the head of the front partition.
            }
            q_front = cell; //Update new head of front partition
            return;         //Insert Complete
        }
        else //Else insertion is to occur at some non-head location in front partition
        {
            bool index_found = false;                      //Becomes true if intermediate insertion occurs
            GHPQ_Cell* target = q_front;                   //Stores currently evaluated node
            while (target->m_next != NULL && !index_found) //While intermediate insertion is possible
            {
                if (heur_val < target->m_next->m_node->m_heuristic) index_found = true;
                //If heuristic value of incoming node is less than next cell's heuristic, exit loop.
                else target = target->m_next;   //Else continue searching for insertion location.
            }
            cell = new GHPQ_Cell(target, node); //Create new cell with node data, addend to cell "target"
            return;                             //Insert Complete
        }
    }
}

//Boolean Search Function
bool GHP_Queue::contains(GrBeFGS_Node* & node)
{
    if(q_memory_head == NULL) return false; //Empty, cannot contain node
    GrBeFGS_Node* target_node;               //Stores node being compared
    for (GHPQ_Cell* target_cell = q_memory_head; target_cell != NULL; target_cell = target_cell->m_next)
    {                                                                   //For loop to iterate across memory
        target_node = target_cell->m_node;                              //Pull node being compared
        if(target_node->m_state.getScore() == node->m_state.getScore()) //If scores match, check boards
        {
            bool possible_grid_match = true;                            //Becomes false if different value found
            int width = (int)node->m_state.m_board.size();              //Width of boards
            int height = (int)node->m_state.m_board[0].size();          //Height of boards
            for(int i = 0; i < height && possible_grid_match; i++)      //Height iterator
            {
                for(int j = 0; j < width && possible_grid_match; j++)   //Width iterator
                {
                    if (node->m_state.m_board[j][i] != target_node->m_state.m_board[j][i])
                    {
                        possible_grid_match = false; //Board disparity, these two nodes are not equal
                    }
                }
            }
            if(possible_grid_match) return true; //If loop exits and no differences, equivalent node found
        }
    }
    return false; //If loop exits and node was not found, it is not stored in this queue
}

//Queue Merging Function
void GHP_Queue::merge(GHP_Queue &incoming_queue)
{
    if(incoming_queue.isEmpty()) return; //Empty
    GrBeFGS_Node* node;                   //Stores node being transferred to calling queue
    GHPQ_Cell* emptied_cell;             //Stores cell to be emptied

    // PERFORM FRONT INSERT (Only if calling queue empty, or new heuristic minimum)
    if(isEmpty())                              //If calling queue is empty
    {
        emptied_cell = incoming_queue.q_front; //Cell to be emptied is head of incoming queue's front partition
        node = incoming_queue.pop();           //Store node being transferred, pop cell from incoming queue
        insert(node);                          //Move node into calling queue, letting insert() manage pointers
        delete emptied_cell;                   //Delete popped cell
    }
    else if (incoming_queue.q_front->m_node->m_heuristic < q_front->m_node->m_heuristic) //New minimum heuristic value
    {
        emptied_cell = incoming_queue.q_front; //Cell to be emptied is head of incoming queue's front partition
        node = incoming_queue.pop();           //Store node being transferred, pop cell from incoming queue
        q_front = new GHPQ_Cell(q_memory_tail, node);
        //Front insert occurs. This command creates a new cell, stores node in it, addends it to the end of the
        //rear partition of the calling queue, and lastly sets it as the head of the front partition.
        delete emptied_cell;
    }

    // PERFORM SEQUENTIAL INSERTS (This portion takes advantage of both queues already being sorted)
    GHPQ_Cell* target = q_front;     //Stores current position in calling queue
    bool lcv;                        //Becomes false to indicate when to trigger insertion
    int heur_val;                    //Stores heuristic value of incoming node
    while(!incoming_queue.isEmpty()) //While not all nodes from incoming queue have been popped
    {
        emptied_cell = incoming_queue.q_front; //Store cell of node to be moved to calling queue
        node = incoming_queue.pop();           //Pop cell from incoming queue, store incoming node
        delete emptied_cell;                   //Delete popped cell from incoming queue
        lcv = true;                            //Mark loop control variable as true
        heur_val = node->m_heuristic;          //Get incoming node's heuristic value
        while (target->m_next != NULL && lcv)  //While end of calling queue has not been reached,
        {                                      //search for intermediate insertion locations
            if (heur_val < target->m_next->m_node->m_heuristic) lcv = false; //If insertion location found
            else target = target->m_next;                                    //Else advance down list
        }
        target = new GHPQ_Cell(target, node);  //Addend node to target cell in calling queue
    }
    incoming_queue.q_memory_head = NULL; //All elements from incoming queue have been popped and deleted
    incoming_queue.q_memory_tail = NULL; //Clear back partition of incoming queue
    incoming_queue.q_front = NULL;       //Clear front partition of incoming queue
}

//Popping Function
GrBeFGS_Node* GHP_Queue::pop()
{
    if (isEmpty()) { cerr << "ERROR: Queue Empty" << endl; return NULL; }
    q_memory_tail = q_front;                                //Add head of front partition to tail of rear partition
    if (q_front->m_next != NULL) q_front = q_front->m_next; //Update head of front partition
    return q_memory_tail->m_node;                           //Return transferred node
}