/*
 *Author: Wyatt on 
 *Created on: 2023-11-12
*/

#ifndef EXAMPLE_NAVIGATIONS_H
#define EXAMPLE_NAVIGATIONS_H
#include "dependencies.h"

/**
 * @brief handles decision making for AEDV movement and pathing
 */
void OneWayNavigation();
void UpdateVehicleStats(AEDVNode ** currentVehicle);

/**
 * @brief handles movement of active AEDV's
 */
void OneWayNavigation_NEW();

void NoMoveCalculated(AEDVNode * currentVehicle);

void UpdateDeliveryStats(AEDVNode ** currentVehicle, int orderNum, int mode);
BuildingNode* FindClosestBuilding(Cord position, int mode);

double FindDiagonalDistance(Cord pos1, Cord pos2);
#endif //EXAMPLE_NAVIGATIONS_H
