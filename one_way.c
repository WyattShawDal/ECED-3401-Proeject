/*
 *Author: Cameron Archibald
 *Created on: 23 Nov 2023
 *Module Info: Implement BFS algorithm and moving of vehicles
*/

#include "dependencies.h"

InstructionNode* reverseInstructions(TileNode* end){
    /* Reverse linked list of TileNodes which points from destination to origin,
     * create new linked list of InstructionNodes which points from origin to destination */

    //Nodes to increment along old linked list, curr->parent = next
    TileNode* curr = end;
    TileNode* next;

    //Copies of curr and next, nextCopy->child = curr (reversing direction)
    InstructionNode* currCopy = malloc(sizeof(InstructionNode));
    InstructionNode* nextCopy;
    currCopy->nextPosition = curr->coordinate;
    currCopy->child = NULL;

    //Move through old list, copying next to nextCopy and linking nextCopy to the new list
    //Old list is not freed until ClearBFS is called, otherwise the links in the queues would be lost
    while(curr->parent != NULL) {
        next = curr->parent;
        nextCopy = malloc(sizeof(InstructionNode));
        nextCopy->nextPosition = next->coordinate;
        nextCopy->child = currCopy;
        currCopy = nextCopy;
        curr = next;
    }
    //Return pointer to nextCopy (origin)
    return nextCopy;
}

InstructionNode* pathCalculation(Cord start, Cord end) {
    /* Uses breadth-first search algorithm
     * to generate a linked list of TileNodes that points from the destination to the origin.
     *
     * dynamicMap[][] stores validDirections, indicating where an AEDV can move from a given tile in the array.
     * Using dynamicMap, BFS generates a tree of TileNodes branching from the origin, and every TileNode points to its parent.
     * Once the tree finds the destination, the shortest path is traced up through parents from destination to origin.
     *
     * Calls reverseInstructions() to reverse the linked list, so it points from origin to destination.
     * Returns the first node in the linked list, which contains the first move the AEDV will need to perform.
     * */

    if(start.x == end.x && start.y == end.y) {
        //If start == end, return a single instruction that makes one move to the current location
        InstructionNode* instruction = malloc(sizeof(InstructionNode));
        instruction->child = NULL;
        instruction->nextPosition = start;
        return instruction;
    }

    TileNode* current = new_tile(start);
    TileNode* dest = NULL;

    //Add origin to notVisitedQueue
    enQueue(current, NULL,notVisitedQueue, NO);

    //Move through notVisitedQueue
    while((notVisitedQueue->front != NULL) && (dest == NULL)) {
        current = notVisitedQueue->front;
        //Add current node to visitedQueue
        enQueue(current, NULL, visitedQueue, YES);
        if(current->coordinate.x != end.x || current->coordinate.y != end.y) {
            for(int direction = SOUTH;direction <= WEST;direction++) {
                searchAdjacentTiles(direction, current->coordinate, current);
            }
        } else {
            //Current position = destination position, path found, save in TileNode destination
            dest = current;
        }
        //Remove current node from notVisitedQueue
        deQueue(notVisitedQueue, NO);
    }
    //Reverse the linked list
    InstructionNode * returnInstruction = reverseInstructions(current);

    //Free the queues to allow for use in future calculations
    clearBFS(&notVisitedQueue, &visitedQueue);

    //Return node of the first instruction
    return returnInstruction;
}

void searchAdjacentTiles(int direction, Cord position, TileNode* parent) {

    //Search if vehicle could move to adjacent tiles from current position
    if(dynamicMap[position.x][position.y].validDirection[direction] == true) {
        Cord nextPosition = position;
        switch(direction) {
            case SOUTH:
                nextPosition.y++;
                break;
            case NORTH:
                nextPosition.y--;
                break;
            case EAST:
                nextPosition.x++;
                break;
            case WEST:
                nextPosition.x--;
                break;
        }
        //If the adjacent node is not in the visitedQueue, add it to the visitedQueue
        if(!searchQueue(nextPosition,visitedQueue))
            enQueue(new_tile(nextPosition), parent, notVisitedQueue, NO);
    }
}


