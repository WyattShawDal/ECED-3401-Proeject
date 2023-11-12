/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info:
*/
#include "dependencies.h"

void AddToListBeginning(AEDV_Node** listRoot, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    AEDV_Node* new_vehicle = malloc(sizeof(AEDV_Node));
    if(new_vehicle == NULL) {
        TraceLog(LOG_ERROR, "Heap Exceeded In AEDV Allocation");
        exit(-1);
    }
    new_vehicle->data.EVIN = EVINBASE + identifierCode;
    new_vehicle->data.drawSize = (Vector2) {cellWidth, cellHeight};
    new_vehicle->data.position =  (Cord){locationX, locationY};
    new_vehicle->data.destination =  (Cord){destinationX, destinationY};
    new_vehicle->data.color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
    new_vehicle->data.currStatus = TRANSIT;

    new_vehicle->next = *listRoot;
    *listRoot = new_vehicle;
}