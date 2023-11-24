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
typedef struct order {
    Cord pickupLocation;
    Cord dropOffLocation;
    int pickupFloor;
    int dropFloor;
}OrderData;

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

#define FIRSTNAMELEN 11
#define LASTNAMELEN 16
#define BUILDINGLEN 3 //Fits 2 chars + \000

typedef struct {
    int custNum;
    char firstName[FIRSTNAMELEN];
    char lastName[LASTNAMELEN];
    char building[BUILDINGLEN];
    char entrance[BUILDINGLEN];
    int floor;
}Customer;
typedef enum BLDG_TYPE { CHG = 0, STB, BOTH, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD { N, NE, NW, S, SE, SW, E, W, INVALID_QUAD }QUADRANT_TYPE;
typedef struct bldg {
    Cord location;
    BUILDING_TYPE type;
    QUADRANT_TYPE quad;
}BUILDINGDATA;

typedef struct prompt
{
    char* name;
    enum QUAD code;
} KeyPair;

//KeyPair entranceQuadrant[] = {
//        {"NE",NE}, {"N",N}, {"NW",NW},
//        {"E",E}, {"LBL",-1}, {"W",W},
//        {"SE",SE}, {"S",S}, {"SW",SW} };


#endif //EXAMPLE_STRUCTS_H
