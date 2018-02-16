///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Function definition file for "Greedy Heuristic Priority Queue" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "coordpair.h"
#include "gebfgs_node.h"
#include "ghp_queue.h"
using namespace std;

void GHP_Queue::clear()
{
    if (q_memory_head != NULL)
    {
        delete q_memory_head;
        q_memory_head = NULL;
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
        if (heur_val < q_front->m_node->m_heuristic) //If new minimum heuristic found, front insert
        {
            if (q_memory_tail == NULL)               //If there is no dead memory
            {
                cell = new GHPQ_Cell();
                cell->m_node = node;
                cell->m_next = q_front;
                q_memory_head = cell;
            }
            else if (q_memory_tail == q_front)       //If there is no live memory
            {
                cell = new GHPQ_Cell();
                cell->m_node = node;
                q_front->m_next = cell;
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
            GHPQ_Cell* target = q_front;
            while (target->m_next != NULL)
            {
                if (heur_val < target->m_next->m_node->m_heuristic) break;
                else target = target->m_next;
            }
            cell = new GHPQ_Cell(target, node);
            return;
        }
    }
}

GHPQ_Cell* GHP_Queue::pop()
{
    if (isEmpty()) { cout << "ERROR: Queue Empty" << endl; return NULL; }
    q_memory_tail = q_front;
    q_front = q_front->m_next;
    return q_memory_tail;
}
