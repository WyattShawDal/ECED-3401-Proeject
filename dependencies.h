//
// Created by wyatt on 2023-10-18.
//

#ifndef EXAMPLE_DEPENDENCIES_H
#define EXAMPLE_DEPENDENCIES_H

#define TWOWAY //Allows for two-way navigation
//#define DEBUG //Allows for diagnostic printing
//#define OLD //Functions from one-way experimenting
//#define OLD1 //Fixed readability and efficiency by Cameron on 31st of October

#include "stdio.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "structs.h"
#include <ctype.h>

#include <malloc.h>


#define SOUTH 0
#define NORTH 1
#define EAST 2
#define WEST 3

extern int MAX_COLS, MAX_ROWS, cellWidth, cellHeight;
extern Tile** dynamicMap;

/**
 * @brief Function draws the map and then calls the navigations function
 */
void UpdateDrawFrame(void);     // Update and Draw one frame
/**
 * @brief Draws the map of the city using the initalized Tile array
 * @param tile Struct containing Tile type information
 */
void DrawMap(Tile tile);
/**
 * @brief Calculates and then updates the positions of the AEDV's in the map
 * @param vehicle AEDV to move around, will be updated to an array of AEDV's
 */
void MapNavigation(AEDV *vehicle);
/**
 * @brief Calculates North/South Movement
 * @param vehicle AEDV to move
 * @param direction North/South
 */
void AvenueMovement(AEDV *vehicle, int direction);
/**
 * @brief Calculates East/West Movement
 * @param vehicle AEDV to move
 * @param direction Esat/West
 */
void StreetMovement(AEDV *vehicle, int direction);
void InitTiles();
void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode);
void CameraControl();
void setPerimeterRoads(int i, int j);
void allocDynamicMap();
void AEDVInput();
void setInternalTiles(int i, int j, int firstStreetDirection, int firstAvenueDirection);
void OneWayNavigation(AEDV * vehicle);
bool IsValidDirection(AEDV* vehicle, int direction);
void StreetMoves(int currentTile, AEDV *vehicle);
void FindAdjacentJunctions(AEDV* vehicle, int destinationTile, Cord* desiredJunctions);
bool AtDesiredJunction(AEDV* vehicle, Cord* desiredJunctions, int destinationTile);
bool isValidDestination(int col, int row);

#endif //EXAMPLE_DEPENDENCIES_H
