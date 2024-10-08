/*
 *Author: Wyatt Shaw  & Cameron Archibald
 *Created on: 2023-11-12
 *Module Info: Contains functions definitions to allow for linked list operation
*/
#include "dependencies.h"

#ifndef EXAMPLE_LINKEDLIST_H
#define EXAMPLE_LINKEDLIST_H

/**
 * @brief initializes queues with their front and rear pointing to null
 * @param m pointer to the queue to be created, passed by reference
 */
void queueSetup(queue** m);

/**
 * @brief adds new TileNode to the rear of a queue
 * @param new_tile pointer to the TileNode to be enQueued
 * @param parent pointer to the TileNode to assign as new_tile's parent, to generate tree of explored map
 * @param q pointer to the desired queue
 * @param visited parameter passed to enable adding to notVisitedList or visitedList (NO or YES)
 */
void enQueue(TileNode* new_tile, TileNode* parent, queue* q, int visited);

/**
 * @brief removes the node at the front of the queue
 * @param q pointer to the desired queue
 * @param visited parameter passed to enable adding to notVisitedList or visitedList (NO or YES)
 */
void deQueue(queue* q, int visited);

/**
 * @brief searches a queue for a given Cord (pair of coordinates)
 * @param loc coordinates to search for
 * @param q pointer to the desired queue
 * @return returns true if the coordinates were found in the queue
 */
bool searchQueue(Cord loc, queue* q);

/**
 * @brief clears the tree associated with the BFS,
 * looping through and freeing the elements of visitedQueue and notVisitedQueue
 * @param NVQ pointer to the notVisitedQueue, passed by reference
 * @param VQ "" visitedQueue ""
 */
void clearBFS(queue** NVQ, queue** VQ);

TileNode* new_tile(Cord loc);


/**
 * @brief adds an AEDV to a list of choice
 * @param listRoot list to add AEDV to
 * @param locationX  x location to create AEDV at (stable location)
 * @param locationY y location to create AEDV at (stable location)
 * @param identifierCode unique EVIN for AEDV
 */
AEDVNode* AddAEDV(AEDVNode** listRoot, Cord spawnLocation, int identifierCode );

/**
 * @brief moves AEDVNode between the active list and inactive list
 * @param Origin pointer to the list to remove from
 * @param Destination pointer to the list to add to
 * @param SwapEVIN EVIN number of the vehicle to be swapped
 */
void SwapBetweenLists(AEDVNode** Origin, AEDVNode** Destination, int SwapEVIN);

AEDVNode* FindInList(AEDVNode* listRoot, int identifierCode);

/**
 * @brief adds an event node to the end of the event list
 * @param root pointer to the list of events, passed by reference
 * @param Event event struct to add
 */
void AddEvent(EventNode** root, EventData Event);

/**
 * @brief adds an order node to the end of the order list
 * @param root pointer to the list of orders, passed by reference
 * @param Order order struct to add
 */
void AddOrderToList(OrderNode** root, OrderData Order);

/**
 * @brief frees lists of AEDV's and map tiles
 */
void FreeRoutine(void);

/**
 * Adds a new building to
 * @param stableList
 * @param ChargerList
 * @param building
 */
void AddBuilding(BuildingNode** stableList, BuildingNode** chargerList, BuildingData building);

#endif //EXAMPLE_LINKEDLIST_H
