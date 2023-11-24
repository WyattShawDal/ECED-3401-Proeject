/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains functions that handle the initialization/assignments of AEDVs and their Orders
*/
#include "dependencies.h"

#ifdef DEPRECATED
void AEDVInput() {
    int tempCordX, tempCordY,tempDestX, tempDestY;
    do {
        printf("Number of AEDV's (1-4): ");
        scanf("%d", &maxAEDV);
        if(maxAEDV < 1) {
            printf("No AEDV's to assign orders\n");
        }
        if(maxAEDV > 4) {
            printf("Too many AEDV's to assign\n");
        }
    }while(maxAEDV > 4 || maxAEDV < 1);

    for (int i = 0; i < maxAEDV; ++i) {
        do {
            printf("Input starting location of AEDV [1000%d] x, y: ", i);
            scanf("%d %d", &tempCordX, &tempCordY);
            if(tempCordX < 0 || tempCordX > MAX_COLS || tempCordY < 0 || tempCordY > MAX_ROWS) {
                printf("Starting Out of Bounds\n");
            }
        }while(tempCordX < 0 || tempCordX > MAX_COLS || tempCordY < 0 || tempCordY > MAX_ROWS);

        do {
            printf("Input destination of AEDV [1000%d] x, y: ", i);
            scanf("%d %d", &tempDestX, &tempDestY);
            if(!isValidDestination(tempDestX, tempDestY))
                printf("Invalid destination\n");
        } while(!isValidDestination(tempDestX, tempDestY));
#define LLTest2
#ifdef LLTest2
        AddToListBeginning(&ActiveList, tempCordX, tempCordY, i);
#else
        InitAEDV(listOfVehicles[i], tempCordX, tempCordY, tempDestX, tempDestY, i);
#endif

    }
}
void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    vehicle->EVIN = EVINBASE + identifierCode;
    vehicle->drawSize = (Vector2) {cellWidth, cellHeight};
    vehicle->position =  (Cord){locationX, locationY};
    vehicle->destination =  (Cord){destinationX, destinationY};
    vehicle->color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
    vehicle->currStatus = TRANSIT;
}
#endif

void InitRoutine() {
    /*Raylib Setup Settings */
    SetConfigFlags(FLAG_WINDOW_RESIZABLE); //allows window resizing
    SetTraceLogLevel(LOG_ERROR); //allows for tracelog output for debugging/warnings
    //Camera Settings to show whole map
    camera.zoom = DEFAULTZOOM;
    camera.target = (Vector2) {.x = -DEFAULTOFFSET, .y = -DEFAULTOFFSET};

    int frameTarget = 20; //Amount of ticks/second
    frameCount = 0;
    //Initialization Functions
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map"); //creates the window
    SetTargetFPS(frameTarget);// Set our simulation to run at x frames-per-second
    ReadEventFile("EventFile.txt"); //read event file to populate list at program beginning
}

void GenerationCheck() {
    printf("Do you want to generate a new map file? Y/N :");
    char check;
    fscanf(stdin, "%c", &check);
    if(toupper(check) == 'Y') {
        OpenTargetFile(WRITE_BINARY, "GenerationFile.dat", &FileDescriptor);

        GenerateBuildFile();
    }
    else printf("Resuming operation with old map..");
}