/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-22
 *Module Info:
*/

#ifndef EXAMPLE_FILES_H
#define EXAMPLE_FILES_H
#include "dependencies.h"


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

/**
 * @brief creates the delivery file with a header containing the first available package num (500)
 */
void CreateDeliveryFile(void);

/**
 * @brief adds to the delivery file, updating the linked list of deliveries via the customer file
 * @param order delivery struct passed in
 */
void AddToDeliveryFile(OrderData order);

/**
 * @brief returns delivery entries from a given position in the delivery file
 * @param mode HEADER returns the first entry, ENTRY returns entry at provided packageNum
 * @param packageNum used to specify which record to return. Value ignored for HEADER
 * @return returns the DeliveryEntry (union between header and order struct)
 */
DeliveryEntry ReadDeliveryFile(int mode, int packageNum);

/**
 * @brief navigates (relative) LastDeliveryFile for given ID to determine that customer's most recent delivery
 * @param ID
 * @return returns LastDeliveryEntry (struct containing customer ID and last package number)
 */
LastDeliveryEntry GetLastDelivery(int ID);

/**
 * @brief prints records from delivery file
 * @param mode ALL prints all records in ascending package order, CUSTOMER prints a given origin customer's packages from latest to earliest
 * SINGLE prints record with given package number
 * @param ID Used to specify which customer, ignored for ALL
 */
void QueryDeliveryInfo(int mode, int ID);

void ConvertBuildingCords(BuildingData *building);

void CreateVehicleFile(int MODE);

void AddToVehicleFile(VehicleEntry newEntry);

VehicleEntry GetVehicleEntry (int MODE, int recordNum);

void RecordFinalVehicleStates(void);

void PrintVehicleFile(int EVIN);

void PrintVehicleEntry(VehicleEntry entry);

void CloseFiles(void);

bool IsValidEVIN(int EVIN);
#endif //EXAMPLE_FILES_H
