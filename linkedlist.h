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
void SwapBetweenLists(Node* Origin, Node* Destination, int SwapEVIN); //swap AEDV between Active/Inactive || Inactive/Active
void MoveToListBeginning(Node** listRoot, Node* addNode);
Node* FindInList(Node* listRoot, int identifierCode);



#endif //EXAMPLE_LINKEDLIST_H
