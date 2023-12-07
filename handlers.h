/*
 *Author: Wyatt Shaw & Cameron Archibald
 *Created on: 2023-11-12
 *Module Info: Contains function definitions for handler.c
*/
#include "dependencies.h"

#ifndef EXAMPLE_EVENTS_H
#define EXAMPLE_EVENTS_H
/**
 * @brief Reads from the event list and calls subsidiary handlers
 * @param time
 * @param current
 * @return time of next event
 */
int EventHandler(int time,EventNode **current);

/**
 * @brief Takes in two customers data structs and packages into an order for an AEDV
 * @param Root
 * @param Order
 * @param Delivery
 * @return packaged order
 */
OrderData OrderHandler(OrderNode** Root, Customer Order, Customer Delivery);

/**
 * @brief adjusts coordinates to fit with grid parameters
 * @param location
 * @return coordinate pair of adjusted location
 */
Cord AdjustOrder(Cord location);

/**
 * @brief assigns deliveries to vehicles in the inactive list
 */
void AEDVHandler();

/**
 * @brief adds an order to the vehicle's array of orders
 * @param currentVehicle pointer to the current AEDV, passed by reference
 */
void AddOrder(AEDVNode **currentVehicle);





#endif //EXAMPLE_EVENTS_H
