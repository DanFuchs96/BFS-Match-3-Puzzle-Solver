///////////////////////////////////////////////////////////////////////////////////////////////////
/// PROGRAMMER: DANIEL FUCHS
/// CLASS/SECT: CS5400A - ARTIFICIAL INTELLIGENCE
/// ASSIGNMENT: MATCH3 PUZZLE ASSIGNMENT: PART 4
/// DATE: 2/22/18
/// DESC: Function definition file for "Optimized Priority Queue" class.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include "coordpair.h"
#include "ags_node.h"
using namespace std;

#ifndef OP_QUEUE_H
#define OP_QUEUE_H

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// > CUSTOM PRIORITY QUEUE IMPLEMENTATION ///
/////////////////////////////////////////////
///The "OP Queue", or Optimized Priority Queue, is a custom-built priority queue designed to operate
///as fast possible specifically for this implementation of the Match-3 puzzle. A more formal title for it
///would be "Encapsulated Dual-Partitioned Triple-Indexed Ordered LinkedList".
///
/// > LinkedList Component Description:
/// - Encapsulation:   QHP Queue is the encapsulating class, GHPQ Cell is the represents link cells
/// - Partitions:      Two (Referred to as the "rear" and "front partitions")
/// - List Pointers:   Three (One to each partition's head, one to the rear partition's tail)
/// - Order:           Ordering by estimated cost, least to greatest
///
/// > Pointer State Descriptions:
/// - q_memory_head:  When null, this means the queue is completely empty; neither partition has cells.
///                   Otherwise, stores address of head of rear partition.
/// - q_memory_tail:  When null, the rear partition is empty.
///                   Otherwise, stores address of tail of rear partition.
/// - q_front:        When null OR equal to q_memory_tail, the front partition is empty.
///                   Otherwise, stores address of head of front partition.
///
///The rear partition of this queue is intended to serve as an "EXPLORED Queue", and only stores nodes
///that have been popped from the queue. The front partition is intended to serve as a "FRONTIER Queue",
///so all insertions are placed in the front partition. When a pop occurs, the tail pointer of the rear
///partition advances forward, moving the head of the front partition into the rear partition. The head
///pointer of the front partition also moves forward. (Note that the rear partition's tail is generally
///adjacent to the front partition's head.)
///
///These optimizations allow for insertions to be done very quickly for small queues. For large sequences
///of insertions, a merge function is provided which drains a source queue into the front partition of the
///target queue, while preserving order. This allows for large sequences of insertions to be done in linear
///time complexity. Pop operations also perform very quickly. The queue is also designed around operating
///with pointers to avoid needless reconstruction of stored objects.
///
///By using this class, runtime was greatly improved compared to the standard priority queue.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class OPQ_Cell //Component class of OP_Queue; used to store GeBFGS nodes
{
public:
    AGS_Node* m_node; //Node to be stored
    OPQ_Cell* m_next;   //Pointer to next cell

    OPQ_Cell(): m_node(NULL), m_next(NULL) {}; //Default Constructor
    ~OPQ_Cell() { };                           //Destructor; All deletion is handled externally by the GHP Queue

    //DESCRIPTION: Attachment Constructor. This takes an existing cell and a node to be inserted as parameters.
    //  This constructor creates a cell that stores the specified node, and inserts itself behind the specified
    //  cell. It marks the cell that originally followed it's predecessor as following it instead.
    //PRECONDITIONS: None.
    //POSTCONDITIONS: Predecessor "tail", or following cell, is now set to newly constructed cell. Newly constructed
    //  cell stores passed node, and marks the node that followed predecessor as following itself instead.
    OPQ_Cell(OPQ_Cell* & predecessor, AGS_Node* & node)
    {
        m_next = predecessor->m_next; //Mark predecessor's tail as own
        predecessor->m_next = this;   //Addend self as predecessor's tail
        m_node = node;                //Store passed node
    }

};

class OP_Queue //Greedy Heuristic Priority Queue
{
public:
    OPQ_Cell* q_front;       //Pointer to head of front partition
    OPQ_Cell* q_memory_head; //Pointer to head of rear partition
    OPQ_Cell* q_memory_tail; //Pointer to tail of rear partition

    OP_Queue(): q_front(NULL), q_memory_head(NULL), q_memory_tail(NULL) {};  //Default Constructor
    ~OP_Queue() { clear(); };                                                //Destructor, calls clear()
    bool isEmpty() { return (q_front == NULL || q_front == q_memory_tail); } //Returns true if empty; else false
    void clear();                         //Clearing Function; empties queue
    void insert(AGS_Node* & node);        //Insertion Function; inserts node into front partition
    bool contains(AGS_Node* & node);      //Boolean Search Function; Returns true if node found; false otherwise
    void merge(OP_Queue &incoming_queue); //Merging Function; Empties and sorts incoming queue into calling queue
    AGS_Node* pop();                      //Popping function; Pops a cell, adjusts partitions, returns stored node
};

#endif