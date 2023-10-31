/*******************************************************************************************
*  Author: Wyatt Shaw and Cameron Archibald
*  Date: October 31 2023
*  Module Info: The following module implements 'dumb' AEDV movement, allowing up to four AEDVs to navigate to given destination points
*  Dependency Info: In order to handle graphical output, the module utilizes Raylib.h
*  https://github.com/raysan5/raylib
*  Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "dependencies.h"
#include "structs.h"


//----------------------------------------------------------------------------------
// Const Defines
//----------------------------------------------------------------------------------
#ifdef OLD
#define MAX_COLS 21
#define MAX_ROWS 21
const int cellWidth = screenWidth/MAX_COLS;
const int cellHeight = screenWidth/MAX_COLS;
#endif
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1000;
const int screenHeight = 1000;

int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight;
char defaultTest;
Camera2D camera = { 0 };

static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;
AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};

//Tile map[MAX_COLS][MAX_ROWS];
 Tile **dynamicMap;


//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    //Allows Window Resizing (Doesn't effect the Grid Dimensions)
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    //Disables INFO output in console window at startup
    SetTraceLogLevel(LOG_ERROR);

    printf("Default Test? (Y or N): ");
    scanf("%c", &defaultTest);

    if(defaultTest == 'Y') {
        MAX_COLS = 21;
        MAX_ROWS = 21;
        cellHeight = screenWidth/MAX_COLS;
        cellWidth = screenWidth/MAX_COLS;
        printf("Dimensions of map defaulted to %d columns by %d rows\n", MAX_COLS, MAX_ROWS);
        InitAEDV(listOfVehicles[0], 1, 0, 10, 12, 0);
    }
    else {

        printf("Number of Columns: ");
        scanf("%d",&MAX_COLS);
        printf("Number of Rows: ");
        scanf("%d",&MAX_ROWS);
        printf("You inputted %d x %d dimensions\n", MAX_COLS, MAX_ROWS);
        cellHeight = screenWidth/MAX_COLS;
        cellWidth = screenWidth/MAX_COLS;
        AEDVInput();

    }
    camera.zoom = 1.0f;
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map");
    SetTargetFPS(8);// Set our simulation to run at x frames-per-second
    //--------------------------------------------------------------------------------------
    // Main simulation loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        CameraControl();
        UpdateDrawFrame();
    }
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

void UpdateDrawFrame(void)
{


    //----------------------------------------------------------------------------------
    // Raylib Functions:
    for (int i = 0; i < 4; ++i) {
        if(listOfVehicles[0]->currStatus == UNLOADING) {
            printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[0]->EVIN);
            scanf("%d %d", &listOfVehicles[0]->destination.x,&listOfVehicles[0]->destination.y);
            listOfVehicles[0]->currStatus = TRANSIT;
        }
        //MapNavigation(listOfVehicles[i]);
    }

    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    //Draw the map into the window using tile data generated in main
    for (int CURR_COL = 0; CURR_COL < MAX_COLS; ++CURR_COL) {
        for(int CURR_ROW = 0; CURR_ROW < MAX_ROWS; ++CURR_ROW) {
            if(CURR_COL == 0) {
                DrawText(TextFormat("%d", CURR_ROW), -30, CURR_ROW * cellHeight +12, 20, WHITE);
            }
            if(CURR_ROW == 0) {
                DrawText(TextFormat("%d", CURR_COL), CURR_COL * cellWidth +12, -30, 20, WHITE);
            }
            DrawMap(dynamicMap[CURR_COL][CURR_ROW]);
        }
    }
    //If the destination value is invalid don't bother attempting to nav

    //begin navigation
    if(defaultTest == 'Y') {
        OneWayNavigation(listOfVehicles[0]);

    }
    else{
        for (int i = 0; i < maxAEDV; ++i) {
            OneWayNavigation(listOfVehicles[i]);
            //MapNavigation(listOfVehicles[i]);
        }
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
}
void AEDVInput() {
    int tempCordX, tempCordY,tempDestX, tempDestY;
    printf("Number of AEDV's (1-4): ");
    scanf("%d", &maxAEDV);
    if(maxAEDV < 1) {
        TraceLog(LOG_ERROR, "No AEDV :(");
        exit(-1);
    }

    for (int i = 0; i < maxAEDV; ++i) {
        printf("Input starting location of AEDV [1000%d] x, y: ", i);
        scanf("%d %d", &tempCordX, &tempCordY);
        if(tempCordX < 0 || tempCordX > MAX_COLS || tempCordY < 0 || tempCordY > MAX_ROWS) {
            TraceLog(LOG_ERROR, "Starting Out of Bounds");
            exit(-1);
        }
        printf("Input destination of AEDV [1000%d] x, y: ", i);
        scanf("%d %d", &tempDestX, &tempDestY);
        if(tempDestX < 0 || tempDestX > MAX_COLS || tempDestY < 0 || tempDestY > MAX_ROWS) {
            TraceLog(LOG_ERROR, "Destination Out of Bounds");
            exit(-1);
        }
        InitAEDV(listOfVehicles[i], tempCordX, tempCordY, tempDestX, tempDestY, i);
    }
}
void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    vehicle->EVIN = 10000 + identifierCode;
    vehicle->drawSize = (Vector2) {cellWidth, cellHeight};
    vehicle->position =  (Cord){locationX, locationY};
    vehicle->destination =  (Cord){destinationX, destinationY};
    vehicle->color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
}

void CameraControl() {
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))  {
        Vector2 delta  = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);

        camera.target = Vector2Add(camera.target, delta);
    }
    float wheel = GetMouseWheelMove();
    if(wheel != 0) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;
        const float zoomIncrement = 0.125f;

        camera.zoom += (wheel*zoomIncrement);
        if(camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
    }
}
#ifdef EVERYTHINGINMAIN
void DrawMap(Tile tile) {
    int CURR_COL = tile.location.x;
    int CURR_ROW = tile.location.y;
    if(dynamicMap[CURR_COL][CURR_ROW].Type == BUILDING) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLUE);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == JUNCTION) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, DARKGRAY);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == AVENUE_N) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, RED);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == AVENUE_S) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, ORANGE);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == STREET_E) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BROWN);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == STREET_W) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, VIOLET);
    }
    else{ //Normal Road Section
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, GRAY);
    }
    //Add gridlines for readability
    DrawRectangleLines(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLACK);
}

void StreetMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == EAST) {
        vehicle->position.x += 1;
    }
    else vehicle->position.x -= 1;
}

void AvenueMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == SOUTH) {
        vehicle->position.y += 1;
    }
    else vehicle->position.y -= 1;
}

//NOTE
//Most likely some errors from me swapping destination and x. Should check those first when debugging
void MapNavigation(AEDV * vehicle) {
    //Delivering to an Avenue
    if(dynamicMap[vehicle->destination.x][vehicle->destination.y].Type == AVENUE) {
        //TODO this is the state that should take user input
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);

            return;
        }
        //at the wrong x, move in the direction that decreases distance
        else if((dynamicMap[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.x != vehicle->destination.x) || dynamicMap[vehicle->destination.x][vehicle->position.y].Type == STREET) {
            if(vehicle->destination.x > vehicle->position.x) {
                StreetMovement(vehicle, EAST);
            }
            else if (vehicle->destination.x < vehicle->position.x) StreetMovement(vehicle, WEST);
        }
        //at a junction with the correct x value, now move north or south to decrease distance
        else{
            if(vehicle->destination.y > vehicle->position.y) {
                AvenueMovement(vehicle, SOUTH);
            }
            else if (vehicle->destination.y < vehicle->position.y)AvenueMovement(vehicle, NORTH);
            //If we've made it to our destination we can set our state to be unloading(idle)
            if(vehicle->position.y == vehicle->destination.y) {
                vehicle->currStatus = UNLOADING;
            }
        }
    }
    //Delivering to a street
    else {
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
            /*
             * GetNewAddress()
             *      OUPUT "Enter New Destination for AEDV [ EVIN ]"
             *      READ destination.x destination.y
             *      SET status = active or whatever
             *
             */
            return;
        }
        //If you're at a junction and not at the right street height, adjust to correct height
       if((dynamicMap[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.y != vehicle->destination.y) || dynamicMap[vehicle->position.x][vehicle->position.y].Type == AVENUE ) {
           if (vehicle->destination.y > vehicle->position.y) {
               AvenueMovement(vehicle, SOUTH);
           } else if (vehicle->destination.y < vehicle->position.y) AvenueMovement(vehicle, NORTH);
       }

       //At the correct height now pick a direction to go to either east or west (depend on street direction)
       else{
           if(vehicle->destination.x > vehicle->position.x) {
               StreetMovement(vehicle, EAST);
           }
           else if (vehicle->destination.x < vehicle->position.x) StreetMovement(vehicle, WEST);
           //If we've made it to our destination we can set our state to be unloading(idle)
           if(vehicle->position.x == vehicle->destination.x) {
               vehicle->currStatus = UNLOADING;
           }
       }
    }
}
void InitTiles() {
    allocDynamicMap();

    int firstStreetDirction, firstAvenueDirection;
    printf("Input the direction of the first street (1 for E, 2 for W): ");
    scanf("%d", &firstStreetDirction);

    printf("Input the direction of the first avenue: (4 for N, 5 for S): ");
    scanf("%d", &firstAvenueDirection);

    for (int i = 0; i < MAX_COLS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            setPerimeterRoads(i, j);
        }
    }
    for (int i = 1; i < MAX_COLS - 1; ++i) {
        for (int j = 1; j < MAX_ROWS - 1; ++j) {
            setInternalTiles(i, j, firstStreetDirction, firstAvenueDirection);
        }
    }
}

