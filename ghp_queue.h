///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 3
/// DATE: 2/15/18
/// DESC: Function definition file for "Greedy Heuristic Priority Queue" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "gebfgs_node.h"
using namespace std;

#ifndef GHP_QUEUE_H
#define GHP_QUEUE_H

class GHPQ_Cell
{
public:
    GeBFGS_Node* m_node;
    GHPQ_Cell* m_next;

    GHPQ_Cell(): m_node(NULL), m_next(NULL) {};     //Default Constructor
    ~GHPQ_Cell() { delete m_node; delete m_next; }; //Destructor

    //Insertion Constructor; automatically inserts to tail of existing cell
    GHPQ_Cell(GHPQ_Cell* & predecessor, GeBFGS_Node* & node)
    {
        m_next = predecessor->m_next;
        predecessor->m_next = this;
        m_node = node;
    }

};

class GHP_Queue
{
private:
    GHPQ_Cell* q_front;
    GHPQ_Cell* q_memory_head;
    GHPQ_Cell* q_memory_tail;

public:
    GHP_Queue(): q_front(NULL), q_memory_head(NULL), q_memory_tail(NULL) {};
    ~GHP_Queue() { clear(); };
    bool isEmpty() { return (q_front == NULL || q_front == q_memory_tail); }
    void clear();
    void insert(GeBFGS_Node* & node);
    //void merge(GHP_Queue & incoming_queue);
    GHPQ_Cell* pop();
};


#endif
