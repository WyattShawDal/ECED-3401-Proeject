/**
 * Author: Wyatt Shaw & Cameron Archibald
 * Date: October 31st 2023
 * Module Info: Contains function definitions for moving vehicles around the map.
*/

#include "dependencies.h"

#ifdef TWOWAY



void StreetMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == EAST) {
        vehicle->position.x += 1;
    }
    else vehicle->position.x -= 1;
}

void AvenueMovement(AEDV *vehicle, int direction) {
    DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, vehicle->color);
    if(direction == SOUTH) {
        vehicle->position.y += 1;
    }
    else vehicle->position.y -= 1;
}

void MapNavigation(AEDV * vehicle) {
    int destinationTile = dynamicMap[vehicle->destination.x][vehicle->destination.y].Type;
    int currentTile = dynamicMap[vehicle->position.x][vehicle->position.y].Type;

    //if(vehicle->currStatus == TRANSIT) printf("AEDV [%d] currently at %d, %d --- Navigating to %d, %d\n", vehicle->EVIN, vehicle->position.x, vehicle->position.y, vehicle->destination.x, vehicle->destination.y );
    //else if(vehicle->currStatus == UNLOADING) printf("AEDV [%d] arrived at destination %d, %d\n", vehicle->EVIN, vehicle->position.x, vehicle->position.y, vehicle->destination.x, vehicle->destination.y );

    if((vehicle->destination.x == vehicle->position.x) && (vehicle->destination.y == vehicle->position.y)) {
        vehicle->currStatus = UNLOADING;
        DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
        return;
    }
    //Delivering to an Avenue
    if(destinationTile == AVENUE || destinationTile == AVENUE_N || destinationTile == AVENUE_S || destinationTile == JUNCTION) {
        //at a junction at the wrong x OR on a street, move in the direction that decreases distance
        if((currentTile == JUNCTION && (vehicle->position.x != vehicle->destination.x)) || (currentTile == STREET || currentTile == STREET_E || currentTile == STREET_W)) {
            if(vehicle->destination.x > vehicle->position.x)
                StreetMovement(vehicle, EAST);
            else
                StreetMovement(vehicle, WEST);
        }
        //at a junction with the correct x value, now move north or south to decrease distance
        else if(currentTile == JUNCTION) {
            if(vehicle->destination.y > vehicle->position.y)
                AvenueMovement(vehicle, SOUTH);
            else
                AvenueMovement(vehicle, NORTH);
        }
        //on an avenue
        else {
            if((vehicle->destination.x == vehicle->position.x) && (vehicle->destination.y == vehicle->position.y)) {
                vehicle->currStatus = UNLOADING;
                DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
                return;
            }
            else if(vehicle->destination.x == vehicle->position.x) {
                //On the destination avenue, move up or down
                if(vehicle->destination.y > vehicle->position.y)
                    AvenueMovement(vehicle, SOUTH);
                else
                    AvenueMovement(vehicle, NORTH);
            }
            else
                //On the wrong avenue, move up
                AvenueMovement(vehicle, NORTH);
        }
    }
    //Delivering to a street
    else if(destinationTile == STREET || destinationTile == STREET_E || destinationTile == STREET_W){
        //at a junction at the wrong y OR on an avenue, move in the direction that decreases distance
        if((currentTile == JUNCTION && (vehicle->position.y != vehicle->destination.y)) || (currentTile == AVENUE || currentTile == AVENUE_N || currentTile == AVENUE_S)) {
            if(vehicle->destination.y > vehicle->position.y)
                AvenueMovement(vehicle, SOUTH);
            else
                AvenueMovement(vehicle, NORTH);
        }
        //at a junction with the correct y value, now move east or west to decrease distance
        else if(currentTile == JUNCTION) {
            if(vehicle->destination.x > vehicle->position.x)
                StreetMovement(vehicle, EAST);
            else
                StreetMovement(vehicle, WEST);
        }
        //on a street
        else {
            if((vehicle->destination.x == vehicle->position.x) && (vehicle->destination.y == vehicle->position.y)) {
                vehicle->currStatus = UNLOADING;
                DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
                return;
            }
            else if(vehicle->destination.y == vehicle->position.y) {
                //On the destination street, move up or down
                if(vehicle->destination.x > vehicle->position.x)
                    StreetMovement(vehicle, EAST);
                else
                    StreetMovement(vehicle, WEST);
            }
            else
                //On the wrong avenue, move up
                StreetMovement(vehicle, WEST);
        }
    }
}
#endif