void setInternalTiles(int i, int j, int firstStreetDirection, int firstAvenueDirection) {
    dynamicMap[i][j] = (Tile) {.location.x = i, .location.y  = j};
    //Multi Directional Roads
    if(i % 4 == 0 && j % 4 == 0) {
        dynamicMap[i][j].Type = JUNCTION;
    }
    else if((j % 4 == 0 && j % 8 != 0) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = firstStreetDirection;
    }
    else if((j % 8 == 0) && dynamicMap[i][j].Type != JUNCTION) {
        firstStreetDirection == STREET_E ? (dynamicMap[i][j].Type = firstStreetDirection+1) :( dynamicMap[i][j].Type = firstStreetDirection -1);
    }
    else if((i % 4 == 0 && i % 8 != 0) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = firstAvenueDirection;
    }
    else if((i % 8 == 0) && dynamicMap[i][j].Type != JUNCTION) {
        firstAvenueDirection == AVENUE_N ? (dynamicMap[i][j].Type = firstAvenueDirection+1) :( dynamicMap[i][j].Type = firstAvenueDirection -1);
    }
    else {
        dynamicMap[i][j].Type =  BUILDING;
    }
}

void setPerimeterRoads(int i, int j) {
    dynamicMap[i][j] = (Tile) {.location.x = i, .location.y  = j};
    //Multi Directional Roads
    if(i % 4 == 0 && j % 4 == 0) {
        dynamicMap[i][j].Type = JUNCTION;
    }
    else if((i == 0 || i == MAX_COLS) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = AVENUE;
    }
    else if((j == 0 || j == MAX_ROWS) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = STREET;
    }
}
#endif
