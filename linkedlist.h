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

bool emptyList(queue** q, int visited);

TileNode* new_tile(Cord loc);

/**
 * @brief adds an AEDV to a list of choice
 * @param listRoot list to add AEDV to
 * @param locationX  x location to create AEDV at (stable location)
 * @param locationY y location to create AEDV at (stable location)
 * @param identifierCode unique EVIN for AEDV
 */
void AddAEDV(AEDVNode** listRoot, int locationX, int locationY, int identifierCode );
/**
 * @brief swaps AEDV between Active/Inactive
 * @param Origin
 * @param Destination
 * @param SwapEVIN
 */
void SwapBetweenLists(AEDVNode** Origin, AEDVNode** Destination, int SwapEVIN); //swap AEDV between Active/Inactive || Inactive/Active
//Needs explanation
void MoveToListBeginning(AEDVNode** listRoot, AEDVNode* addNode);
AEDVNode* FindInList(AEDVNode* listRoot, int identifierCode);
/**
 * @brief adds an event node to the end of the event lists
 * @param root
 * @param Event
 */
void AddEvent(EventNode** root, EventData Event);

//Needs Explanation
void RemoveEvent(EventNode** root);
/**
 * @brief adds an order to the end of the order list
 * @param root
 * @param Event
 */
void AddOrderToList(OrderNode** root, OrderData Order);


#endif //EXAMPLE_LINKEDLIST_H
