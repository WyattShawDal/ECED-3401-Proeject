/**
* Author: Wyatt Shaw & Cameron Archibald
 * Date 2023-10-30
 * Module Info: Contains function definitions for initalization and construction of the map.
*/
#include "dependencies.h"



void allocDynamicMap() {
    dynamicMap = (Tile**) malloc(MAX_COLS * sizeof (Tile));
    if(dynamicMap == NULL){
        TraceLog(LOG_ERROR, "ERROR ALLOCATING ROW MEMORY FOR MAP");
        exit (-1);
    }
    for (int i = 0; i < MAX_COLS; ++i) {
        dynamicMap[i] = (Tile*) malloc(MAX_ROWS * sizeof (Tile));
        if(dynamicMap[i] == NULL) {
            TraceLog(LOG_ERROR, "ERROR ALLOCATING ROW MEMORY FOR MAP");
            exit (-1);
        }
    }
}
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

