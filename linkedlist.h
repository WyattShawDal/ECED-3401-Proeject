/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains functions definitions to allow for linked list operation
*/
#include "dependencies.h"

#ifndef EXAMPLE_LINKEDLIST_H
#define EXAMPLE_LINKEDLIST_H

void queueSetup(queue** m);

void enQueue(TileNode* new_tile, TileNode* parent, queue* q, int visited);

void deQueue(queue* q, int visited);

bool searchQueue(Cord loc, queue* q);

bool emptyList(queue* q, int visited);

TileNode* new_tile(Cord loc);

void AddToListBeginning(AEDVNode** ListRoot, int locationX, int locationY,  int identifierCode ); // Initialize AEDV at start of list
void SwapBetweenLists(AEDVNode** Origin, AEDVNode** Destination, int SwapEVIN); //swap AEDV between Active/Inactive || Inactive/Active
void MoveToListBeginning(AEDVNode** listRoot, AEDVNode* addNode);
AEDVNode* FindInList(AEDVNode* listRoot, int identifierCode);
void AddEvent(EventNode** root, EVENT Event);
void RemoveEvent(EventNode** root);
void AddOrderToList(OrderNode** root, OrderData Event);


#endif //EXAMPLE_LINKEDLIST_H
