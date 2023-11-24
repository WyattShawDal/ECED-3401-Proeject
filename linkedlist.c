/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains linked list function definitions
*/
#include "dependencies.h"

void AddToListBeginning(Node** listRoot, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    Node* new_vehicle = malloc(sizeof(Node));
    if(new_vehicle == NULL) {
        TraceLog(LOG_ERROR, "Heap Exceeded In AEDV Allocation");
        exit(-1);
    }
    new_vehicle->data.EVIN = EVINBASE + identifierCode;
    new_vehicle->data.drawSize = (Vector2) {cellWidth, cellHeight};
    new_vehicle->data.position =  (Cord){locationX, locationY};
    new_vehicle->data.destination =  (Cord){destinationX, destinationY};
    new_vehicle->data.color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
    new_vehicle->data.currStatus = TRANSIT;

    new_vehicle->next = *listRoot;
    *listRoot = new_vehicle;
}



void SwapBetweenLists(Node** Origin, Node** Destination, int SwapEVIN) {
    Node* prev = NULL;
    Node* curr = *Origin;
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

    Node* temp = curr;
    temp->next = *Destination;
    *Destination = temp;

    Node* ne = Destination;
    Node* old = Origin;
}

void MoveToListBeginning(Node** listRoot, Node* addNode) {

}

Node* FindInList(Node* listRoot, int identifierCode) {
    Node* curr = listRoot;
    bool found = false;

    while(curr->next != NULL) {
        if(curr->data.EVIN == identifierCode) {
            found = true; //curr now points to the node with the given identifier code.
            break;
        }
        curr = curr->next;
    }
    if((!found) && (curr->data.EVIN == identifierCode))
        found = true; //checks if the value lies at either the last node, or the only node of a 1 list.

    if(found) return curr;
    else return NULL;
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

bool emptyList(queue* q, int visited) {
    /*Empties a specified queue, returns true if the queue was empty*/

    TileNode* temp = q->front;
    TileNode* freeTemp;
    bool wasEmpty = true;

    if(temp != NULL) {
        wasEmpty = false;
        if(visited == NO) { //Use the queuePrev param (empty the notVisitedQueue)
            //Loop through with two pointers, freeing the previous pointer (freeTemp)
            while(temp->queuePrev != NULL) {
                freeTemp = temp;
                temp = temp->queuePrev;
                free(freeTemp);
            }
        } else { //Use the visitedPrev parameter (empty the visitedQueue)
            while(temp->visitedPrev != NULL) {
                freeTemp = temp;
                temp = temp->visitedPrev;
                free(freeTemp);
            }
        }
        //Free the last element
        free(temp);
    }
    //Mark the queue as empty
    q->front = NULL;
    q->rear = NULL;
    return wasEmpty;
}

TileNode* new_tile(Cord loc) {
    /*Creates new TileNode with given coordinates, returns the pointer to the tile*/

    TileNode* tile = malloc(sizeof(TileNode));
    tile->coordinate = loc;
    return tile;
}






