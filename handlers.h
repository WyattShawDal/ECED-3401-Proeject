//
// Created by wyatt on 2023-11-22.
//
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
void AEDVHandler_NEW();

/**
 * @brief adds an order to the vehicle's array of orders
 * @param currentVehicle pointer to the current AEDV, passed by reference
 */
void AddOrder(AEDVNode **currentVehicle);
void QueryVehicleInfo();
void PrintVehicleStats(AEDV vehicle, int level);
void CommandHandler();
void GetCommands(int startup);




#endif //EXAMPLE_EVENTS_H
