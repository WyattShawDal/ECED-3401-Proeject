/*
 * Created By: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains definition of Enum and Structs
 */
#include "dependencies.h"

#ifndef EXAMPLE_STRUCTS_H
#define EXAMPLE_STRUCTS_H
#define FIRSTNAMELEN 11
#define LASTNAMELEN 16
#define BUILDINGLEN 3
#define MAX_ORDER_COUNT 5

/* Included at top so all typedefs can use definition easily*/
typedef struct Coordinate {
    int x;
    int y;
} Cord;

/* Enums */
typedef enum Type {
    STREET = 0,
    STREET_E,
    STREET_W,
    AVENUE,
    AVENUE_N,
    AVENUE_S,
    JUNCTION,
    BUILDING,
    CHARGER,
    STABLE,
    BOTH,
    CONSTRUCTION,

}TILE_TYPE;

typedef enum Status {
    IDLE,
    PICKUP,
    TRANSIT,
    DROPOFF,
    LOADING,
    UNLOADING,
    ETGOHOME,
    RECHARGING,
}AEDV_STATUS;

typedef enum Visit {
    NO,
    YES
}VISITED;
typedef enum OP {READ_BINARY, WRITE_BINARY, READ_TEXT, WRITE_TEXT} OPERATION;
typedef enum BLDG_TYPE { CHG = 0, STB, B, INVALID_TYPE }BUILDING_TYPE;
typedef enum QUAD {NE, NW, SE, SW, INVALID_QUAD }QUADRANT_TYPE;

/* Structs */
typedef struct Tile {
    Cord location;
    TILE_TYPE Type;
    bool validDirection[4]; //[SOUTH,NORTH,EAST,WEST] (typdef in dependencies.h).
}Tile;


typedef struct event {
    char type;
    int time;
    int originID;
    int destinationID;
    int weight;
}EventData;
/* Need to add +1 to listed length */

typedef struct {
    int custNum;
    char firstName[FIRSTNAMELEN];
    char lastName[LASTNAMELEN];
    char building[BUILDINGLEN];
    char entrance[BUILDINGLEN];
    int floor;
}Customer;
typedef struct order {
    Cord pickUp;
    Cord dropOff;
    int pickupFloor;
    int dropFloor;
    Customer activeCustomers[2];
}OrderData;

typedef struct bldg {
    Cord location;
    char buildingLabel[BUILDINGLEN];
    BUILDING_TYPE type;
    QUADRANT_TYPE quad;
}BuildingData;

typedef struct EventNode {
    EventData eventData;
    struct EventNode *nextEvent;
}EventNode;

typedef struct OrderNode {
    OrderData data;
    struct OrderNode *nextOrder;
}OrderNode;

typedef struct TileNode {
    struct TileNode *parent;
    struct TileNode *queuePrev;
    struct TileNode *visitedPrev;
    Cord coordinate;
}TileNode;

typedef struct BuildingNode {
    BuildingData data;
    struct BuildingNode* nextBuilding;
}BuildingNode;

typedef struct InstructionNode {
    struct InstructionNode *child;
    Cord nextPosition;
}InstructionNode;

typedef struct queue {
    TileNode * front;
    TileNode * rear;
}queue;

typedef struct AEDV {
    int EVIN;
    int distanceTravelled;
#ifdef TASK7
    int maxBattery;
    int currentBattery;
    int rechargeRate;
    int drivingRate; //discharge
    int idleRate; //stationary
    int ticksMoving;
    int ticksIdle;
    int ticksCharging;
#endif
    int currentOrderNumber;
    int orderCount;
    int loadingDelay;
    int unloadingDelay;
    Cord position;
    Cord pickUp;
    Cord dropOff;
    Vector2 drawSize;
    Color color;
    AEDV_STATUS currStatus;
    InstructionNode * nextMove;
}AEDV;

typedef struct Node {
    AEDV data;
    struct Node *next;
}AEDVNode;

#endif //EXAMPLE_STRUCTS_H
