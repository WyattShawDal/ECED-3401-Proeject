/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-12
 *Module Info: Contains Frame and Camera update functions, which make the core "loop" functions declared in main
*/
#include "dependencies.h"
// Module Functions Definition
void UpdateDrawFrame(void) {

    // begins drawing a new frame
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(camera);
    // Draw the map into the window using tile data generated in main
    UpdateMap();
    // Intelligent Order Assignment
    AEDVHandler();
    // Calculate Paths if necessary
    OneWayNavigation();


    // Draw the new movements
    AEDVNode* curr = ActiveList;
    AEDVNode* inactiveCurr = InactiveList;
    while(curr != NULL) {

        DrawVehicleMovements(curr);
        curr = curr->next;
    }
    while(inactiveCurr != NULL) {

        DrawVehicleMovements(inactiveCurr);
        inactiveCurr = inactiveCurr->next;
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
