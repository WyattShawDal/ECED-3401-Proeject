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
#include "one_way.h"

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
Node * InactiveList = NULL;
Tile **dynamicMap;
queue * notVisitedQueue;
queue * visitedQueue;

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

    //#define SEARCHTEST2
#ifdef SEARCHTEST2
    TileNode* tile;
    Cord location;
    char tmp;

    while(1) {
        printf("Enter x y to search or enqueue or clear or dequeue:\n");
        scanf("%d %d %c", &location.x, &location.y, &tmp);
        if(tmp == 'e')
            enQueue(location, notVisitedQueue);
        else if(tmp == 's')
            printf("Found if 1, not found if 0: %d\n",searchQueue(location, notVisitedQueue));
        else if(tmp == 'c')
            emptyList(notVisitedQueue);
        else if(tmp == 'd')
            deQueue(notVisitedQueue);
        tile = notVisitedQueue->front;
    }
#endif
//#define SEARCHTEST
#ifdef SEARCHTEST
    Cord coordinate;
    coordinate.x = 0;
    coordinate.y = 1;
    enQueue(coordinate, searchQueue);
    coordinate.x ++;
    coordinate.y ++;
    enQueue(coordinate, searchQueue);
    deQueue(searchQueue);
    queue* q = searchQueue;
#endif
//#define LLTest4 //Tests the swapping function
#ifdef LLTest4
    SwapBetweenLists(&ActiveList, &InactiveList, 10000);
    SwapBetweenLists(&ActiveList, &InactiveList, 10001);
    SwapBetweenLists(&ActiveList, &InactiveList, 10002);
    SwapBetweenLists(&ActiveList, &InactiveList, 10003);
    SwapBetweenLists(&InactiveList, &ActiveList, 10002);
    //SwapBetweenLists(&InactiveList, &ActiveList, 10000);
    //SwapBetweenLists(&InactiveList, &ActiveList, 10000);

    Node* active = ActiveList;
    Node* inactive = InactiveList;
#endif
    //SwapBetweenLists(&InactiveList, &ActiveList, 10000);
//#define LLTest3
#ifdef LLTest3 //tests the searching function
    while(1) {
        int code;
        printf("Enter desired AEDV code: ");
        scanf("%d", &code);
        Node* nodal = FindInList(ActiveList, code);
        if(nodal == NULL)
            printf("NULL\n");
        else
            printf("Found: %d %d\n", nodal->data.position.x, nodal->data.position.y);
    }
#endif

    queueSetup(&notVisitedQueue);
    queueSetup(&visitedQueue);
    Cord startingLoc = {.x = 0, .y = 0};
    Cord endingLoc = {.x = 0, .y = 7};

    InstructionNode* t = pathCalculation(startingLoc,endingLoc);
    emptyList(notVisitedQueue, NO);
    emptyList(visitedQueue, NO);
    queue * n = notVisitedQueue;
    queue * v = visitedQueue;

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