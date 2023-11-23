/*
 * Created By: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains definition of Enum and Structs
 */
#include "dependencies.h"

#ifndef EXAMPLE_STRUCTS_H
#define EXAMPLE_STRUCTS_H

typedef enum Type {
    STREET = 0,
    STREET_E,
    STREET_W,
    AVENUE,
    AVENUE_N,
    AVENUE_S,
    JUNCTION,
    BUILDING,
#ifdef TASK6
    STABLE,
    CHARGER,
    BOTH,
    CONSTRUCTION,
    ACCIDENT
#endif
}Tile_Type;


typedef enum Status {
    IDLE,
    PICKUP,
    TRANSIT,
    DELIVERING,
    RECHARGING,
    UNLOADING
}AEDV_STATUS;

typedef struct Coordinate {
    int x;
    int y;
} Cord;

typedef struct Tile {
    Cord location;
    bool isOccupied;
    Tile_Type Type;

}Tile;

typedef struct AEDV {
    int EVIN;
    int junctionToTry;
    int distanceTravelled;
    int currentCharge;
    int maxCharge;
    int chargeRate;
    Cord position;
    Cord destination;
    Vector2 drawSize;
    Color color;
    AEDV_STATUS currStatus;

}AEDV;
typedef enum OPERATION {READ_BINARY, WRITE_BINARY, READ_TEXT, WRITE_TEXT} Operation;

typedef struct event {
    char type;
    int time;
    int originID;
    int destinationID;
    int weight;
}EVENT;

#define MAX_NAME_LENGTH 50
typedef struct {

    int custNum;
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char building[MAX_NAME_LENGTH];
    char entrance[MAX_NAME_LENGTH];
    int floor;
}Customer;
typedef enum BLDG_TYPE { CHG = 0, STB, BOTH, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD { N, S, E, W, INVALID_QUAD }QUADRANT_TYPE;
typedef struct bldg {
    Cord location;
    BUILDING_TYPE type;
    QUADRANT_TYPE quad;
}BUILDINGDATA;


#endif //EXAMPLE_STRUCTS_H
