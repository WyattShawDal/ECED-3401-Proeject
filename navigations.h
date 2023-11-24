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


void OneWayNavigation();

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



#endif //EXAMPLE_NAVIGATIONS_H
