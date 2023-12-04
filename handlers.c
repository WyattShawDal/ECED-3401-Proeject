//
// Created by wyatt on 2023-11-22.
//

#include "dependencies.h"

/*
 * Checks if the Order List is empty, then check if the InActiveAEDV is empty if both are not empty
 * It should assign the order to an AEDV, and Call Swap Lists to move that to active
 * It should do this until either list is empty
 *  A B C D
 *
 */

void AEDVHandler() {
    if(InactiveList == NULL) {
        //printf("No Free AEDV's\n");
        return;
    }
    AEDVNode *currentAEDV = InactiveList;
    if(OrderList == NULL) {
        //printf("No More Orders\n");
        currentAEDV->data.currStatus = RESET_PICKUP;
        return;
    }
    //Both values updated in loop, so must be checked in condition

    while(InactiveList != NULL && OrderList != NULL){
        //Set AEDV values for navigation
        currentAEDV->data.loadingDelay = (OrderList->data.pickupFloor*delaySecsPerFloor*frameRate)/delayScale; //Ticks per floor, includes up and down
        currentAEDV->data.unloadingDelay = (OrderList->data.dropFloor*delaySecsPerFloor*frameRate)/delayScale;
        currentAEDV->data.pickUp = OrderList->data.pickUp;
        currentAEDV->data.dropOff = OrderList->data.dropOff;
        currentAEDV->data.currStatus = RESET_PICKUP;
        //store next address in temp pointer since it will be lost in the swap
        AEDVNode * temp = currentAEDV->next;
        SwapBetweenLists(&InactiveList,&ActiveList,currentAEDV->data.EVIN);
        currentAEDV =  temp;
        //update to next order
        OrderList = OrderList->nextOrder;
    }
}

//PUT ORDERS INTO ARRAYS OF AS MANY AS POSSIBLE THAT ARE INACTIVE
void AEDVHandler_NEW() {
    AEDVNode *currentAEDV = InactiveList; //Get first AEDV in the inactive list

    //While inactive vehicles exist and the orderlist isn't empty
    while(currentAEDV != NULL && OrderList != NULL) {
        currentAEDV->data.orderCount = 0; //current inactive AEDV has 0 orders assigned

        //While current vehicle has less than 5 orders assigned and orderlist isn't empty
        while(currentAEDV->data.orderCount < MAX_ORDER_COUNT && OrderList != NULL) {
            AddOrder(&currentAEDV); //Assign order to AEDV
            OrderList = OrderList->nextOrder; //Move to next order in order list
        }

        currentAEDV->data.currStatus = RESET_PICKUP; //Change the vehicle to the next active state
        AEDVNode * temp = currentAEDV->next; //Save next AEDV from inactive list
        SwapBetweenLists(&InactiveList,&ActiveList,currentAEDV->data.EVIN); //Move AEDV to active list
        currentAEDV =  temp; //Move to next vehicle in inactive list
    }
}

//PUT ORDER IN THE ARRAY
void AddOrder(AEDVNode **currentVehicle) {

    (*currentVehicle)->orderList[(*currentVehicle)->data.orderCount] = OrderList->data; //Add oldest item in delivery list to the AEDV's array
    (*currentVehicle)->data.orderCount++; //AEDV now has one more order assigned
}

