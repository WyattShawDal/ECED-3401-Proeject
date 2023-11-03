/*
 * Created By: Wyatt Shaw and Cameron Archibald
 * Date: October 31st 2023
 * Module Info : Contains required libraries as well as function prototypes
 *
 */

#ifndef EXAMPLE_DEPENDENCIES_H
#define EXAMPLE_DEPENDENCIES_H

#define TWOWAY //Allows for two-way navigation
//#define DEBUG //Allows for diagnostic printing
//#define OLD //Functions from one-way experimenting
//#define OLD1 //Fixed readability and efficiency by Cameron on 31st of October

#include "stdio.h" // included for regular c functionality printf, scanf etc
#include "raylib.h" // included for graphical output
#include "raymath.h" // required by raylib.h
#include "rlgl.h" //required by raylib.h
#include "typedefs.h" // included for function prototypes
#include "raygui.h" // included for alt screen buttons and boxes
#include <ctype.h> // included for tolower

#include <malloc.h> // included for dynamic allocation using malloc


#define SOUTH 0
#define NORTH 1
#define EAST 2
#define WEST 3

extern int MAX_COLS, MAX_ROWS, cellWidth, cellHeight;
extern Tile** dynamicMap;

/**
 * @brief Function draws the map and then calls the navigations function
 */
void UpdateDrawFrame(void);     // Update and Draw one frame
/**
 * @brief Draws the map of the city using the initalized Tile array
 * @param tile Struct containing Tile type information
 */
void DrawMap(Tile tile);
/**
 * @brief Calculates and then updates the position of the AEDV in the map
 * @param vehicle AEDV to move around
 * @note algorithm works on a two-way rule free road system
 */
void MapNavigation(AEDV *vehicle);
/**
 * @brief Calculates North/South Movement
 * @param vehicle AEDV to move
 * @param direction North/South
 */
void AvenueMovement(AEDV *vehicle, int direction);
/**
 * @brief Calculates East/West Movement
 * @param vehicle AEDV to move
 * @param direction Esat/West
 */
void StreetMovement(AEDV *vehicle, int direction);
/**
 * @brief allows for moving and resizing of the camera
 */
void CameraControl();
/**
 * @brief  the size of the map array using the input rows and columns
 */
void allocDynamicMap();
/**
 * @brief function takes user input and then calls InitAEDV to assign provided values
 */
void AEDVInput();
/**
 * @brief populates AEDV struct with inital values
 */
void InitAEDV(AEDV *vehicle, int locationX, int locationY, int destinationX, int destinationY, int identifierCode);
/**
 * @brief initializes values of the tiles in the map
 * @note Uses setPerimeterRoads() and setInternalTiles()
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
 * @brief One way pathing navigation algorithm
 *
 * @param vehicle
 *
 * @note Algorithm is dumb, only checking one tile ahead and without considering rules of the road. Ommitted from Task 5 Code
 */
void OneWayNavigation(AEDV * vehicle);
/**
 * @brief function used for one way navigation, determines whether the input direction is able to be travelled
 * @param vehicle
 * @param direction
 * @note used in one way, omitted for task 5
 * @return True or False
 */
bool IsValidDirection(AEDV* vehicle, int direction);
/**
 *
 * @param currentTile
 * @param vehicle
 * @note used in one way, omitted for task 5
 */
void StreetMoves(int currentTile, AEDV *vehicle);
/**
 *
 * @param vehicle
 * @param destinationTile
 * @param desiredJunctions
 * @note used in one way omitted in task 5
 */
void FindAdjacentJunctions(AEDV* vehicle, int destinationTile, Cord* desiredJunctions);
/**
 *
 * @param vehicle
 * @param desiredJunctions
 * @param destinationTile
 * @note utilized in one way, omitted in task 5
 * @return
 */
bool AtDesiredJunction(AEDV* vehicle, Cord* desiredJunctions, int destinationTile);
/**
 * @brief uses generation rules to determine if the tile chosen is a deliverable location
 * @param col
 * @param row
 * @note function uses col and row instead of a tile type because it can be called before tiles have been generated
 * @return
 */
bool isValidDestination(int col, int row);

void SetupInitialConditions();

void UpdateMap();

void AssignNewOrders();

void UpdateInputScreen();


#endif //EXAMPLE_DEPENDENCIES_H
