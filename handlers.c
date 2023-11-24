//
// Created by wyatt on 2023-11-22.
//

#include "dependencies.h"

int EventHandler(int time, EventNode **root) {
    EventNode *current = *root;

    int temp = time;
    //RemoveEvent(&EventList);
    printf("Calling Customer %d\n", current->eventData.originID);
    if(toupper(current->eventData.type) != 'D') {
        current = current->nextEvent;
        *root = current;
        printf("Event Not a Delivery (Out of Scope for Task 6\n");
        return time;
    }
    //Get first set of order data
    ReadCustomerFile(current->eventData.originID, current->eventData.destinationID);
    ReadTextCustomerFile();
    OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
     GetCustomerInfo(current->eventData.destinationID));
    //If there are no more events after this one
    if((current = current->nextEvent) == NULL) {
        printf("No more events total amount");
        (void)getchar();

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
            ReadCustomerFile(current->eventData.originID, current->eventData.destinationID);
            //Call order handler to process the next order
            OrderHandler(&OrderList, GetCustomerInfo(current->eventData.originID),
                         GetCustomerInfo(current->eventData.destinationID));
            current = current->nextEvent;
            *root = current;
            time = current->eventData.time;
        }
        printf("Time of next event = %d\n", time);
    }
    return time;
}

OrderData OrderHandler(OrderNode** Root, Customer Order, Customer Delivery) {
    OrderData newOrder;

    newOrder.pickupLocation.x = Order.building[0] - 'A';
    newOrder.pickupLocation.y = Order.building[1] - 'A';
    newOrder.pickupFloor = Order.floor;
    //adjust coordinates to match AEDV perspective
    newOrder.pickupLocation = AdjustOrder(newOrder.pickupLocation);

    newOrder.dropOffLocation.x = Delivery.building[0] - 'A';
    newOrder.dropOffLocation.y = Delivery.building[1] - 'A';
    newOrder.dropFloor = Delivery.floor;
    //adjust coordinates to match AEDV perspective
    newOrder.dropOffLocation = AdjustOrder(newOrder.dropOffLocation);


    /* Shift delivery according to entrance location
     * Cells are equal to 1, a building is comprised of a 3x3 grid of cells, and label located at the centre
     * Thus to shift for the entrance we need to move 2 tiles in the given direction
    */
    if(strcmp(Order.entrance, "N") == 0) {
        newOrder.pickupLocation.y +=2; //navigate to the road above location
    }
    else if(strcmp(Order.entrance, "S") == 0) {
        newOrder.pickupLocation.y -=2; //navigate to the road below location
    }
    else if(strcmp(Order.entrance, "E") == 0) {
        newOrder.pickupLocation.y +=2; //navigate to the road east of location
    }
    else if(strcmp(Order.entrance, "W") == 0) {
        newOrder.pickupLocation.y -=2; //navigate to the road west location
    }
    else {
        printf("Unexpected Entrance Location");
    }
    if(strcmp(Delivery.entrance, "N") == 0) {
        newOrder.dropOffLocation.y +=2; //navigate to the road above location
    }
    else if(strcmp(Delivery.entrance, "S") == 0) {
        newOrder.dropOffLocation.y -=2; //navigate to the road below location
    }
    else if(strcmp(Delivery.entrance, "E") == 0) {
        newOrder.dropOffLocation.y +=2; //navigate to the road east of location
    }
    else if(strcmp(Delivery.entrance, "W") == 0) {
        newOrder.dropOffLocation.y -=2; //navigate to the road west location
    }
    else {
        printf("Unexpected Entrance Location");
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
 * So B --> 1, pickupLocation = 1*4 +2 = 6 etc.
 */
    if(location.x == 0) {
        location.x += 2;
    }
    else {
        location.x = (location.x*4) +2;
    }
    if(location.y == 0) {
        location.y += 2;
    }
    else {
        location.y = (location.y*4) +2;
    }


    return location;
}
