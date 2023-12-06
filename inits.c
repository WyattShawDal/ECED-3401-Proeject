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
    SpawnAEDVs();


    queueSetup(&notVisitedQueue);
    queueSetup(&visitedQueue);
    GetCommands(SETUP);

}

void GenerationCheck() {
    printf("Do you want to generate a new map file? Y/N :");
    char check;
    fscanf(stdin, "%c", &check);
    if(toupper(check) == 'Y') {
        OpenTargetFile(WRITE_BINARY, "GenerationFile.dat", &BuildFileDescriptor);

        GenerateBuildFile();
        //If a new build file is made, cannot keep vehicle file in case stables are not kept in place
        CreateVehicleFile(NEW_FILE);

    }
    else  {
        printf("Resuming operation with old map\n");
        CreateVehicleFile(KEEP_FILE);
    }

}

Cord QuadrantToStreetAddress(int quad, Cord location) {
    quad < SE ? (location.y--) : (location.y++);
    return location;
}

void SpawnAEDVs(void) {
    int EVIN;
    VehicleEntry previousEntry;
    AEDVNode * newVehicle;
    VehicleStats temp;
    bool valid = false; //false until at least one AEDV spawned
    do{
        printf("Enter a EVIN to use (10000 - 10049), -1 to begin simulation: ");
        scanf("%d",&EVIN);
        if(EVIN < 10050 && EVIN >= 10000) {
            valid = true;
            previousEntry = GetVehicleEntry(VEHICLE_NUMBER,EVIN);
            if(previousEntry.data.EVIN == -1) {
                //No previous entries with given EVIN, create new vehicle at first stable and initialze stats
                newVehicle = AddAEDV(&InactiveList, QuadrantToStreetAddress(StableList->data.quad, StableList->data.location), EVIN);
                InitAEDVStats(&newVehicle, NEW_AEDV,temp);
            } else {
                //Get stats and stable, from previous entry with given EVIN, spawn at the previous stable
                BuildingData stable = previousEntry.data.stats.lastStable;
                newVehicle = AddAEDV(&InactiveList,QuadrantToStreetAddress(stable.quad,stable.location),EVIN);
                InitAEDVStats(&newVehicle,EXISTING_AEDV,previousEntry.data.stats);
            }
        }
        else if(EVIN != -1)
            printf("EVIN out of range\n");
    } while((EVIN > 0) || !valid);
}

void InitAEDVStats(AEDVNode **vehicle, int MODE, VehicleStats existingStats) {
    if(MODE == NEW_AEDV) {
        (*vehicle)->data.stats.maxBattery = 100; //100 kw.t
        (*vehicle)->data.stats.currentBattery = (*vehicle)->data.stats.maxBattery; // # kw.t
        (*vehicle)->data.stats.drivingRate = 0.1; //kw.t/tick
        (*vehicle)->data.stats.idleRate = 0.01; //kw.t/tick
        (*vehicle)->data.stats.rechargeRate = 0.05; //kw.t/tick
        (*vehicle)->data.stats.ticksCharging = 0;
        (*vehicle)->data.stats.ticksIdle = 0;
        (*vehicle)->data.stats.ticksMoving = 0;
    }
    else {
        (*vehicle)->data.stats = existingStats;
        (*vehicle)->data.stats.currentBattery = (*vehicle)->data.stats.maxBattery;
    }
}

void ModifyAEDVStats(AEDVNode **vehicle) {
    printf("Enter battery capacity (float kw*time), "
           "recharge rate (float kw*time/tick), "
           "driving discharge rate (float kw*time/tick), "
           "and idling discharge rate (float kw*time/tick) :");
    scanf("%lf %lf %lf %lf",
          &(*vehicle)->data.stats.maxBattery,
          &(*vehicle)->data.stats.rechargeRate,
          &(*vehicle)->data.stats.drivingRate,
          &(*vehicle)->data.stats.idleRate);

    printf("Updated successfully\n\n");
}


