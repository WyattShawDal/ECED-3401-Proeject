//
// Created by wyatt on 2023-10-18.
//

#ifndef EXAMPLE_DEPENDENCIES_H
#define EXAMPLE_DEPENDENCIES_H
#include "stdio.h"
#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "structs.h"

#define SOUTH 0
#define NORTH 1
#define EAST 0
#define WEST 1


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


#endif //EXAMPLE_DEPENDENCIES_H
