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
void SwapBetweenLists(Node** List1, Node** List2, Node* SwapNode); //swap AEDV between Active/Inactive || Inactive/Active



#endif //EXAMPLE_LINKEDLIST_H
