/*******************************************************************************************
*  Author: Wyatt Shaw and Cameron Archibald
*  Date: October 31 2023
*  Module Info: The following module implements 'dumb' AEDV movement, allowing up to four AEDVs to navigate to given destination points
*  Dependency Info: In order to handle graphical output, the module utilizes Raylib.h
*  https://github.com/raysan5/raylib
*   Copyright (c) 2015 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "dependencies.h"
#include "structs.h"
#define gridDrawing

//----------------------------------------------------------------------------------
// Const Defines
//----------------------------------------------------------------------------------
#define MAX_COLS 21
#define MAX_ROWS 21
//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const int screenWidth = 900;
const int screenHeight = 900;
Camera2D camera = { 0 };
const int cellWidth = screenWidth / MAX_COLS;
const int cellHeight = screenHeight / MAX_ROWS;
bool _showLocationData = false;
bool atDestination = false;


static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;
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
    camera.zoom = 1.0f;
    InitWindow(screenWidth, screenHeight, "AEDV Live Map");
    InitAEDV(&wAEDV, 18, 0, 0, 16); //sets the conditions for spawning an AEDV
    InitAEDV(&xAEDV, 0, 0, 16, 14); //sets the conditions for spawning an AEDV
   // InitAEDV(&yAEDV, 0, 0, 16, 14); //sets the conditions for spawning an AEDV
   // InitAEDV(&zAEDV, 0, 0, 16, 14); //sets the conditions for spawning an AEDV

    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    SetTargetFPS(10);   // Set our simulation to run at 10 frames-per-second
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
    ClearBackground(GRAY);
    BeginMode2D(camera);
    //Draw the map into the window using tile data generated in main
    for (int CURR_COL = 0; CURR_COL < MAX_COLS; ++CURR_COL) {
        for(int CURR_ROW = 0; CURR_ROW < MAX_ROWS; ++CURR_ROW) {
            drawMap(map[CURR_COL][CURR_ROW]);
        }
    }
    //If the destination value is invalid don't bother attempting to nav
    if(map[(int)wAEDV.destination.x/cellWidth][(int)wAEDV.destination.y/cellHeight].Type == BUILDING) {
        DrawText(TextFormat("Invalid Address supplied"), 0, -60, 20, RED);
    }
    //begin navigation
    else{
        map_navigation(&wAEDV);
        map_navigation(&xAEDV);
        //map_navigation(&yAEDV);
        //map_navigation(&zAEDV);
    }
    EndDrawing();
    //----------------------------------------------------------------------------------
}
void drawMap(Tile tile) {
    int CURR_COL = tile.i;
    int CURR_ROW = tile.j;
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
void street_movement(AEDV *vehicle, int direction) {
    DrawRectangleV(vehicle->location, vehicle->drawSize, vehicle->color);
    if(direction == EAST) {
        vehicle->location.x += cellWidth;
    }
    else vehicle->location.x -= cellWidth;
}

void avenue_movement(AEDV *vehicle, int direction) {
    DrawRectangleV(vehicle->location, vehicle->drawSize, vehicle->color);
    if(direction == SOUTH) {
        vehicle->location.y += cellWidth;
    }
    else vehicle->location.y -= cellWidth;
}

void map_navigation(AEDV * vehicle) {
    // convert location data into a clean integer
    int xCell = vehicle->location.x/cellWidth;
    int yCell = vehicle->location.y/cellHeight;
    int destX = vehicle->destination.x/cellWidth;
    int destY = vehicle->destination.y/cellHeight;
    //Delivering to an Avenue
    if(map[destX][destY].Type == AVENUE) {
        //TODO this is the state that should take user input
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV(vehicle->location, vehicle->drawSize, GREEN);
            return;
        }
        //at the wrong x, move in the direction that decreases distance
        if(map[xCell][yCell].Type == JUNCTION && xCell != destX) {
            xCell = vehicle->location.x/cellHeight;
            if(destX > xCell) {
                street_movement(vehicle, EAST);
            }
            else if (destX < xCell) street_movement(vehicle, WEST);
        }
        //on a street, continue moving until the next junction
        else if (map[xCell][yCell].Type == STREET) {
            xCell = vehicle->location.x/cellHeight;
            if(destX >= xCell) {
                street_movement(vehicle, EAST);
            }
            else if (destX < xCell) street_movement(vehicle, WEST);

        }
        //at a junction with the correct x value, now move north or south to decrease distance
        else{
            yCell = vehicle->location.y/cellWidth;
            if(destY > yCell) {
                avenue_movement(vehicle, SOUTH);
            }
            else if (destY < yCell)avenue_movement(vehicle, NORTH);
            //If we've made it to our destination we can set our state to be unloading(idle)
            if(yCell == destY) {
                vehicle->currStatus = UNLOADING;
            }
        }
    }
    //Delivering to a street
    else {
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV(vehicle->location, vehicle->drawSize, GREEN);
            return;
        }
        //If you're at a junction and not at the right street height, adjust to correct height
       if(map[xCell][yCell].Type == JUNCTION && yCell != destY) {
           yCell = vehicle->location.y/cellHeight;
           if(destY > yCell) {
               avenue_movement(vehicle, SOUTH);
           }
           else if (destY < yCell) avenue_movement(vehicle, NORTH);
       }
       //This will need to change depending on avenue direction, but continue in the direction you started
       else if (map[xCell][yCell].Type == AVENUE) {
           yCell = vehicle->location.y/cellHeight;
           if(destY > yCell) {
               avenue_movement(vehicle, SOUTH);
           }
           else if (destY < yCell) avenue_movement(vehicle, NORTH);
       }
       //At the correct height now pick a direction to go to either east or west (depend on street direction)
       else{
           xCell = vehicle->location.x/cellWidth;
           if(destX > xCell) {
               street_movement(vehicle, EAST);
           }
           else if (destX < xCell) street_movement(vehicle, WEST);
           //If we've made it to our destination we can set our state to be unloading(idle)
           if(xCell == destX) {
               vehicle->currStatus = UNLOADING;
           }
       }
    }
}
void InitTiles() {
    for (int i = 0; i < MAX_COLS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            map[i][j] = (Tile) {.i = i, .j  = j};
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
void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY) {
    vehicle->drawSize = (Vector2) {cellWidth, cellHeight};
    vehicle->location = (Vector2) {locationX*cellWidth, locationY*cellHeight};
    vehicle->destination = (Vector2) {destinationX*cellWidth, destinationY*cellHeight};
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
