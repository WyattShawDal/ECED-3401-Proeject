/*
 *Author: Wyatt on 
 *Created on: 2023-11-12
*/

#ifndef EXAMPLE_MAP_OPERATION_H
#define EXAMPLE_MAP_OPERATION_H
#include "dependencies.h"

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
 * @brief allows for moving and resizing of the camera
 */
void UpdateMap();


void DrawVehicleMovements(AEDVNode* currentVehicle);
#endif //EXAMPLE_MAP_OPERATION_H
