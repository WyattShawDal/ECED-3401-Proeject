/*******************************************************************************************
*  Author: Wyatt Shaw & Cameron Archibald
*  Date: October 31 2023
*  Module Info: The following module implements 'dumb' AEDV movement, allowing up to four AEDVs to navigate to given destination points
*  Dependency Info: In order to handle graphical output, the program utilizes the open source library Raylib.h
*  https://github.com/raysan5/raylib
*  Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "dependencies.h"
#include "typedefs.h"

/* Global Variables Definition */
//File Pointers
FILE* BuildingFileDescriptor;
//Screen definitions
const int screenWidth = 1000;
const int screenHeight = 1000;
Camera2D camera = { 0 };
//Int Vars
int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight;

//Aedv's REMOVE
static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;

//AEDV Lists
AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};
Node * ActiveList = NULL;
Node * InactiveList = NULL;
//Map
Tile **dynamicMap;

// Main Entry Point
int main() {
    //Allows Window Resizing (Doesn't affect the Grid Dimensions)
    OpenBinaryFile();
    GenerateBuildFile();
#ifdef Files
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //Disables INFO output in console window at startup allows tracelogs to alert hard errors
    SetTraceLogLevel(LOG_ERROR);
    //Camera Settings to show whole picture
    camera.zoom = DEFAULTZOOM;
    camera.target = (Vector2) {.x = -DEFAULTOFFSET, .y = -DEFAULTOFFSET};
    //adjusting frame target will speed up operation of the navigation screen , recommended is 10 or less
    //although higher is possible
    int frameTarget = 10; //each tick is .1 seconds
    //Initialization Functions
    SetupInitialConditions();
    SwapBetweenLists(ActiveList, InactiveList, 10000);
//#define LLTest3
#ifdef LLTest3 //tests the searching function
    while(1) {
        int code;
        printf("Enter desired AEDV code: ");
        scanf("%d", &code);
        Node* nodal = FindInList(InactiveList, code);
        if(nodal == NULL)
            printf("NULL\n");
        else
            printf("Found: %d %d\n", nodal->data.position.x, nodal->data.position.y);
    }
#endif
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map");
    SetTargetFPS(frameTarget);// Set our simulation to run at x frames-per-second

    // Main simulation loop
    while (!WindowShouldClose())   // Detect window close button or ESC key
    {
        CameraControl();
        UpdateDrawFrame();
    }
    // De-Initialization
    // Close window and OpenGL context
    CloseWindow();
    //free rows, then free cols
    for (int i = 0; i < MAX_ROWS; ++i) {
        free(dynamicMap[i]);
    }
    free(dynamicMap);
#endif
    return 0;
}