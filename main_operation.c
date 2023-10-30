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
#define MAX_COLS 21
#define MAX_ROWS 21
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 1000;
const int screenHeight = 1000;
Camera2D camera = { 0 };
const int cellWidth = screenWidth/MAX_COLS;
const int cellHeight = screenWidth/MAX_COLS;
bool _showLocationData = false;
bool atDestination = false;

static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;
AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};

Tile map[MAX_COLS][MAX_ROWS];

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------


//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags()

    camera.zoom = 1.0f;
    //TODO it kind of works but random needs some help
    //Loop through the AEDV's generating some initial conditions. I'm not yet sure where in the
    //Control flow we want to have instruction generation happening, + user input stuff
    for (int i = 0; i < 4; ++i) {
        InitAEDV(listOfVehicles[i], 4 + i * 4, i * 4, 10, 12, i);
    }
    InitWindow(screenWidth, screenHeight, "AEDV Live Map");

    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    SetTargetFPS(5);// Set our simulation to run at x frames-per-second
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
            DrawMap(map[CURR_COL][CURR_ROW]);
        }
    }
    //If the destination value is invalid don't bother attempting to nav
    if(map[(int)wAEDV.destination.x/cellWidth][(int)wAEDV.destination.y/cellHeight].Type == BUILDING) {
        DrawText(TextFormat("Invalid Address supplied"), 0, -60, 20, RED);
    }
    //begin navigation
    else{
        for (int i = 0; i < 4; ++i) {
            MapNavigation(listOfVehicles[i]);
        }
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
}
void DrawMap(Tile tile) {
    int CURR_COL = tile.location.x;
    int CURR_ROW = tile.location.y;
    if(map[CURR_COL][CURR_ROW].Type == BUILDING) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLUE);
    }
    else if(map[CURR_COL][CURR_ROW].Type == JUNCTION) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, DARKGRAY);
    }
    else{ //Normal Road Section
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, GRAY);
    }
    //Add gridlines for readability
    DrawRectangleLines(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLACK);
}
//TODO Will need to implement one-way movement for these two functions
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
    if(map[vehicle->destination.x][vehicle->destination.y].Type == AVENUE) {
        //TODO this is the state that should take user input
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);

            return;
        }
        //at the wrong x, move in the direction that decreases distance
        else if((map[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.x != vehicle->destination.x) || map[vehicle->destination.x][vehicle->position.y].Type == STREET) {
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
       if((map[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.y != vehicle->destination.y) || map[vehicle->position.x][vehicle->position.y].Type == AVENUE ) {
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
    for (int i = 0; i < MAX_COLS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            map[i][j] = (Tile) {.location.x = i, .location.y  = j};
            if(i % 4 == 0 && j % 4 == 0 && (i + j) != 0) {
                map[i][j].Type = JUNCTION;
            }
            else if(i % 4 == 0 && map[i][j].Type != JUNCTION) {
                map[i][j].Type = AVENUE;
            }
            else if(j % 4 == 0 && map[i][j].Type != JUNCTION) {
                map[i][j].Type = STREET;
            }
            else {
                map[i][j].Type = BUILDING;
            }
        }
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
