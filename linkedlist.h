/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains functions definitions to allow for linked list operation
*/

#ifndef EXAMPLE_LINKEDLIST_H
#define EXAMPLE_LINKEDLIST_H
#include "dependencies.h"
typedef struct Node {
    AEDV data;
    struct Node *next;
}Node;

typedef struct EventNode {
    EVENT eventData;
    struct EventNode *nextEvent;
}EventNode;

typedef struct OrderNode {
    OrderData data;
    struct OrderNode *nextOrder;
}OrderNode;
void AddToListBeginning(Node** ListRoot, int locationX, int locationY, int destinationX, int destinationY, int identifierCode ); // Initialize AEDV at start of list
void SwapBetweenLists(Node** Origin, Node** Destination, int SwapEVIN); //swap AEDV between Active/Inactive || Inactive/Active
void MoveToListBeginning(Node** listRoot, Node* addNode);
Node* FindInList(Node* listRoot, int identifierCode);
void AddEvent(EventNode** root, EVENT Event);
void RemoveEvent(EventNode** root);
void AddOrderToList(OrderNode** root, OrderData Event);


typedef struct TileNode {
    struct TileNode *parent;
    struct TileNode *queuePrev;
    struct TileNode *visitedPrev;
    Cord coordinate;
}TileNode;

typedef struct InstructionNode {
    struct InstructionNode *child;
    Cord nextPosition;
}InstructionNode;

typedef struct queue {
    TileNode * front;
    TileNode * rear;
}queue;

void queueSetup(queue** m);

void enQueue(TileNode* new_tile, TileNode* parent, queue* q, int visited);

void deQueue(queue* q, int visited);

bool searchQueue(Cord loc, queue* q);

bool emptyList(queue* q, int visited);

TileNode* new_tile(Cord loc);







#endif //EXAMPLE_LINKEDLIST_H
