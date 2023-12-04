/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains functions that handle the initialization/assignments of AEDVs and their Orders
*/
#include "dependencies.h"


void InitRoutine() {
    /*Raylib Setup Settings */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //allows window resizing
    SetTraceLogLevel(LOG_ERROR); //allows for tracelog output for debugging/warnings
    //Camera Settings to show whole map
    camera.zoom = DEFAULTZOOM;
    camera.target = (Vector2) {.x = -DEFAULTOFFSET, .y = -DEFAULTOFFSET};

    frameCount = 0;
    //Initialization Functions
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map"); //creates the window
    SetTargetFPS(frameRate);// Set our simulation to run at x frames-per-second
    ReadEventFile("EventFile.txt"); //read event file to populate list at program beginning
    CreateRelativeCustomerFile();
    CreateDeliveryFile();
    SpawnAEDVs(2);


    queueSetup(&notVisitedQueue);
    queueSetup(&visitedQueue);
    GetCommands(SETUP);

}

void GenerationCheck() {
    printf("Do you want to generate a new map file? Y/N :");
    char check;
    fscanf(stdin, "%c", &check);
    if(toupper(check) == 'Y') {
        OpenTargetFile(WRITE_BINARY, "GenerationFile.dat", &FileDescriptor);

        GenerateBuildFile();
    }
    else printf("Resuming operation with old map\n");
}

Cord QuadrantToStreetAddress(int quad, Cord location) {
    quad < SE ? (location.y--) : (location.y++);
    return location;
}

void SpawnAEDVs(int num) {
    BuildingNode * temp = StableList;
    for(int i = 0;i < num;i++) {
        AddAEDV(&InactiveList, QuadrantToStreetAddress(temp->data.quad, temp->data.location),i);
        temp = temp->nextBuilding;
        if(temp == NULL) temp = StableList;
    }
    AEDVNode* currentVehicle = InactiveList;
    while(currentVehicle !=  NULL) {
        InitAEDVStats(&currentVehicle);
        currentVehicle = currentVehicle->next;
    }
}

void InitAEDVStats(AEDVNode **vehicle) {
    (*vehicle)->data.stats.maxBattery = 100; //100 kw.t
    (*vehicle)->data.stats.currentBattery = (*vehicle)->data.stats.maxBattery; // # kw.t
    (*vehicle)->data.stats.drivingRate = 0.1; //kw.t/tick
    (*vehicle)->data.stats.idleRate = 0.01; //kw.t/tick
    (*vehicle)->data.stats.rechargeRate = 0.05; //kw.t/tick
    (*vehicle)->data.stats.ticksCharging = 0;
    (*vehicle)->data.stats.ticksIdle = 0;
    (*vehicle)->data.stats.ticksMoving = 0;
}
