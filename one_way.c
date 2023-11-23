/*
 *Author: Cameron Archibald
 *Created on: 23 Nov 2023
 *Module Info: Implement BFS algorithm and moving of vehicles
*/

#include "dependencies.h"

InstructionNode* reverseInstructions(TileNode* end){
    TileNode* curr = end;
    TileNode* next;

    InstructionNode* currCopy = malloc(sizeof(InstructionNode));
    InstructionNode* nextCopy;
    currCopy->nextPosition = curr->coordinate;
    currCopy->child = NULL;

    while(curr->parent != NULL) {
        next = curr->parent;
        nextCopy = malloc(sizeof(InstructionNode));
        nextCopy->nextPosition = next->coordinate;
        nextCopy->child = currCopy;
        free(curr);
        currCopy = nextCopy;
        curr = next;
    }
    free(next);
    return nextCopy;
}

InstructionNode* pathCalculation(Cord start, Cord end) {
    Cord position = start;
    Cord nextPosition;

    TileNode* current = new_tile(start);
    TileNode* dest;

    enQueue(current, NULL,notVisitedQueue, NO);

    while(notVisitedQueue->front != NULL) {
        current = notVisitedQueue->front;
        enQueue(current, NULL, visitedQueue, YES);
        position = current->coordinate;
        if(current->coordinate.x != end.x || current->coordinate.y != end.y) {
            if(dynamicMap[position.x][position.y].validDirection[EAST] == true) {
                nextPosition = position;
                nextPosition.x++;
                if(!searchQueue(nextPosition,visitedQueue))
                    enQueue(new_tile(nextPosition),current,notVisitedQueue, NO);
            }
            if(dynamicMap[position.x][position.y].validDirection[WEST] == true) {
                nextPosition = position;
                nextPosition.x--;
                if(!searchQueue(nextPosition,visitedQueue))
                    enQueue(new_tile(nextPosition),current,notVisitedQueue, NO);
            }
            if(dynamicMap[position.x][position.y].validDirection[NORTH] == true) {
                nextPosition = position;
                nextPosition.y--;
                if(!searchQueue(nextPosition,visitedQueue))
                    enQueue(new_tile(nextPosition),current,notVisitedQueue, NO);
            }
            if(dynamicMap[position.x][position.y].validDirection[SOUTH] == true) {
                nextPosition = position;
                nextPosition.y++;
                if(!searchQueue(nextPosition,visitedQueue))
                    enQueue(new_tile(nextPosition),current,notVisitedQueue, NO);
            }
        } else {
            dest = current;
            break;
        }
        deQueue(notVisitedQueue, NO);
    }


    TileNode* temp = current;
    while(temp->parent != NULL) {
        printf("%d %d\n", temp->coordinate.x, temp->coordinate.y);
        temp = temp->parent;
    }
    InstructionNode* temp2 = reverseInstructions(current);
    while(temp2->child != NULL) {
        printf("%d %d\n", temp2->nextPosition.x, temp2->nextPosition.y);
        temp2 = temp2->child;
    }
    return NULL;
}


