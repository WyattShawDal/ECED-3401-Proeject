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
                case RESET_PICKUP:
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
                        currentVehicle->data.currStatus = RESET_PICKUP;
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
        else if(currentVehicle->data.currStatus == RESET_PICKUP)
            SwapBetweenLists(&ActiveList, &InactiveList, currentVehicle->data.EVIN);
        currentVehicle = savedVal; //Move to next AEDV in list
    }
}
void UpdateNextInfo(AEDVNode * aedv, int currentOrderNumber , int mode) {
    if(mode == PICKUP) {
        aedv->data.destination = aedv->orderList[currentOrderNumber].pickUp;
        aedv->data.delay = aedv->orderList[currentOrderNumber].pickupFloor;
    } else {
        aedv->data.destination = aedv->orderList[currentOrderNumber].dropOff;
        aedv->data.delay = aedv->orderList[currentOrderNumber].dropFloor;
    }
}

void OneWayNavigation_NEW() {
    AEDVNode* currentVehicle = ActiveList;
    AEDVNode* savedVal;


    //Loop through the list of AEDV's
    while(currentVehicle != NULL) {
        //If AEDV has to find a new path
        if(currentVehicle->data.nextMove == NULL) {
            switch(currentVehicle->data.currStatus) {
                case RESET_PICKUP:
                    if(currentVehicle->data.currentOrderNumber < currentVehicle->data.orderCount) {
                        UpdateNextInfo(currentVehicle, currentVehicle->data.currentOrderNumber, PICKUP);
                        currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.destination);
                        currentVehicle->data.currStatus = PICKUP;
                    } else {
                        currentVehicle->data.currStatus = RESET_DROPOFF;
                        currentVehicle->data.currentOrderNumber = 0;
                    }
                    break;
                case PICKUP:
                    currentVehicle->data.nextMove = NULL; // No next move because you're going to be loading
                    currentVehicle->data.currStatus = LOADING;
                    break;
                case LOADING:
                    if(currentVehicle->data.delay == 0) {
                        currentVehicle->data.currentOrderNumber ++;
                        currentVehicle->data.currStatus = RESET_PICKUP;
                    }
                    else {
                        currentVehicle->data.delay--;
                        currentVehicle->data.currStatus = LOADING;
                    }
                    break;
                case RESET_DROPOFF:
                    if(currentVehicle->data.currentOrderNumber < currentVehicle->data.orderCount) {
                        UpdateNextInfo(currentVehicle, currentVehicle->data.currentOrderNumber, DROPOFF);
                        currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.destination);
                        currentVehicle->data.currStatus = DROPOFF;
                    } else {
                        currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, QuadrantToStreetAddress(StableList->data.quad, StableList->data.location));
                        currentVehicle->data.currStatus = ETGOHOME;
                        currentVehicle->data.currentOrderNumber = 0;
                    }
                    break;
                case DROPOFF:
                    currentVehicle->data.nextMove = NULL;
                    currentVehicle->data.currStatus = UNLOADING;
                    break;
                case UNLOADING:
                    if(currentVehicle->data.delay == 0) {
                        currentVehicle->data.currentOrderNumber++;
                        currentVehicle->data.currStatus = RESET_DROPOFF;
                    }
                    else {
                        currentVehicle->data.delay--;
                        currentVehicle->data.currStatus = UNLOADING;
                    }
                    break;
                case ETGOHOME:
                    currentVehicle->data.currentOrderNumber = 0;
                    currentVehicle->data.currStatus = IDLE;
                    break;
                case IDLE:
                    SwapBetweenLists(&ActiveList, &InactiveList, currentVehicle->data.EVIN);
                    break;
            }
        }
        savedVal = currentVehicle->next; //Save next AEDV in active list
        if(currentVehicle->data.currStatus == DROPOFF || currentVehicle->data.currStatus == PICKUP || currentVehicle->data.currStatus == ETGOHOME) {
            currentVehicle->data.position = currentVehicle->data.nextMove->nextPosition; //Update location
            InstructionNode * tempMove = currentVehicle->data.nextMove;
            currentVehicle->data.nextMove = tempMove->child; //Move to next instruction
            free(tempMove);
        }
        currentVehicle = savedVal; //Move to next AEDV in list
    }
}

void UpdateVehicleStats(AEDVNode ** currentVehicle) {
    (*currentVehicle)->data.distanceTravelled++;
}

/*
 * JUST LET THE KIDDO SPEAK, HE LIKES HIS BALLOONS...
 */

