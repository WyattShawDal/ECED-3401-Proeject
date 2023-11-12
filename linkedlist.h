/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info:
*/

#ifndef EXAMPLE_LINKEDLIST_H
#define EXAMPLE_LINKEDLIST_H
#include "dependencies.h"
typedef struct Node {
    AEDV data;
    struct Node *next;
}AEDV_Node;
void AddToListBeginning(AEDV_Node** ListRoot, int locationX, int locationY, int destinationX, int destinationY, int identifierCode );

#endif //EXAMPLE_LINKEDLIST_H
