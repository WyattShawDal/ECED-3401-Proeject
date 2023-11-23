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
void AddToListBeginning(Node** ListRoot, int locationX, int locationY, int destinationX, int destinationY, int identifierCode ); // Initialize AEDV at start of list
void SwapBetweenLists(Node** Origin, Node** Destination, int SwapEVIN); //swap AEDV between Active/Inactive || Inactive/Active
void MoveToListBeginning(Node** listRoot, Node* addNode);
Node* FindInList(Node* listRoot, int identifierCode);

typedef struct TileNode {
    struct TileNode *parent;
    struct TileNode *queuePrev;
    struct TileNode *visitedNext;
    Cord coordinate;
}TileNode;

typedef struct queue {
    TileNode * front;
    TileNode * rear;
}queue;

void queueSetup(queue* m);

void enQueue(Cord loc, queue* q);

void deQueue(queue* q);

bool searchQueue(Cord loc, queue* q);

bool emptyList(queue* q);







#endif //EXAMPLE_LINKEDLIST_H
