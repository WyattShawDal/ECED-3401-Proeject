/*
 *Author: Wyatt on 
 *Created on: 2023-11-12
*/

#ifndef EXAMPLE_DRAWING_H
#define EXAMPLE_DRAWING_H
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
void CameraControl();
void UpdateMap();
#endif //EXAMPLE_DRAWING_H
