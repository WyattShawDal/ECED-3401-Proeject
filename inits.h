/*
 *Author: Wyatt on 
 *Created on: 2023-11-12
*/

#ifndef EXAMPLE_INITS_H
#define EXAMPLE_INITS_H
#include "dependencies.h"

#define NOT_TWO_WAY_ROW() ((j != 0) && (j != MAX_ROWS - 1))
#define NOT_TWO_WAY_COL() ((i != 0) && (i != MAX_COLS - 1))

void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode);
/**
 * @brief initializes values of the tiles in the map
 * @note Uses setPerimeterRoads() and setInternalTiles()
 * @warning Must be called after GenerateBuildFile()
 */
void InitTiles();
/**
 * @brief sets the values of the boundary roads (either two way roads or junctions)
 * @param i col
 * @param j row
 */
void setPerimeterRoads(int i, int j);
/**
 * @brief sets the value of the internal tiles according to road directions set
 * @param i
 * @param j
 * @param firstStreetDirection
 * @param firstAvenueDirection
 */
void setInternalTiles(int i, int j, int firstStreetDirection, int firstAvenueDirection);
/**
* @brief  the size of the map array using the input rows and columns
*/
void setDirection(int i, int j);
void setJunctionDirection(int i, int j, int firstStreetDirection, int firstAvenueDirection);

void allocDynamicMap();

void SetupInitialConditions();
void AEDVInput();
/**
 * @brief populates AEDV struct with inital values
 */
Cord QuadrantToStreetAddress(int quad, Cord location);
void SpawnAEDVs(void);
void InitRoutine();
void GenerationCheck();
void InitAEDVStats(AEDVNode **vehicle, int MODE, VehicleStats existingStats);
void ModifyAEDVStats(AEDVNode **vehicle);

bool IsValidEVIN(int EVIN);
#endif //EXAMPLE_INITS_H
