/**
* Author: Wyatt Shaw & Cameron Archibald
 * Date October 31st 2023
 * Module Info: Contains function definitions for initalization and construction of the map.
*/
#include "dependencies.h"

char *NESWArray[3][3] = {"NW", "W", "SW", "N", "", "S", "NE", "E", "SE"};

void allocDynamicMap() {
    dynamicMap = (Tile**) malloc(MAX_COLS * sizeof (Tile));
    if(dynamicMap == NULL){
        TraceLog(LOG_ERROR, "ERROR ALLOCATING ROW MEMORY FOR MAP");
        //If heap memory is exceeded, trying to rectify garbage is more trouble than it's worth. Exit
        exit (-1);
    }
    for (int i = 0; i < MAX_COLS; ++i) {
        dynamicMap[i] = (Tile*) malloc(MAX_ROWS * sizeof (Tile));
        if(dynamicMap[i] == NULL) {
            TraceLog(LOG_ERROR, "ERROR ALLOCATING ROW MEMORY FOR MAP");
            //If heap memory is exceeded, trying to rectify garbage is more trouble than it's worth. Exit
            exit (-1);
        }
    }
}

void DrawMap(Tile tile) {
    int CURR_COL = tile.location.x;
    int CURR_ROW = tile.location.y;
    int currentType = dynamicMap[tile.location.x][tile.location.y].Type;

    //Switch case is used to fill in the colours of the different squares, and add tile information characters (N, NE, E etc.)
    switch(currentType) {
        case BUILDING:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, BLUE);
            DrawText(TextFormat("%s", NESWArray[(CURR_COL % 4)-1][(CURR_ROW % 4)-1]), CURR_COL * cellWidth + 12, CURR_ROW * cellHeight +12, DEFAULTFONTSIZE, WHITE);
            break;
        case JUNCTION:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, DARKGRAY);
            break;
        case AVENUE_N:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, RED);
            if(CURR_ROW == 1) {
                DrawText(TextFormat("N"), CURR_COL * cellWidth + 16, 15, DEFAULTFONTSIZE, WHITE);
            }
            break;
        case AVENUE_S:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, ORANGE);
            if(CURR_ROW == 1) {
                DrawText(TextFormat("S"), CURR_COL * cellWidth + 16, 15, DEFAULTFONTSIZE, WHITE);
            }
            break;
        case STREET_E:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, PINK);
            if(CURR_COL == 1) {
                DrawText(TextFormat("E"), 15, CURR_ROW * cellHeight + 16, DEFAULTFONTSIZE, WHITE);
            }
            break;
        case STREET_W:
            DrawRectangle(CURR_COL * cellWidth, CURR_ROW * cellHeight, cellWidth, cellHeight, VIOLET);
            if(CURR_COL == 1) {
                DrawText(TextFormat("W"), 15, CURR_ROW * cellHeight + 16, DEFAULTFONTSIZE, WHITE);
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
    int firstStreetDirection, firstAvenueDirection;
    ReadBuildFile(&firstStreetDirection, &firstAvenueDirection);


    allocDynamicMap();
//    printf("Input the direction of the first street (1 for E, 2 for W): ");
//    scanf("%d", &firstStreetDirection);
//
//    printf("Input the direction of the first avenue: (4 for N, 5 for S): ");
//    scanf("%d", &firstAvenueDirection);

    //Set outside streets and their available directions
    for(int i = 0;i < MAX_COLS;i++) {
        setPerimeterRoads(i,0);
        setDirection(i,0);
        setPerimeterRoads(i, MAX_ROWS - 1);
        setDirection(i, MAX_ROWS - 1);
    }

    //Set outside avenues and their available directions
    for(int j = 1;j < MAX_ROWS -1;j++) {
        setPerimeterRoads(0, j);
        setDirection(0,j);
        setPerimeterRoads(MAX_COLS - 1,j);
        setDirection(MAX_COLS - 1, j);
    }

    //Set the internal tiles and their directions
    for (int i = 1; i < MAX_COLS - 1; ++i) {
        for (int j = 1; j < MAX_ROWS - 1; ++j) {
            setInternalTiles(i, j, firstStreetDirection, firstAvenueDirection);
            setDirection(i,j);
        }
    }

    //Set the direction of all junction tiles
    for(int i = 0;i < MAX_COLS;i += 4) {
        for(int j = 0;j < MAX_ROWS;j += 4) {
            setJunctionDirection(i,j,firstStreetDirection,firstAvenueDirection);
        }
    }

//#define directionTest1 //Prints every tile's direction
#ifdef directionTest1
    for(int i = 0;i < MAX_COLS;i++) {
        for(int j = 0;j < MAX_ROWS;j++) {
            printf("S: %d N: %d E: %d W: %d\n", dynamicMap[i][j].validDirection[SOUTH], dynamicMap[i][j].validDirection[NORTH], dynamicMap[i][j].validDirection[EAST], dynamicMap[i][j].validDirection[WEST]);
        }
        printf("New row\n");
    }
#endif

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
        //printf("SOUTH: %d NORTH: %d EAST: %d WEST: %d", dynamicMap[i][j].validDirection[SOUTH], dynamicMap[i][j].validDirection[NORTH], dynamicMap[i][j].validDirection[EAST], dynamicMap[i][j].validDirection[WEST]);
    }
}