#ifdef OLD
void OneWayNavigation(AEDV * vehicle) {
    int currentTile = dynamicMap[vehicle->position.x][vehicle->position.y].Type;
    int destinationTile = dynamicMap[vehicle->destination.x][vehicle->destination.y].Type;
    Cord desiredJunctions[2];

    if(destinationTile == STREET ||destinationTile == STREET_W ||destinationTile == STREET_E) { //it's a street of some kind
        int shift = vehicle->destination.x % 4;
        desiredJunctions[0] = (Cord){.x = vehicle->destination.x - shift, .y = vehicle->destination.y};
        desiredJunctions[1] = (Cord) {.x = vehicle->destination.x + (4-shift),.y = vehicle->destination.y };
    }
    else { //it's an avenue of somekind
        int shift = vehicle->destination.y % 4;
        desiredJunctions[0] = (Cord){.x = vehicle->destination.x, .y = vehicle->destination.y - shift};
        desiredJunctions[1] = (Cord) {.x = vehicle->destination.x,.y = vehicle->destination.y + (4-shift)};
    }
   printf("Trying Junction Number %d @ CORD %d, %d\n", vehicle->junctionToTry, desiredJunctions[vehicle->junctionToTry].x, desiredJunctions[vehicle->junctionToTry].y);
    if(vehicle->position.x == vehicle->destination.x && vehicle->position.y == vehicle->destination.y) {
        vehicle->currStatus = UNLOADING;
    }
    if(vehicle->currStatus == UNLOADING) {
        DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
        return;
    }
#ifdef dont
    if(AtDesiredJunction(vehicle, desiredJunctions, destinationTile)) {
        int distanceX = vehicle->destination.x - vehicle->position.x;
        if(distanceX == 0) { // If our junction is on the same avenue
            int distanceY = vehicle->destination.y - vehicle->position.y;
            if(distanceY > 0 && IsValidDirection(vehicle, SOUTH)) {
                StreetMovement(vehicle, SOUTH);
            }

        }
        else { //If our junction is on the same street as our destination, move towards destination
            (distanceX > 0) ? StreetMovement(vehicle, EAST) : StreetMovement(vehicle, WEST);

        }
    }
#endif
    else if(currentTile == JUNCTION) {
        //Desired Junction is to our left and the road direction is valid
        if(vehicle->position.x > desiredJunctions[vehicle->junctionToTry].x && IsValidDirection(vehicle, WEST) ) {
            StreetMovement(vehicle, WEST);
        }
        //Desired Junction is to our right and the road direction is valid
        else if(vehicle->position.x < desiredJunctions[vehicle->junctionToTry].x && IsValidDirection(vehicle, EAST) ) {
            StreetMovement(vehicle, EAST);
        }
        //Desired Junction is directly above or below us or we couldn't move in a valid x direction
        else {
            //Is it above and can we move there?
            if(vehicle->position.y > desiredJunctions[vehicle->junctionToTry].y && IsValidDirection(vehicle, NORTH) ) {
                //Move to final location
                vehicle->junctionToTry = 0;
                AvenueMovement(vehicle, NORTH);
            }
            //Is it below and can we move there?
            else if(vehicle->position.y < desiredJunctions[vehicle->junctionToTry].y && IsValidDirection(vehicle, SOUTH) ) {
                vehicle->junctionToTry = 0;
                AvenueMovement(vehicle, SOUTH);
            }
            //It's in a direction we want to go but we can't go that way, so we need to adjust our X
            else {
                vehicle->junctionToTry = 1;
                //Move in whatever X direction is legal
                if((dynamicMap[vehicle->position.x+1][vehicle->position.y].Type == STREET_E) ||(dynamicMap[vehicle->position.x+1][vehicle->position.y].Type == STREET)) {
                    StreetMovement(vehicle, EAST);
                }
                else {
                    StreetMovement(vehicle, WEST);
                }
            }
        }
    }
    else {
        switch (currentTile) {
            case STREET_E:
                StreetMovement(vehicle, EAST);
                break;
            case STREET_W:
                StreetMovement(vehicle, WEST);
                break;
            case STREET:
                (vehicle->position.x  < desiredJunctions[vehicle->junctionToTry].x) ? StreetMovement(vehicle, EAST) : StreetMovement(vehicle, WEST);
                break;
            case AVENUE_N:
                AvenueMovement(vehicle, NORTH);
                break;
            case AVENUE_S:
                AvenueMovement(vehicle, SOUTH);
                break;
            case AVENUE:
                (vehicle->position.y  < desiredJunctions[vehicle->junctionToTry].y) ? AvenueMovement(vehicle, SOUTH) : AvenueMovement(vehicle, NORTH);
                break;
            default:
                printf("No direction provided to function");
                break;
        }
    }
}
#endif

