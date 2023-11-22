/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-22
 *Module Info:
*/

#ifndef EXAMPLE_FILES_H
#define EXAMPLE_FILES_H
#include "dependencies.h"


#define TRUNCATE(name)	name[strlen(name)-1] = '\0'
#define NAMELEN	16	/* Filename length */

typedef enum EVENT_TYPE {
    Delivery,
    Construction,
    Spawn
}EV_Type;

typedef struct event {

    EV_Type Type;
    int Time;
    Cord Origin;
    Cord Destination;
    int weight;
}EVENT;
typedef enum BLDG_TYPE { CHG = 0, STB, BOTH, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD { N, S, E, W, INVALID_QUAD }QUADRANT_TYPE;
typedef struct bldg {
    Cord location;
    BUILDING_TYPE type;
    QUADRANT_TYPE quad;
}BUILDINGDATA;

int OpenBinaryFile();
void GenerateBuildFile();

bool CheckDirectionChar(char direction);
bool CheckBuildingChar(char building);

/* File Pointers */


#endif //EXAMPLE_FILES_H
