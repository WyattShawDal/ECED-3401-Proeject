/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info:
*/
#include "dependencies.h"
// Module Functions Definition
void UpdateDrawFrame(void) {
    // Allows user to press F to input new destination for an AEDV
    if(GetKeyPressed() == KEY_F) {
        AssignNewOrders();
    }
    // begins drawing a new frame
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    // Draw the map into the window using tile data generated in main
    UpdateMap();
    // begin navigation
    for (int i = 0; i < maxAEDV; ++i) {
        //OneWayNavigation(listOfVehicles[i]); <-- Not used for Task 5
        MapNavigation(listOfVehicles[i]);
    }
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
    if(wheel != 0) {
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        camera.offset = GetMousePosition();
        camera.target = mouseWorldPos;
        const float zoomIncrement = 0.125f;

        camera.zoom += (wheel*zoomIncrement);
        if(camera.zoom < zoomIncrement) camera.zoom = zoomIncrement;
    }
}