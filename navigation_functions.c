/**
* Author: Wyatt Shaw & Cameron Archibald
 * Date 2023-10-30
 * Module Info: Contains function definitions for moving vehicles around the map.
*/

#include "dependencies.h"

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

void OneWayNavigation(AEDV * vehicle) {
    int currentTile = dynamicMap[vehicle->position.x][vehicle->position.y].Type;
    int destinationTile = dynamicMap[vehicle->destination.x][vehicle->destination.y].Type;
    Cord desiredJunctions[2];
    static int tryJunction = 0;
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
   printf("Trying Junction Number %d @ CORD %d, %d\n", tryJunction, desiredJunctions[tryJunction].x, desiredJunctions[tryJunction].y);
    if(vehicle->position.x == vehicle->destination.x && vehicle->position.y == vehicle->destination.y) {
        vehicle->currStatus = UNLOADING;
    }
    if(vehicle->currStatus == UNLOADING) {
        DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);

        return;
    }
    if(currentTile == JUNCTION) {
        //Desired Junction is to our left and the road direction is valid
        if(vehicle->position.x > desiredJunctions[tryJunction].x && IsValidDirection(vehicle, WEST) ) {
            StreetMovement(vehicle, WEST);
        }
        //Desired Junction is to our right and the road direction is valid
        else if(vehicle->position.x < desiredJunctions[tryJunction].x &&  IsValidDirection(vehicle, EAST) ) {
            StreetMovement(vehicle, EAST);
        }

        //Desired Junction is directly above or below us or we couldn't move in a valid x direction
        else {
            //Is it above and can we move there?
            if(vehicle->position.y > desiredJunctions[tryJunction].y && IsValidDirection(vehicle, NORTH) ) {
                //Move to final location
                tryJunction = 0;

                AvenueMovement(vehicle, NORTH);
            }
            //Is it below and can we move there?
            else if(vehicle->position.y < desiredJunctions[tryJunction].y &&  IsValidDirection(vehicle, SOUTH) ) {
                tryJunction = 0;

                AvenueMovement(vehicle, SOUTH);
            }
            //It's in a direction we want to go but we can't go that way, so we need to adjust our X
            else {
                tryJunction = 1;
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
                (vehicle->position.x  < desiredJunctions[tryJunction].x) ? StreetMovement(vehicle, EAST) : StreetMovement(vehicle, WEST);
                break;
            case AVENUE_N:
                AvenueMovement(vehicle, NORTH);
                break;
            case AVENUE_S:
                AvenueMovement(vehicle, SOUTH);
                break;
            case AVENUE:
                (vehicle->position.y  < desiredJunctions[tryJunction].y) ? AvenueMovement(vehicle, SOUTH) : AvenueMovement(vehicle, NORTH);
                break;
        }
    }
}


#ifdef HOLD
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


void StreetMoves(int currentTile, AEDV *vehicle) {
    int shift = vehicle->destination.x % 4;
    switch (currentTile) {
        case STREET_E:
            StreetMovement(vehicle, EAST);
            break;
        case STREET_W:
            StreetMovement(vehicle, WEST);
            break;
        case STREET:
            if(vehicle->position.x < vehicle->destination.x - shift){
                StreetMovement(vehicle, EAST);
            }
            else {
                StreetMovement(vehicle, WEST);
            }
            break;
        case AVENUE_N:
            AvenueMovement(vehicle, NORTH);
            break;
        case AVENUE_S:
            AvenueMovement(vehicle, SOUTH);
            break;
        case AVENUE:
            if(vehicle->position.y < vehicle->destination.y - shift){
                StreetMovement(vehicle, SOUTH);
            }
            else {
                StreetMovement(vehicle, WEST);
            }
            break;
    }
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






#ifdef OLDNAV
void MapNavigation(AEDV * vehicle) {
    //Delivering to an Avenue
    if(dynamicMap[vehicle->destination.x][vehicle->destination.y].Type == AVENUE) {
        //TODO this is the state that should take user input
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);

            return;
        }
            //at the wrong x, move in the direction that decreases distance
        else if((dynamicMap[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.x != vehicle->destination.x) || dynamicMap[vehicle->destination.x][vehicle->position.y].Type == STREET) {
            if(vehicle->destination.x > vehicle->position.x) {
                StreetMovement(vehicle, EAST);
            }
            else if (vehicle->destination.x < vehicle->position.x) StreetMovement(vehicle, WEST);
        }
            //at a junction with the correct x value, now move north or south to decrease distance
        else{
            if(vehicle->destination.y > vehicle->position.y) {
                AvenueMovement(vehicle, SOUTH);
            }
            else if (vehicle->destination.y < vehicle->position.y)AvenueMovement(vehicle, NORTH);
            //If we've made it to our destination we can set our state to be unloading(idle)
            if(vehicle->position.y == vehicle->destination.y) {
                vehicle->currStatus = UNLOADING;
            }
        }
    }
        //Delivering to a street
    else {
        if(vehicle->currStatus == UNLOADING) {
            DrawRectangleV((Vector2) {.x = vehicle->position.x * cellWidth,.y = vehicle->position.y * cellHeight}, vehicle->drawSize, GREEN);
            /*
             * GetNewAddress()
             *      OUPUT "Enter New Destination for AEDV [ EVIN ]"
             *      READ destination.x destination.y
             *      SET status = active or whatever
             *
             */
            return;
        }
        //If you're at a junction and not at the right street height, adjust to correct height
        if((dynamicMap[vehicle->position.x][vehicle->position.y].Type == JUNCTION && vehicle->position.y != vehicle->destination.y) || dynamicMap[vehicle->position.x][vehicle->position.y].Type == AVENUE ) {
            if (vehicle->destination.y > vehicle->position.y) {
                AvenueMovement(vehicle, SOUTH);
            } else if (vehicle->destination.y < vehicle->position.y) AvenueMovement(vehicle, NORTH);
        }

            //At the correct height now pick a direction to go to either east or west (depend on street direction)
        else{
            if(vehicle->destination.x > vehicle->position.x) {
                StreetMovement(vehicle, EAST);
            }
            else if (vehicle->destination.x < vehicle->position.x) StreetMovement(vehicle, WEST);
            //If we've made it to our destination we can set our state to be unloading(idle)
            if(vehicle->position.x == vehicle->destination.x) {
                vehicle->currStatus = UNLOADING;
            }
        }
    }
}
#endif


