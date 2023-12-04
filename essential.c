/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains Frame and Camera update functions as well as querying functions, which make the core "loop" functions declared in main
*/
#include "dependencies.h"
// Module Functions Definition
void UpdateDrawFrame(void) {

    CommandHandler();

    // begins drawing a new frame
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    DrawText(TextFormat("Current Time: %d", frameCount), 0, -50, 20, RAYWHITE); //print the time every 10 ticks
    // Draw the map into the window using tile data generated in main
    UpdateMap();
    // Intelligent Order Assignment
    AEDVHandler_NEW();
    // Calculate Paths if necessary
    OneWayNavigation_NEW();
    // Draw the new movements
    AEDVNode* curr = ActiveList;

    while(curr != NULL) {
        DrawVehicleMovements(curr);
        UpdateVehicleStats(&curr);

        curr = curr->next;
    }
    curr = InactiveList;
    while(curr != NULL) {
        DrawVehicleMovements(curr);
        UpdateVehicleStats(&curr);
        curr = curr->next;
    }
    frameCount++; // Update Frame count to next tick
    EndDrawing();
}


/* Initialization and Assigment Functions */
void CameraControl() {
    if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))  {
        Vector2 delta  = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f/camera.zoom);

        camera.target = Vector2Add(camera.target, delta);
    }

    float wheel = GetMouseWheelMove();
    if(wheel != NOMOVEMENT) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;
        const float zoomIncrement = 0.125f; //float value to adjust the level of zoom

        camera.zoom += (wheel*zoomIncrement);
        if(camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
    }
}

void QueryVehicleInfo() {
    int vehicleIndex;
    printf("Enter Vehicle EVIN : ");
    scanf("%d", &vehicleIndex);

    //Search active list
    AEDVNode * vehicle = FindInList(ActiveList,vehicleIndex);

    if(vehicle == NULL)
        //If not in active list, search inactive list
        vehicle = FindInList(InactiveList,vehicleIndex);

    if(vehicle == NULL)
        //Not in either list
        printf("No AEDV in either list with EVIN = %d\n\n", vehicleIndex);
    else
        //Vehicle found, print stats
        PrintVehicleStats(vehicle->data,IN_DEPTH);
}

void QueryDeliveryInfo(int command, int custID) {
    PrintDeliveries(command, custID);
}

void PrintVehicleStats(AEDV vehicle, int level) {
    if(level == QUICK) {
        printf("Quick stats:\n"
               "Status = %d\n"
               "Order Number = %d\n"
               "Total Orders = %d\n\n",
               vehicle.currStatus,  vehicle.currentOrderNumber,  vehicle.orderCount);
    }
    else if (level == IN_DEPTH) {
        printf("In depth stats:\n"
               "Status = %d\n"
               "Order Number = %d\n"
               "Total Orders = %d\n"
               "Battery Level = %lf\n"
               "Ticks Moving = %d\n"
               "Ticks Idle = %d\n\n",
               vehicle.currStatus,  vehicle.currentOrderNumber,  vehicle.orderCount,
               vehicle.stats.currentBattery, vehicle.stats.ticksMoving, vehicle.stats.ticksIdle);
    }
}

void GetCommands(int startup) {
    if(startup == SETUP) {
        printf("Welcome to the AEDV delivery system. This system employs various commands for I/O\n"
               "Here is a list of useful commands, you may call this list again by pressing 'H'\n"
               "Query vehicle information: F\n"
               "List all deliveries: D\n"
               "List a customers deliveries: S\n"
               "Print a specific delivery: P\n"
               "Zoom: Mouse Wheel up/down\n"
               "Pan: Hold Right Click.\n"
               "This program was written by: Wyatt Shaw & Cameron Archibald for ECED 3401\n\n");
    }
    else {
        printf("Here is a list of useful commands, you may call this list again by pressing 'H'\n"
               "Query vehicle information: F\n"
               "List all deliveries: D\n"
               "List a customers deliveries: S\n"
               "Print a specific delivery: P\n"
               "Zoom: Mouse Wheel up/down\n"
               "Pan: Hold Right Click.\n\n");
    }
}
void CommandHandler() {
    if(IsKeyPressed(KEY_H)) {
        GetCommands(CALLED);
    }
    if(IsKeyPressed(KEY_D))
        QueryDeliveryInfo(ALL, 0);

    if(IsKeyPressed(KEY_F)) {
        QueryVehicleInfo();
    }
    if(IsKeyPressed(KEY_S)) {
        int custID;
        printf("Enter the customer ID to search: ");
        scanf("%d", &custID);
        QueryDeliveryInfo(CUSTOMER, custID);
    }
    if(IsKeyPressed(KEY_P)) {
        int packageID;
        printf("Enter the package ID to search: ");
        scanf("%d",&packageID);
        QueryDeliveryInfo(PACKAGE,packageID);
    }
}

/*    double maxBattery;
    double currentBattery;
    double rechargeRate;
    double drivingRate; //discharge
    double idleRate; //stationary
    int ticksMoving;
    int ticksIdle;
    int ticksCharging;*/