int EventHandler(int time, EventNode **root) {
    EventNode *current = *root;
    int temp = time;
    if(toupper(current->eventData.type) == 'D') {
        //Get first set of order data
        OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
                     GetCustomerInfo(current->eventData.destinationID));
        //If there are no more events after this one
        if ((current = current->nextEvent) == NULL) {
            printf("No more events\n");
            return time;
        }
            //There are more events, are they at the same time?
        else {
            //update our root node from main
            *root = current;
            //update time to check the next event's time
            time = current->eventData.time;
            //if the first event is the same as the last event
            while (temp == time) {
                temp = time;
                printf("Two events with same time\n");
                //ReadCustomerFile(current->eventData.originID, current->eventData.destinationID);
                //Call order handler to process the next order
                OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
                             GetCustomerInfo(current->eventData.destinationID));
                current = current->nextEvent;
                *root = current;
                time = current->eventData.time;
            }
            //printf("Time of next event = %d\n", time);
        }
    }
    /* maybe combine these and pass the type to the handler... */
    else if(toupper(current->eventData.type) == 'C') {
        //start construction
    }
    else if(toupper(current->eventData.type) == 'E') {
        // end construction
    }
    else {
        current = current->nextEvent;
        *root = current;
        printf("Unknown Event occurred");
        return time;
    }
    return time;
}
#ifdef TASK7
int EventHandler(int time, EventNode **root) {

    EventNode *current = *root;
    int temp = time;

    //Get first set of order data
    switch(toupper(current->eventData.type)) {
        case 'D':
            OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
                         GetCustomerInfo(current->eventData.destinationID));
            break;
        case 'C':
            //start event
            break;
        case 'E':
            //end construction
            break;
    }
    //If there are no more events after this one
    if ((current = current->nextEvent) == NULL) {
        printf("No more events total amount");
        return time;
    }
        //There are more events, are they at the same time?
    else {
        //update our root node from main
        *root = current;
        //update time to check the next event's time
        time = current->eventData.time;
        //if the first event is the same as the last event
        while (temp == time) {
            temp = time;
            printf("Two events with same time\n");

            switch(toupper(current->eventData.type)) {
                case 'D':
                    OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
                                 GetCustomerInfo(current->eventData.destinationID));
                    break;
                case 'C':
                    //start event
                    break;
                case 'E':
                    //end construction
                    break;
            }
            current = current->nextEvent;
            *root = current;
            time = current->eventData.time;
        }
        //printf("Time of next event = %d\n", time);

    }
        /* maybe combine these and pass the type to the handler... */
    else if(toupper(current->eventData.type) == 'C') {
        //start construction
    }
    else if(toupper(current->eventData.type) == 'E') {
        // end construction
    }
    else {
        current = current->nextEvent;
        *root = current;
        printf("Unknown Event occurred");
        return time;
    }
    return time;
}
#endif
OrderData OrderHandler(OrderNode** Root, Customer Order, Customer Delivery) {
    OrderData newOrder;

    newOrder.activeCustomers[0] = Order;
    newOrder.activeCustomers[1] = Delivery;
    //subtract 'A' to get integer code from the building code
    newOrder.pickUp.x = Order.building[0] - 'A';
    newOrder.pickUp.y = Order.building[1] - 'A';
    newOrder.pickupFloor = Order.floor;
    //adjust coordinates to match AEDV perspective
    newOrder.pickUp = AdjustOrder(newOrder.pickUp);

    newOrder.dropOff.x = Delivery.building[0] - 'A';
    newOrder.dropOff.y = Delivery.building[1] - 'A';
    newOrder.dropFloor = Delivery.floor;
    //adjust coordinates to match AEDV perspective
    newOrder.dropOff = AdjustOrder(newOrder.dropOff);

    /* Shift delivery according to entrance location
     * Cells are equal to 1, a building is comprised of a 3x3 grid of cells, and label located at the centre
     * Thus to shift for the entrance we need to move 2 tiles in the given direction
    */
    if(strcmp(Order.entrance, "N") == 0) {
        newOrder.pickUp.y -=TILESHIFT; //navigate to the road above location
    }
    else if(strcmp(Order.entrance, "S") == 0) {
        newOrder.pickUp.y +=TILESHIFT; //navigate to the road below location
    }
    else if(strcmp(Order.entrance, "E") == 0) {
        newOrder.pickUp.x +=TILESHIFT; //navigate to the road east of location
    }
    else if(strcmp(Order.entrance, "W") == 0) {
        newOrder.pickUp.x -=TILESHIFT; //navigate to the road west location
    }
    else {
        printf("Unexpected Entrance Location, %s", Order.entrance);
    }
    if(strcmp(Delivery.entrance, "N") == 0) {
        newOrder.dropOff.y -=TILESHIFT; //navigate to the road above location
    }
    else if(strcmp(Delivery.entrance, "S") == 0) {
        newOrder.dropOff.y +=TILESHIFT; //navigate to the road below location
    }
    else if(strcmp(Delivery.entrance, "E") == 0) {
        newOrder.dropOff.x +=TILESHIFT; //navigate to the road east of location
    }
    else if(strcmp(Delivery.entrance, "W") == 0) {
        newOrder.dropOff.x -=TILESHIFT; //navigate to the road west location
    }
    else {
        printf("Unexpected Entrance Location, %s", Order.entrance);
    }
    AddOrderToList(&OrderList, newOrder);
    return newOrder;
}

Cord AdjustOrder(Cord location) {
/*
 * Building Codes [AA... FI]
 * Navigation uses integer values, so convert location from character to integer value
 * Need to shift integer value to the actual on map tile location, the first row & column this is a shift of 2
 * for all others it's a shift of location*4 + 2
 * A = 0, so Pickup Location = 0 + 2 = 2
 * So B --> 1, pickUp = 1*4 +2 = 6 etc.
 */
    if(location.x == 0) {
        location.x += TILESHIFT;
    }
    else {
        location.x = (location.x*4) +TILESHIFT;
    }
    if(location.y == 0) {
        location.y += TILESHIFT;
    }
    else {
        location.y = (location.y*4) +TILESHIFT;
    }
    return location;
}

