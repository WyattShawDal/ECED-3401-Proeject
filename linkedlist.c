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



void SwapBetweenLists(Node* Origin, Node* Destination, int SwapEVIN) {
    Node* prev = NULL;
    Node* curr = Origin;
    bool found = false;
    while(curr->next != NULL) {
        if(curr->data.EVIN == SwapEVIN) {
            found = true;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    Node* temp = curr;
    temp->next = Destination;
    Destination = temp;
    if((!found) && (prev != NULL)) {
        prev->next = NULL;
        //node is at the end (curr)
    }
    else if(prev == NULL) {
        Origin = curr->next;
        //node is at the start (curr)
    }
    else {
        prev->next = curr->next;
        //node is in the middle (curr)
    }
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

void AddEvent(EventNode** root, EVENT Event) {
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