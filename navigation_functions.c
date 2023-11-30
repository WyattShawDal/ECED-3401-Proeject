/**
 * Author: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains function definitions for moving vehicles around the map.
*/

#include "dependencies.h"

void OneWayNavigation() {
    AEDVNode* currentVehicle = ActiveList;
    AEDVNode* savedVal;

    //Loop through the list of AEDV's
    while(currentVehicle != NULL) {
        //If AEDV has to find a new path
        if(currentVehicle->data.nextMove == NULL) {
            switch(currentVehicle->data.currStatus) {
                case IDLE:
                    currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.pickUp);
                    currentVehicle->data.currStatus = PICKUP;
                    break;
                case PICKUP:
                    printf("AEDV %d made it to pickup location: %d %d\n", currentVehicle->data.EVIN, currentVehicle->data.position.x, currentVehicle->data.position.y);
                    currentVehicle->data.nextMove = NULL;
                    currentVehicle->data.currStatus = LOADING;
                    break;

                case LOADING:
                    if(currentVehicle->data.loadingDelay == 0) {
                        currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.dropOff);
                        currentVehicle->data.currStatus = DROPOFF;
                    }
                    else {
                        currentVehicle->data.loadingDelay--;
                        currentVehicle->data.currStatus = LOADING;
                    }
                    break;

                case DROPOFF:
                    printf("AEDV %d made it to destination %d %d\n", currentVehicle->data.EVIN, currentVehicle->data.position.x, currentVehicle->data.position.y);
                    currentVehicle->data.nextMove = NULL;
                    currentVehicle->data.currStatus = UNLOADING;
                    break;
                case UNLOADING:
                    if(currentVehicle->data.unloadingDelay == 0) {
                        currentVehicle->data.currStatus = IDLE;
                    }
                    else {
                        currentVehicle->data.unloadingDelay--;
                        currentVehicle->data.currStatus = UNLOADING;
                    }
                    break;
            }
        }
        savedVal = currentVehicle->next; //Save next AEDV in active list
        if(currentVehicle->data.currStatus == DROPOFF || currentVehicle->data.currStatus == PICKUP) {
            currentVehicle->data.position = currentVehicle->data.nextMove->nextPosition; //Update location
            InstructionNode * tempMove = currentVehicle->data.nextMove;
            currentVehicle->data.nextMove = tempMove->child; //Move to next instruction
            free(tempMove);
        }
        else if(currentVehicle->data.currStatus == IDLE)
            SwapBetweenLists(&ActiveList, &InactiveList, currentVehicle->data.EVIN);
        currentVehicle = savedVal; //Move to next AEDV in list
    }
}

void UpdateVehicleStats(AEDVNode ** currentVehicle) {
    (*currentVehicle)->data.distanceTravelled++;
}

/*
 * JUST LET THE KIDDO SPEAK, HE LIKES HIS BALLOONS...
 */

