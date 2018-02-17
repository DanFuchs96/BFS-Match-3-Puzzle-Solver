///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Function definition file for "Greedy Heuristic Priority Queue" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "gebfgs_node.h"
#include "ghp_queue.h"
using namespace std;

void GHP_Queue::clear()
{
    if (q_memory_head != NULL)
    {
        GHPQ_Cell* temp = q_memory_head;
        while(temp != NULL)
        {
            temp = q_memory_head->m_next;
            q_memory_head->m_next = NULL;
            delete q_memory_head->m_node;
            delete q_memory_head;
            q_memory_head = temp;
        }
        q_memory_tail = NULL;
        q_front = NULL;
    }
}

void GHP_Queue::insert(GeBFGS_Node* & node)
{
    if (q_memory_head == NULL) //If memory is completely empty
    {
        q_front = new GHPQ_Cell();
        q_front->m_node = node;
        q_memory_head = q_front;
        return;
    }
    else
    {
        int heur_val = node->m_heuristic;
        GHPQ_Cell* cell;
        if (q_memory_tail == q_front)                     //If there is no live memory, front insert
        {
            cell = new GHPQ_Cell();
            cell->m_node = node;
            q_front->m_next = cell;
            q_front = cell;
        }
        else if (heur_val < q_front->m_node->m_heuristic) //If new minimum heuristic found, front insert
        {
            if (q_memory_tail == NULL)                    //If there is no dead memory
            {
                cell = new GHPQ_Cell();
                cell->m_node = node;
                cell->m_next = q_front;
                q_memory_head = cell;
            }
            else
            {
                cell = new GHPQ_Cell(q_memory_tail, node);
            }
            q_front = cell;
            return;
        }
        else //Begin recursive insertion procedure
        {
            bool lcv = true;
            GHPQ_Cell* target = q_front;
            while (target->m_next != NULL && lcv)
            {
                if (heur_val < target->m_next->m_node->m_heuristic) lcv = false;
                else target = target->m_next;
            }
            cell = new GHPQ_Cell(target, node);
            return;
        }
    }
}

void GHP_Queue::merge(GHP_Queue & incoming_queue)
{
    if(incoming_queue.isEmpty()) return;
    GeBFGS_Node* node;
    GHPQ_Cell* emptied_cell;
    if(isEmpty()) //If FRONTIER is empty, allow insert() to handle front insert
    {
        emptied_cell = incoming_queue.q_front;
        node = incoming_queue.pop();
        insert(node);
        delete emptied_cell;
    }
    else if (incoming_queue.q_front->m_node->m_heuristic < q_front->m_node->m_heuristic) //New minimum heuristic value
    {
        emptied_cell = incoming_queue.q_front;
        node = incoming_queue.pop();
        q_front = new GHPQ_Cell(q_memory_tail, node);
        delete emptied_cell;
    }
    GHPQ_Cell* target = q_front;
    bool lcv;
    int heur_val;
    while(!incoming_queue.isEmpty())
    {
        //The incoming queue is already sorted, so only back insertions will occur
        emptied_cell = incoming_queue.q_front;
        node = incoming_queue.pop();
        delete emptied_cell;
        lcv = true;
        heur_val = node->m_heuristic;
        while (target->m_next != NULL && lcv)
        {
            if (heur_val < target->m_next->m_node->m_heuristic) lcv = false;
            else target = target->m_next;
        }
        target = new GHPQ_Cell(target, node);
    }
    incoming_queue.q_memory_head = NULL;
    incoming_queue.q_memory_tail = NULL;
    incoming_queue.q_front = NULL;
}

GeBFGS_Node* GHP_Queue::pop()
{
    if (isEmpty()) { cout << "ERROR: Queue Empty" << endl; return NULL; }
    q_memory_tail = q_front;
    if (q_front->m_next != NULL) q_front = q_front->m_next;
    return q_memory_tail->m_node;
}