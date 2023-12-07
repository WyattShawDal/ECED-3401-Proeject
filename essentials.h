//
// Created by wyatt on 2023-12-06.
//
#include "dependencies.h"

#ifndef EXAMPLE_ESSENTIALS_H
#define EXAMPLE_ESSENTIALS_H
/**
 * @brief function checks both lists for a given AEDV's ID, and then calls the print stats function
 */
void QueryVehicleInfo();

/**
 * @brief prints out a collection of information about and AEDV, has two levels of search
 * @param vehicle
 * @param level QUICK, IN_DEPTH for levels
 */
void PrintVehicleStats(AEDV vehicle, int level);

/**
 * @brief checks input on a given frame, it it matches a command key, run the function
 */
void CommandHandler();

/**
 * @brief prints out the list of commands
 * @param startup SETUP tells the function to output an intro message, otherwise it just prints the commands CALLED skips the init message
 */
void GetCommands(int startup);

/**
 * @brief allows for movement of the camera using the mouse
 */
void CameraControl();
#endif //EXAMPLE_ESSENTIALS_H
