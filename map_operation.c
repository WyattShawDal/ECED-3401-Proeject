/**
* Author: Wyatt Shaw & Cameron Archibald
 * Date 2023-10-30
 * Module Info: Contains function definitions for initalization and construction of the map.
*/
#include "dependencies.h"

char *NESWArray[3][3] = {"NW", "W", "SW", "N", "", "S", "NE", "E", "SE"};

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
    int currentType = dynamicMap[tile.location.x][tile.location.y].Type;

    switch(currentType) {
        case BUILDING:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLUE);
            DrawText(TextFormat("%s", NESWArray[(CURR_COL % 4)-1][(CURR_ROW % 4)-1]), CURR_COL * cellWidth + 12, CURR_ROW * cellHeight +12, 20, WHITE);
            break;
        case JUNCTION:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, DARKGRAY);
            break;
        case AVENUE_N:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, RED);
            if(CURR_ROW == 1) {
                DrawText(TextFormat("N"), CURR_COL * cellWidth + 16, 15, 20, WHITE);
            }
            break;
        case AVENUE_S:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, ORANGE);
            if(CURR_ROW == 1) {
                DrawText(TextFormat("S"), CURR_COL * cellWidth + 16, 15, 20, WHITE);
            }
            break;
        case STREET_E:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, PINK);
            if(CURR_COL == 1) {
                DrawText(TextFormat("E"), 15, CURR_ROW * cellHeight + 16, 20, WHITE);
            }
            break;
        case STREET_W:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, VIOLET);
            if(CURR_COL == 1) {
                DrawText(TextFormat("W"), 15, CURR_ROW * cellHeight + 16, 20, WHITE);
            }
            break;
        case AVENUE:
        case STREET:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, GRAY);
            break;
        default:
            break;
    }

    //Add gridlines for readability
    DrawRectangleLines(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLACK);

#ifdef OLD

    if(dynamicMap[CURR_COL][CURR_ROW].Type == BUILDING) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLUE);
        DrawText(TextFormat("%s", NESWArray[(CURR_COL % 4)-1][(CURR_ROW % 4)-1]), CURR_COL * cellWidth + 12, CURR_ROW * cellHeight +12, 20, WHITE);
        //FillTiles(), to draw NW, N, NE ...
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == JUNCTION) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, DARKGRAY);
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == AVENUE_N) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, RED);
        if(CURR_ROW == 1) {
            DrawText(TextFormat("N"), CURR_COL * cellWidth + 16, 15, 20, WHITE);
        }
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == AVENUE_S) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, ORANGE);
        if(CURR_ROW == 1) {
            DrawText(TextFormat("S"), CURR_COL * cellWidth + 16, 15, 20, WHITE);
        }
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == STREET_E) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, PINK);
        if(CURR_COL == 1) {
            DrawText(TextFormat("E"), 15, CURR_ROW * cellHeight + 16, 20, WHITE);
        }
    }
    else if(dynamicMap[CURR_COL][CURR_ROW].Type == STREET_W) {
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, VIOLET);
        if(CURR_COL == 1) {
            DrawText(TextFormat("W"), 15, CURR_ROW * cellHeight + 16, 20, WHITE);
        }
    }
    else{ //Normal Road Section
        DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, GRAY);
    }
    //Add gridlines for readability
    DrawRectangleLines(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLACK);
#endif
}
void InitTiles() {
    allocDynamicMap();

    int firstStreetDirection, firstAvenueDirection;
    printf("Input the direction of the first street (1 for E, 2 for W): ");
    scanf("%d", &firstStreetDirection);

    printf("Input the direction of the first avenue: (4 for N, 5 for S): ");
    scanf("%d", &firstAvenueDirection);

    for (int i = 0; i < MAX_COLS; ++i) {
        for (int j = 0; j < MAX_ROWS; ++j) {
            setPerimeterRoads(i, j);
        }
    }
    for (int i = 1; i < MAX_COLS - 1; ++i) {
        for (int j = 1; j < MAX_ROWS - 1; ++j) {
            setInternalTiles(i, j, firstStreetDirection, firstAvenueDirection);
        }
    }

#ifdef DEBUG //prints map of tile types for diagnostic
    for(int i = 0;i < MAX_COLS;i++) {
        for(int j = 0;j < MAX_ROWS;j++) {
            printf("%d ", dynamicMap[j][i].Type);
        }
        printf("\n");
    }
#endif
}

void setInternalTiles(int i, int j, int firstStreetDirection, int firstAvenueDirection) {
    dynamicMap[i][j] = (Tile) {.location.x = i, .location.y  = j};
    //Multi Directional Roads
    if(i % 4 == 0 && j % 4 == 0) {
        dynamicMap[i][j].Type = JUNCTION;
    }
    else if((j % 4 == 0 && j % 8 != 0) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = firstStreetDirection;
        //dynamicMap[i][j].Type = STREET;
    }
    else if((j % 8 == 0) && dynamicMap[i][j].Type != JUNCTION) {
        firstStreetDirection == STREET_E ? (dynamicMap[i][j].Type = firstStreetDirection+1) :( dynamicMap[i][j].Type = firstStreetDirection -1);
        //firstStreetDirection == STREET_E ? (dynamicMap[i][j].Type = STREET) :( dynamicMap[i][j].Type =STREET);

    }
    else if((i % 4 == 0 && i % 8 != 0) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = firstAvenueDirection;
        //dynamicMap[i][j].Type = AVENUE;
    }
    else if((i % 8 == 0) && dynamicMap[i][j].Type != JUNCTION) {
        firstAvenueDirection == AVENUE_N ? (dynamicMap[i][j].Type = firstAvenueDirection+1) :( dynamicMap[i][j].Type = firstAvenueDirection -1);
        //firstAvenueDirection == AVENUE_N ? (dynamicMap[i][j].Type = AVENUE) :( dynamicMap[i][j].Type = AVENUE);

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
    else if((i == 0 || i == MAX_COLS - 1) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = AVENUE;
    }
    else if((j == 0 || j == MAX_ROWS - 1) && dynamicMap[i][j].Type != JUNCTION) {
        dynamicMap[i][j].Type = STREET;
    }
}

bool isValidDestination(int col, int row) {
    bool isValid = true;
    /* List of conditions you can't navigate to
     * 0 < cols <= max
     * 0 < rows <= max
     * junctions
     * buildings
   */
    if((col >= MAX_COLS) ||
    (col < 0) ||
    (row >= MAX_ROWS)||
    (row < 0) ||
    ((col % 4) == 0) && ((row % 4) == 0) ||
    ((col % 4) != 0) && ((row % 4) != 0))
        isValid = false;
    //Last two could be XOR
    return isValid;
}