void setDirection(int i, int j) {
    switch(dynamicMap[i][j].Type) {
        case STREET:
            dynamicMap[i][j].validDirection[EAST] = true;
            dynamicMap[i][j].validDirection[WEST] = true;
            break;
        case STREET_E:
            dynamicMap[i][j].validDirection[EAST] = true;
            break;
        case STREET_W:
            dynamicMap[i][j].validDirection[WEST] = true;
            break;
        case AVENUE:
            dynamicMap[i][j].validDirection[NORTH] = true;
            dynamicMap[i][j].validDirection[SOUTH] = true;
            break;
        case AVENUE_N:
            dynamicMap[i][j].validDirection[NORTH] = true;
            break;
        case AVENUE_S:
            dynamicMap[i][j].validDirection[SOUTH] = true;
            break;
        case BUILDING:
            //Do nothing, no movement instructions from a building
        case JUNCTION:
            //Handled in setJunctionDirection
            break;
    };
}

void setJunctionDirection(int i, int j, int firstStreetDirection, int firstAvenueDirection) {
    if((i == 0) || (i == MAX_COLS - 1)) { //Outside avenues, set NS movement
        if(j != 0) dynamicMap[i][j].validDirection[NORTH] = true;
        if(j != MAX_ROWS - 1) dynamicMap[i][j].validDirection[SOUTH] = true;
    }
    if((j == 0) || (j == MAX_ROWS - 1)) { //Outside streets, set EW movement
        if(i != 0) dynamicMap[i][j].validDirection[WEST] = true;
        if(i != MAX_COLS - 1) dynamicMap[i][j].validDirection[EAST] = true;
    }

    //Set EW movement
    if((j % 8 != 0) && NOT_TWO_WAY_ROW()) { //Odd numbered inside streets (located in j = 4, 12, 20..., not divisible by 8)
        if(firstStreetDirection == STREET_E && (i != MAX_COLS - 1)) //If first street E, odd streets point E
            dynamicMap[i][j].validDirection[EAST] = true; //Set E unless at right most avenue
        else if(firstStreetDirection == STREET_W && (i != 0)) //If first street W, odd streets point W
            dynamicMap[i][j].validDirection[WEST] = true; //Set W unless at left most avenue
    }
    else if(j % 8 == 0 && NOT_TWO_WAY_ROW()) { //Even numbered inside streets (located in j = 8, 16, 24..., divisible by 8)
        if(firstStreetDirection == STREET_E && (i != 0))
            dynamicMap[i][j].validDirection[WEST] = true;
        else if(firstStreetDirection == STREET_W && (i != MAX_COLS - 1))
            dynamicMap[i][j].validDirection[EAST] = true;
    }

    //Set NS movement
    if((i % 8 != 0) && NOT_TWO_WAY_COL()) { //Odd numbered inside avenues
        if(firstAvenueDirection == AVENUE_S && (j != MAX_ROWS - 1))
            dynamicMap[i][j].validDirection[SOUTH] = true;
        else if(firstAvenueDirection == AVENUE_N && (j != 0))
            dynamicMap[i][j].validDirection[NORTH] = true;
    }
    else if(i % 8 == 0 && NOT_TWO_WAY_COL()) { //Even numbered inside avenues
        if(firstAvenueDirection == AVENUE_S && (j != 0))
            dynamicMap[i][j].validDirection[NORTH] = true;
        else if(firstAvenueDirection == AVENUE_N && (j != MAX_ROWS - 1))
            dynamicMap[i][j].validDirection[SOUTH] = true;
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
void UpdateMap() {
    for (int CURR_COL = 0; CURR_COL < MAX_COLS; ++CURR_COL) {
        for(int CURR_ROW = 0; CURR_ROW < MAX_ROWS; ++CURR_ROW) {
            //Draws the Row Counter to the left of the map
            if(CURR_COL == 0) {
                DrawText(TextFormat("%d", CURR_ROW), -30, CURR_ROW * cellHeight +12, DEFAULTFONTSIZE, WHITE);
            }
            //Draws the Column Counter above the map
            if(CURR_ROW == 0) {
                DrawText(TextFormat("%d", CURR_COL), CURR_COL * cellWidth +12, -30, DEFAULTFONTSIZE, WHITE);
            }
            //Fills in the given cell
            DrawMap(dynamicMap[CURR_COL][CURR_ROW]);
        }
    }
}
