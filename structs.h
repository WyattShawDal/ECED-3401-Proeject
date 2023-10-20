//
// Created by wyatt on 2023-10-18.
//
#include "dependencies.h"

#ifndef EXAMPLE_STRUCTS_H
#define EXAMPLE_STRUCTS_H
typedef enum Type {
    STREET = 0,
    AVENUE,
    JUNCTION,
    BUILDING,
    CONSTRUCTION,
    ACCIDENT
}Tile_Type;

typedef enum Status {
    IDLE,
    TRANSIT,
    RECHARGING,
    UNLOADING
}AEDV_STATUS;

typedef struct Tile {
    int i;
    int j;
    bool isOccupied;
    Tile_Type Type;

}Tile;

typedef struct AEDV {
    Vector2 location;
    Vector2 destination;
    Vector2 drawSize;
    Color color;
    AEDV_STATUS currStatus;
}AEDV;

#endif //EXAMPLE_STRUCTS_H
