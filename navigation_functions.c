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
                    currentVehicle->data.nextMove = pathCalculation(currentVehicle->data.position, currentVehicle->data.dropOff);
                    currentVehicle->data.currStatus = DROPOFF;
                    break;
                case DROPOFF:
                    printf("AEDV %d made it to destination %d %d\n", currentVehicle->data.EVIN, currentVehicle->data.position.x, currentVehicle->data.position.y);
                    currentVehicle->data.currStatus = IDLE;
                    break;
            }
        }
        savedVal = currentVehicle->next; //Save next AEDV in active list
        if(currentVehicle->data.currStatus != IDLE) {
            currentVehicle->data.position = currentVehicle->data.nextMove->nextPosition; //Update location
            InstructionNode * tempMove = currentVehicle->data.nextMove;
            currentVehicle->data.nextMove = tempMove->child; //Move to next instruction
            free(tempMove);
        } else
            SwapBetweenLists(&ActiveList, &InactiveList, currentVehicle->data.EVIN);
        currentVehicle = savedVal; //Move to next AEDV in list
    }
}
