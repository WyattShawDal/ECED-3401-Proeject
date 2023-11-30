/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-22
 *Module Info:
*/

#ifndef EXAMPLE_FILES_H
#define EXAMPLE_FILES_H
#include "dependencies.h"

#define TRUNCATE(name)	name[strlen(name)-1] = '\0'

bool OpenTargetFile(OPERATION Operation, const char *fileName, FILE **FPointer);

/**
 * @brief generates a binary c file for building generation according to user input
 */
void GenerateBuildFile();
/**
 * @brief reads a binary file to generate information for the mapping functions
 * @param streetDir
 * @param avenueDir
 */
void ReadBuildFile(int * streetDir, int * avenueDir);
/**
 * @brief reads the tab-seperated event file sequentially and adds to an event list
 * @param fileName
 */
void ReadEventFile(char *fileName);
/**
 * @brief creates a relative customer file from the tab-seperated customer file
 */
void CreateRelativeCustomerFile();
/**
 * @brief navigates relative file to access specific customer record
 * @param CustomerID id of customer to be returned
 * @return customer struct containing customer data
 * @note used in OrderHandler to get customer parameters
 */
Customer GetCustomerInfo(int CustomerID);

bool CheckDirectionChar(char direction);
bool CheckBuildingChar(char building);
void ConvertBuildingCords(BuildingData *building);
#endif //EXAMPLE_FILES_H
