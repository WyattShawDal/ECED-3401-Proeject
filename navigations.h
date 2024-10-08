/*
 *Author: Wyatt Shaw & Cameron Archibald
 *Created on: 2023-11-12
 *Module Info: Contains function definitions for general navigation functions
*/

#ifndef EXAMPLE_NAVIGATIONS_H
#define EXAMPLE_NAVIGATIONS_H
#include "dependencies.h"

/**
 * @brief handles decision making for AEDV movement and pathing
 */
void OneWayNavigation();

/**
 * @brief updates a given vehicles stats that are written to the vehicle file at then end
 * @param currentVehicle
 */
void UpdateVehicleStats(AEDVNode ** currentVehicle);

/**
 * @brief handles assigning new paths and loading/unload for a vehicle
 * @param currentVehicle
 */
void NoMoveCalculated(AEDVNode * currentVehicle);

/**
 * @brief updates time for pickup/drop off of delivery
 * @param currentVehicle
 * @param orderNum Index of order held by AEDV to update
 * @param mode PICKUP updates the pickup time of the vehicle's order to current time
 *             DROPOFF "" dropoff time ""
 */
void UpdateDeliveryStats(AEDVNode ** currentVehicle, int orderNum, int mode);

/**
 * @brief finds the closest stable or charger from current position
 * @param position
 * @param mode STABLE searches the list of stables, CHARGER searches the list of chargers
 * @return Pointer to the BuildingNode of the closest building
 */
BuildingNode* FindClosestBuilding(Cord position, int mode);

/**
 * @brief uses Pythagorean theorem find distance between two points
 * @param pos1
 * @param pos2
 * @return returns distance
 */
double FindDiagonalDistance(Cord pos1, Cord pos2);
#endif //EXAMPLE_NAVIGATIONS_H
