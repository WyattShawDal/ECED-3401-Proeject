/*
 *Author: Wyatt Shaw & Cameron Archibald
 *Created on: 2023-11-22
 *Module Info: Contains function declarations for file operations
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
 * @brief convert building coordinates from build file (in terms of building number)
 * to map coordinates (XY position), and adjusting based on quadrant
 * @param building
 */
void ConvertBuildingCords(BuildingData *building);

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
 * @param MODE ALL prints all records in ascending package order, CUSTOMER prints a given origin customer's packages from latest to earliest
 * SINGLE prints record with given package number
 * @param ID Used to specify which customer, ignored for ALL
 */
void QueryDeliveryInfo(int MODE, int ID);

/**
 * @brief if specified, initializes new (relative) vehicle file with a header, to store final vehicle states
 * @param MODE KEEP_FILE gives the user a choice to overwrite file, NEW_FILE forces to overwrite
 */
void CreateVehicleFile(int MODE);

/**
 * @brief adds entry to end of vehicle file
 * @param newEntry new entry containing EVIN, stats, and final quadrant
 */
void AddToVehicleFile(VehicleEntry newEntry);

/**
 * @brief fetches entries from the vehicle file
 * @param MODE HEADER returns the header entry,
 * ENTRY gets entry at index recordNum,
 * VEHICLE_NUMBER gets most recent entry of EVIN recordNum
 * @param recordNum Ignored by mode HEADER
 * @return
 */
VehicleEntry GetVehicleEntry (int MODE, int recordNum);

/**
 * @brief traverses the inactiveList and records their states in the vehicle file
 */
void RecordFinalVehicleStates(void);

/**
 * @brief prints entries from the vehicle file
 * @param EVIN ignored if MODE = ALL
 * @param MODE ALL prints every entry in the vehicle file, VEHICLE prints all entries with the given EVIN
 */
void PrintVehicleFile(int EVIN, int MODE);

/**
 * @brief handles printf statements for printing vehicle file entries
 * @param entry
 */
void PrintVehicleEntry(VehicleEntry entry);

/**
 * closes delivery file, vehicle file, and last delivery file
 */
void CloseFiles(void);

/**
 * @brief checks if EVIN is within the bounds (10000 to 10000 + (MAX_VEHICLES_FILE))
 * @param EVIN
 * @return TRUE if EVIN is allowable, FALSE if not
 */
bool IsValidEVIN(int EVIN);
#endif //EXAMPLE_FILES_H
