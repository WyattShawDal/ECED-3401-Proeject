/**
 * Author: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains function definitions for moving vehicles around the map.
*/

#include "dependencies.h"

void OneWayNavigation() {
    AEDVNode* curr = ActiveList;
    AEDVNode* temp;
    //Loop through the list of AEDV's
    while(curr != NULL) {
        //If AEDV has to find a new path
        if(curr->data.nextMove == NULL) {
            switch(curr->data.currStatus) {
                case IDLE:
                    curr->data.nextMove = pathCalculation(curr->data.position,curr->data.pickUp);
                    curr->data.currStatus = PICKUP;
                    break;
                case PICKUP:
                    printf("AEDV %d made it to pickup location: %d %d\n", curr->data.EVIN, curr->data.position.x, curr->data.position.y);
                    curr->data.nextMove = pathCalculation(curr->data.position,curr->data.dropOff);
                    curr->data.currStatus = DROPOFF;
                    break;
                case DROPOFF:
                    printf("AEDV %d made it to destination %d %d\n", curr->data.EVIN, curr->data.position.x, curr->data.position.y);
                    curr->data.currStatus = IDLE;
                    break;
            }
        }
        temp = curr->next; //Save next AEDV in active list
        if(curr->data.currStatus != IDLE || curr->data.nextMove) {
            curr->data.position = curr->data.nextMove->nextPosition; //Update location
            curr->data.nextMove = curr->data.nextMove->child; //Move to next instruction
        } else
            SwapBetweenLists(&ActiveList, &InactiveList, curr->data.EVIN);
        curr = temp; //Move to next AEDV in list
    }
}

void StreetMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == EAST) {
        vehicle->position.x += 1;
    }
    else vehicle->position.x -= 1;
}

void AvenueMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == SOUTH) {
        vehicle->position.y += 1;
    }
    else vehicle->position.y -= 1;
}

