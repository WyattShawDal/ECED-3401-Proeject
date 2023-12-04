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
        aedv->data.destination = aedv->orderList[currentOrderNumber].pickUp; //Get destination from order in current index
        //Delay in ticks = (floors * ticks / second * seconds / floor) / delayScale = ticks
        aedv->data.delay = aedv->orderList[currentOrderNumber].pickupFloor*frameRate*delaySecsPerFloor/delayScale;
    } else {
        aedv->data.destination = aedv->orderList[currentOrderNumber].dropOff;
        aedv->data.delay = aedv->orderList[currentOrderNumber].dropFloor*frameRate*delaySecsPerFloor/delayScale;
    }
}

void OneWayNavigation_NEW() {
    AEDVNode* currentVehicle = ActiveList;
    AEDVNode* savedVal;


    //Loop through the list of AEDV's
    while(currentVehicle != NULL) {
        savedVal = currentVehicle->next; //Save next AEDV in active list
        //If AEDV has no further instructions
        if(currentVehicle->data.nextMove == NULL) {
            NoMoveCalculated(currentVehicle);
        }

        if(currentVehicle->data.currStatus == DROPOFF || currentVehicle->data.currStatus == PICKUP || currentVehicle->data.currStatus == ETGOHOME) {
            currentVehicle->data.position = currentVehicle->data.nextMove->nextPosition; //Update location
            InstructionNode * tempMove = currentVehicle->data.nextMove;
            currentVehicle->data.nextMove = tempMove->child; //Move to next instruction
            free(tempMove);
        }
        currentVehicle = savedVal; //Move to next AEDV in list
    }
}

void NoMoveCalculated(AEDVNode * currentVehicle) {
    switch(currentVehicle->data.currStatus) {
        case RESET_PICKUP:
            //If orders left to pickup
            if(currentVehicle->data.currentOrderNumber < currentVehicle->data.orderCount) {
                UpdateNextInfo(currentVehicle, currentVehicle->data.currentOrderNumber, PICKUP); //Load destination and floor delay info for pickup customer
                currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.destination); //Calculate movements to pickup customer
                currentVehicle->data.currStatus = PICKUP;
            } else {
                //No more orders left to pickup, begin dropping off starting at order 0
                currentVehicle->data.currentOrderNumber = 0;
                currentVehicle->data.currStatus = RESET_DROPOFF;
            }
            break;
        case PICKUP:
            //Once AEDV has no more moves in PICKUP state, begin LOADING
            UpdateDeliveryStats(&currentVehicle,currentVehicle->data.currentOrderNumber,PICKUP);
            currentVehicle->data.currStatus = LOADING;
            break;
        case LOADING:
            if(currentVehicle->data.delay == 0) {
                //Delay over, move to next order in RESET_PICKUP
                currentVehicle->data.currentOrderNumber ++;
                currentVehicle->data.currStatus = RESET_PICKUP;
            }
            else {
                //Decrement delay, stay in LOADING
                currentVehicle->data.delay--;
            }
            break;
        case RESET_DROPOFF:
            //If orders left to deliver
            if(currentVehicle->data.currentOrderNumber < currentVehicle->data.orderCount) {
                UpdateNextInfo(currentVehicle, currentVehicle->data.currentOrderNumber, DROPOFF); //Load destination and floor delay based on dropoff customer
                currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.destination); //Calculate path from position to dropoff customer
                currentVehicle->data.currStatus = DROPOFF;
            } else {
                //No more orders, calculate path from position back to closest stable
                BuildingNode * returnStable = FindClosestBuilding(currentVehicle->data.position, STABLE);
                currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, QuadrantToStreetAddress(returnStable->data.quad, returnStable->data.location));
                currentVehicle->data.currentOrderNumber = 0; //Current order to perform reset to 0 (for next time AEDV is assigned orders)
                currentVehicle->data.currStatus = ETGOHOME;
            }
            break;
        case DROPOFF:
            //Finished dropping off, begin unloading
            currentVehicle->data.currStatus = UNLOADING;
            break;
        case UNLOADING:
            if(currentVehicle->data.delay == 0) {
                //Delay over, move to next order
                UpdateDeliveryStats(&currentVehicle,currentVehicle->data.currentOrderNumber,DROPOFF);
                AddToDeliveryFile(currentVehicle->orderList[currentVehicle->data.currentOrderNumber]);
                currentVehicle->data.currentOrderNumber++;
                currentVehicle->data.currStatus = RESET_DROPOFF;
            }
            else {
                //Decrement delay, stay unloading
                currentVehicle->data.delay--;
            }
            break;
        case ETGOHOME:
            //Arrived at stable, move to IDLE
            currentVehicle->data.currStatus = IDLE;
            break;
        case IDLE:
            //Move to inactive list
            SwapBetweenLists(&ActiveList, &InactiveList, currentVehicle->data.EVIN);
            break;
    }
}

BuildingNode* FindClosestBuilding(Cord position, int mode) {
    BuildingNode * currentNode;
    if(mode == STABLE)
        currentNode = StableList;
    else
        currentNode = ChargerList;

    double currentDistance;
    double minDistance;

    //Minimum distance is the first node
    BuildingNode * minNode = currentNode;
    minDistance = FindDiagonalDistance(position, minNode->data.location);
    currentNode = currentNode->nextBuilding;

    //While more nodes exists, compare their distance to the min distance
    while(currentNode != NULL) {
        currentDistance = FindDiagonalDistance(position,currentNode->data.location);
        if(currentDistance < minDistance) {
            //New minDistance and minNode are the current distance and node being analyzed
            minDistance = currentDistance;
            minNode = currentNode;
        }
        currentNode = currentNode->nextBuilding;
    }
    //Return the closest node to the current
    return minNode;
}

double FindDiagonalDistance(Cord pos1, Cord pos2) {
    return sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
}
void UpdateVehicleStats(AEDVNode ** vehicle) {
    if ((*vehicle)->data.currStatus == IDLE || (*vehicle)->data.currStatus == LOADING ||
        (*vehicle)->data.currStatus == UNLOADING) {
        (*vehicle)->data.stats.ticksIdle++;
    } else if ((*vehicle)->data.currStatus == PICKUP || (*vehicle)->data.currStatus == DROPOFF) {
        (*vehicle)->data.stats.ticksMoving++;
        (*vehicle)->data.stats.currentBattery -= (*vehicle)->data.stats.drivingRate;
    }
}

void UpdateDeliveryStats(AEDVNode ** currentVehicle, int orderNum, int mode) {
    if(mode == PICKUP) {
        (*currentVehicle)->orderList[orderNum].pickupTime = frameCount;
    } else {
        (*currentVehicle)->orderList[orderNum].dropTime = frameCount;
    }
}
