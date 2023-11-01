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
Camera2D camera = { 0 };


int maxAEDV, MAX_COLS, MAX_ROWS, cellWidth, cellHeight;

char defaultTest;

static AEDV wAEDV;
static AEDV xAEDV;
static AEDV yAEDV;
static AEDV zAEDV;


AEDV *listOfVehicles[4] = {&wAEDV, &xAEDV, &yAEDV, &zAEDV};

Tile **dynamicMap;

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    //Allows Window Resizing (Doesn't affect the Grid Dimensions)
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    //Disables INFO output in console window at startup
    SetTraceLogLevel(LOG_ERROR);

    //Camera Settings to show whole picture
    camera.zoom = 0.94f;
    camera.target = (Vector2) {.x = -45, .y = -45};
    
    do {
        printf("Default Test? (Y or N):");
        scanf("%c", &defaultTest);

        if(tolower(defaultTest) == 'y') {
            maxAEDV = 1;
            MAX_COLS = 21;
            MAX_ROWS = 21;
            cellHeight = screenWidth/MAX_COLS;
            cellWidth = screenWidth/MAX_COLS;
            printf("Dimensions of map defaulted to %d columns by %d rows\n", MAX_COLS, MAX_ROWS);
            InitAEDV(listOfVehicles[0], 1, 0, 10, 12, 0);
        }
        else if(tolower(defaultTest) == 'n'){
            printf("Number of buildings horizontally: ");
            scanf("%d",&MAX_COLS);
            printf("Number of buildings vertically: ");
            scanf("%d",&MAX_ROWS);
            printf("You inputted %d x %d buildings\n", MAX_COLS, MAX_ROWS);
            MAX_COLS = MAX_COLS * 4 + 1;
            MAX_ROWS = MAX_ROWS * 4 + 1;
            printf("Equals %d x %d tiles\n", MAX_COLS, MAX_ROWS);
            cellHeight = screenWidth/MAX_COLS;
            cellWidth = screenWidth/MAX_COLS;
            AEDVInput();

        }
    //Stored In while loop to allow for bad inputs to be rectified
    } while((tolower(defaultTest) != 'y') && (tolower(defaultTest) != 'n'));
#ifdef DEBUG //Testing of isValidDestination function

#endif
    int frameTarget = 10;
    InitTiles(); //sets the values for the tiles in the map according the map generation algorithm
    InitWindow(screenWidth, screenHeight, "AEDV Live Map");
    SetTargetFPS(frameTarget);// Set our simulation to run at x frames-per-second

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
    int count = 0;
    //Check the amount of IDLE AEDV's
#define MANUALORDER
#ifdef MANUALORDER
    if(GetKeyPressed() == KEY_F) {
        int AEDVNum, tempDestX, tempDestY, AEDVIndex;
        printf("Enter AEDV EVIN: ");
        scanf("%d", &AEDVNum);
        printf("You entered %d\n", AEDVNum);
        AEDVIndex = AEDVNum - 10000;
        if(AEDVIndex > maxAEDV-1) {
            printf("Invalid AEDV Number (Out of Bounds \n)");
        }
        else {
            printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[AEDVIndex]->EVIN);
            scanf("%d %d", &tempDestX,&tempDestY);
            if(isValidDestination(tempDestX, tempDestY)) {
                listOfVehicles[AEDVIndex]->destination.x = tempDestX;
                listOfVehicles[AEDVIndex]->destination.y = tempDestY;
                listOfVehicles[AEDVIndex]->currStatus = TRANSIT;
                return;
            }
            else
                printf("Invalid Destination, F again to reorder\n");
        }
    }
#endif
#ifdef AUTOORDERS
    while(listOfVehicles[count]->currStatus==UNLOADING) {
        count++;
    }
    for (int i = 0; count == maxAEDV && i < maxAEDV; ++i) {
        int tempDestX, tempDestY, isValid = 1;
        while(isValid != 0) {
            printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[i]->EVIN);
            scanf("%d %d", &tempDestX,&tempDestY);
            if(isValidDestination(tempDestX, tempDestY)) {
                listOfVehicles[i]->destination.x = tempDestX;
                listOfVehicles[i]->destination.y = tempDestY;
                listOfVehicles[i]->currStatus = TRANSIT;
                isValid = 0;
            }
            else
                printf("Invalid destination\n");
        }
    }
#endif
#ifdef OLD1 //Old assignment of new destinations
    //If every AEDV is unlaoding then we can go ahead and input all their new destinations
    for (int i = 0; count == maxAEDV && i < 4; ++i) {
        if(listOfVehicles[i]->currStatus == UNLOADING) {

            printf("Enter New Destination for AEDV[%d]: ", listOfVehicles[i]->EVIN);
            scanf("%d %d", &listOfVehicles[i]->destination.x,&listOfVehicles[i]->destination.y);
            listOfVehicles[i]->currStatus = TRANSIT;
        }
    }
#endif
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

    //begin navigation
    for (int i = 0; i < maxAEDV; ++i) {
       //OneWayNavigation(listOfVehicles[i]);
       MapNavigation(listOfVehicles[i]);
    }


    EndDrawing();
    //----------------------------------------------------------------------------------
}
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


        InitAEDV(listOfVehicles[i], tempCordX, tempCordY, tempDestX, tempDestY, i);
    }
}

void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode) {
    vehicle->EVIN = 10000 + identifierCode;
    vehicle->drawSize = (Vector2) {cellWidth, cellHeight};
    vehicle->position =  (Cord){locationX, locationY};
    vehicle->destination =  (Cord){destinationX, destinationY};
    vehicle->color = (Color) {GetRandomValue(0, 255), GetRandomValue(0, 127), GetRandomValue(0, 127), 255};
    vehicle->currStatus = TRANSIT;
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
#ifdef OLD
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
