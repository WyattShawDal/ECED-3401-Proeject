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
    TRANSIT,
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
    Cord position;
    Cord destination;
    Vector2 drawSize;
    Color color;
    AEDV_STATUS currStatus;

}AEDV;

#endif //EXAMPLE_STRUCTS_H
