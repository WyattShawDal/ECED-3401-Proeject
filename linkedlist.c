/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains linked list function definitions
*/
#include "dependencies.h"

void AddAEDV(AEDVNode** listRoot, int locationX, int locationY, int identifierCode) {
    AEDVNode* new_vehicle = malloc(sizeof(AEDVNode));
    if(new_vehicle == NULL) {
        TraceLog(LOG_ERROR, "Heap Exceeded In AEDV Allocation");
        exit(-1);
    }
    new_vehicle->data.EVIN = EVINBASE + identifierCode;
    new_vehicle->data.drawSize = (Vector2) {cellWidth, cellHeight};
    new_vehicle->data.position =  (Cord){locationX, locationY};
    new_vehicle->data.color = RED;
    new_vehicle->data.pickUpFloorDelay = 500; //temp val pls fix
    new_vehicle->data.currStatus = IDLE;
    new_vehicle->data.nextMove = NULL;
    new_vehicle->next = *listRoot;
    *listRoot = new_vehicle;
}

void SwapBetweenLists(AEDVNode** Origin, AEDVNode** Destination, int SwapEVIN) {

    AEDVNode* prev = NULL;
    AEDVNode* curr = *Origin;
    bool found = false;
    while(curr->next != NULL) {
        if(curr->data.EVIN == SwapEVIN) {
            found = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if((!found) && (prev != NULL)) {
        prev->next = NULL;
        //node is at the end (curr)
    }
    else if(prev == NULL) {
        *Origin = curr->next;
        //node is at the start (curr)
    }
    else {
        prev->next = curr->next;
        //node is in the middle (curr)
    }

    AEDVNode* temp = curr;
    temp->next = *Destination;
    *Destination = temp;

}

AEDVNode* FindInList(AEDVNode* listRoot, int identifierCode) {
    AEDVNode* curr = listRoot;
    bool found = false;

    while(curr != NULL) {
        if(curr->data.EVIN == identifierCode) {
            found = true; //curr now points to the node with the given identifier code.
            break;
        }
        curr = curr->next;
    }
    if(found) return curr;
    else return NULL;
}
void AddEvent(EventNode** root, EventData Event) {
    EventNode *newEvent = malloc(sizeof(EventNode));
    if(newEvent == NULL) {
        exit(-1);
    }
    newEvent->eventData = Event;
    newEvent->nextEvent = NULL;
    if(*root == NULL) {
        newEvent->nextEvent = NULL;
        *root = newEvent;
        return;
    }
    EventNode* curr = *root;
    while (curr->nextEvent != NULL) {
        curr = curr->nextEvent;
    }
    curr->nextEvent = newEvent;
}
//Adds order to end of the order list, so oldest is at head of list.
void AddOrderToList(OrderNode** root, OrderData Order) {
    OrderNode *newOrder = malloc(sizeof(EventNode));
    if(newOrder == NULL) {
        exit(-1);
    }
    newOrder->data = Order;
    newOrder->nextOrder = NULL;
    if(*root == NULL) {
        newOrder->nextOrder = NULL;
        *root = newOrder;
        return;
    }
    OrderNode* curr = *root;
    while (curr->nextOrder != NULL) {
        curr = curr->nextOrder;
    }
    curr->nextOrder = newOrder;
}

void queueSetup(queue** m) {
    /* Creates a queue with a base pointer passed in
    Queue contains pointers to the front and rear TileNode */
    *m = (queue*) malloc(sizeof(queue));
    (*m)->front = NULL;
    (*m)->rear = NULL;
}


void enQueue(TileNode* new_tile, TileNode* parent, queue* q, int visited) {
    /*Enqueues to specific queue
    new_tile is the TileNode to enqueue
    visited is a parameter to select the mode. v = NO, enQueue to notVisitedList. v = YES, enQueue to visitedList
    notVisitedList uses queuePrev to link its nodes, visitedList uses visitedPrev to link its nodes */

    //Assign parent when enQueuing to the notVisitedList
    if(visited == NO) new_tile->parent = parent;

    if(q->front == NULL) { //If empty queue
        q->front = new_tile;
        q->rear = new_tile;
    } else { //enQueue to specified list
        visited == NO ? (q->rear->queuePrev = new_tile) : (q->rear->visitedPrev = new_tile);
        q->rear = new_tile;
    }
    // set the new_tile (rear) to point to NULL
    visited == NO ? (new_tile->queuePrev = NULL) : (new_tile->visitedPrev = NULL);
}

void deQueue(queue* q, int visited) {
    /*deQueues front item from specified queue, visited is parameter to use queuePrev or visitedPrev*/

    TileNode * temp = q->front;
    //Assign new front
    visited == NO ? (q->front = q->front->queuePrev) : (q->front = q->front->visitedPrev);
    //If empty, update rear to NULL
    if(q->front == NULL) q->rear = NULL;
    //Change queuePrev or visitedPrev to NULL so the front node is no longer in the queue
    visited == NO ? (temp->queuePrev = NULL) : (temp->visitedPrev = NULL);
}

bool searchQueue(Cord loc, queue* q) {
    /*searches visitedQueue for given coordinates*/

    TileNode* temp = q->front;
    bool found = false;

    if(temp != NULL) {
        //Increment along list comparing node's coordinates with loc
        while(temp->visitedPrev != NULL) {
            if(temp->coordinate.x == loc.x && temp->coordinate.y == loc.y)
                found = true;
            temp = temp->visitedPrev;
        }
        //Check if coordinates were found in the first or last element of visitedQueue
        if(!found)
            if(temp->coordinate.x == loc.x && temp->coordinate.y == loc.y)
                found = true;
    }
    return found;
}
void clearBFS(queue** NVQ, queue** VQ) {

    TileNode * temp = (*VQ)->front;
    TileNode * tempPrev;
    while(temp != NULL) {
        tempPrev = temp;
        temp = temp->visitedPrev;
        free(tempPrev);

    }

    temp = (*NVQ)->front;
    while(temp != NULL) {
        tempPrev = temp;
        temp = temp->queuePrev;
        free(tempPrev);

    }

    (*NVQ)->front = NULL;
    (*NVQ)->rear = NULL;

    (*VQ)->front = NULL;
    (*VQ)->rear = NULL;
}



TileNode* new_tile(Cord loc) {
    /*Creates new TileNode with given coordinates, returns the pointer to the tile*/
    TileNode* tile = malloc(sizeof(TileNode));
    tile->coordinate = loc;
    return tile;
}

void FreeRoutine(void) {
    AEDVNode* temp = InactiveList;
    AEDVNode* prevTemp;
    //Free InactiveList
    while(temp != NULL) {
        prevTemp = temp;
        temp = temp->next;
        free(prevTemp);
    }
    //Free ActiveList (if user exits program before all deliveries are made).
    temp = ActiveList;
    while(temp != NULL) {
        prevTemp = temp;
        temp = temp->next;
        free(prevTemp);
    }
    //Free map
    for (int i = 0; i < MAX_ROWS; ++i) {
        free(dynamicMap[i]);
    }
    free(dynamicMap);
}
