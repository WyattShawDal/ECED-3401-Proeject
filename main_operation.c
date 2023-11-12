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

//Screen definitions
const int screenWidth = 1000;
const int screenHeight = 1000;
Camera2D camera = { 0 };
int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight;

static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;
/* TODO update this to linked list :) */
AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};
Node * ActiveList = NULL;
Tile **dynamicMap;

// Main Entry Point
int main() {
    //Allows Window Resizing (Doesn't affect the Grid Dimensions)
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
    for (int i = 0; i < MAX_ROWS; ++i) {
        free(dynamicMap[i]);
    }
    free(dynamicMap);
    return 0;
}