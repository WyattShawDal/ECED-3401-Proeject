/*
 *Author: Wyatt on 
 *Created on: 2023-11-12
*/

#ifndef EXAMPLE_NAVIGATIONS_H
#define EXAMPLE_NAVIGATIONS_H
#include "dependencies.h"
/**
 * @brief allows user to update AEDV commands
 */
void AssignNewOrders();
/**
 * @brief Calculates and then updates the position of the AEDV in the map
 * @param vehicle AEDV to move around
 * @note algorithm works on a two-way rule free road system
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
/**
 * @brief One way pathing navigation algorithm
 *
 * @param vehicle
 *
 * @note Algorithm is dumb, only checking one tile ahead and without considering rules of the road. Ommitted from Task 5 Code
 */
void OneWayNavigation(AEDV * vehicle);
/**
 * @brief function used for one way navigation, determines whether the input direction is able to be travelled
 * @param vehicle
 * @param direction
 * @note used in one way, omitted for task 5
 * @return True or False
 */
bool IsValidDirection(AEDV* vehicle, int direction);
/**
 *
 * @param currentTile
 * @param vehicle
 * @note used in one way, omitted for task 5
 */
void StreetMoves(int currentTile, AEDV *vehicle);
/**
 *
 * @param vehicle
 * @param destinationTile
 * @param desiredJunctions
 * @note used in one way omitted in task 5
 */
void FindAdjacentJunctions(AEDV* vehicle, int destinationTile, Cord* desiredJunctions);
/**
 *
 * @param vehicle
 * @param desiredJunctions
 * @param destinationTile
 * @note utilized in one way, omitted in task 5
 * @return
 */
bool AtDesiredJunction(AEDV* vehicle, Cord* desiredJunctions, int destinationTile);
/**
 * @brief uses generation rules to determine if the tile chosen is a deliverable location
 * @param col
 * @param row
 * @note function uses col and row instead of a tile type because it can be called before tiles have been generated
 * @return
 */
bool isValidDestination(int col, int row);


#endif //EXAMPLE_NAVIGATIONS_H