#ifdef OLD
void OneWayNavigation(AEDV* vehicle) {
    int destinationTile = dynamicMap[vehicle->destination.x][vehicle->destination.y].Type;
    int currentTile = dynamicMap[vehicle->position.x][vehicle->position.y].Type;
    int desiredDirection;

    if(currentTile == JUNCTION) {
        //If we are south of the destination move north, else move south, west means we are on the correct level
        if(vehicle->position.y > vehicle->destination.y) {
            desiredDirection = NORTH;
        }
        else if(vehicle->position.y > vehicle->destination.y) {
            desiredDirection = SOUTH;
        }
        else if(vehicle->position.x > vehicle->destination.x) { //we must be on the correct westbound by this point.
            desiredDirection = WEST;
            //move west along street
            StreetMovement(vehicle, WEST);
            return;
        }


        //If we are right of the destination and we can move in the desired direction move that way
        if((vehicle->position.x > vehicle->destination.x) && (IsValidDirection(vehicle, desiredDirection))) {
            //Move North or South along the avenue towards the westbound street (should be one above or below us)
            StreetMovement(vehicle, desiredDirection);
        }
        else if(IsValidDirection(vehicle, EAST)){
            //If on an eastbound street, move East until past our destination
            StreetMovement(vehicle, EAST);

        } else {
            //On a westbound street not level with destination, move North or South
            if(IsValidDirection(vehicle,NORTH)) {
                StreetMovement(vehicle, NORTH);
                //Move north to next junction
            }
            else {
                StreetMovement(vehicle, SOUTH);
                //Move South
            }
        }

    }
    else {
        StreetMoves(currentTile, vehicle);
    }

}
#endif

#ifdef OLD
bool AtDesiredJunction(AEDV* vehicle, Cord* desiredJunctions, int destinationTile) {
    if(vehicle->position.x == desiredJunctions[0].x && vehicle->position.y == desiredJunctions[0].y) {
        return true;
    }
    else if (vehicle->position.x == desiredJunctions[1].x && vehicle->position.y == desiredJunctions[1].y) {
        return true;
    }
    else return false;
}
bool IsValidDirection(AEDV* vehicle, int direction) {
    switch (direction) {
        case NORTH:
            if((dynamicMap[vehicle->position.x][vehicle->position.y-1].Type == AVENUE_N) ||(dynamicMap[vehicle->position.x][vehicle->position.y-1].Type == AVENUE)) {
                return true;
            }
            return false;

        case EAST:
            if((dynamicMap[vehicle->position.x+1][vehicle->position.y].Type == STREET_E) ||(dynamicMap[vehicle->position.x+1][vehicle->position.y].Type == STREET)) {
                return true;
            }
            return false;

        case SOUTH:
            if((dynamicMap[vehicle->position.x][vehicle->position.y+1].Type == AVENUE_S) ||(dynamicMap[vehicle->position.x][vehicle->position.y+1].Type == AVENUE)) {
                return true;
            }
            return false;

        case WEST:
            if((dynamicMap[vehicle->position.x-1][vehicle->position.y].Type == STREET_W) ||(dynamicMap[vehicle->position.x-1][vehicle->position.y].Type == STREET)) {
                return true;
            }
            return false;
        default:
            return false;
    }
}
#endif

