/*
 * Author: Wyatt Shaw
 * Created on: 2023-11-22
 * Module Info: The following module declares all the functions related to the reading/writing of information
 * to/from a file
*/

#include "files.h"
const char* GenFile = "GenFile.dat";
/**
 * @brief create a binary file for writing for generation
 *
 */
int OpenBinaryFile() {
    //_set_fmode(_O_BINARY);

    return (BuildingFileDescriptor = fopen(GenFile, "wb")) != NULL;
}

void GenerateBuildFile() {
    int xLength, yLength;
    char streetDirection, avenueDirection;
    BUILDINGDATA building;


    printf("Enter #X and #Y Buildings: _ _\n");
    (void) fscanf(stdin, "%d %d", &xLength, &yLength);
    (void) getchar(); /* Eat last EOLN */

    printf("You inputted %d x %d buildings\n", xLength, yLength);
    fwrite(&xLength, sizeof(int), 1, BuildingFileDescriptor);
    fwrite(&yLength, sizeof(int), 1, BuildingFileDescriptor);

    do {
        printf("Enter Direction of first street (E/W): ");
        fscanf(stdin, "%c", &streetDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(CheckDirectionChar(streetDirection) == false);

    fwrite(&streetDirection, sizeof(char), 1, BuildingFileDescriptor);

    do {
        printf("Enter Direction of first street (N/S): ");
        fscanf(stdin, "%c", &avenueDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(CheckDirectionChar(avenueDirection) == false);

    fwrite(&avenueDirection, sizeof(char), 1, BuildingFileDescriptor);

    printf("Enter Building's Row (<1 to break, 1 is first row): ");
    (void) fscanf(stdin, "%d", &building.location.x);
    (void) getchar(); /* Eat last EOLN */

    while(building.location.x > 0){
        printf("Enter building's Y coordinate (>0)\n");
        (void) fscanf(stdin, "%d", &building.location.y);
        (void) getchar(); /* Eat last EOLN */


        do
        {
            printf("Enter building type (Charge = 0; Stable = 1, Both = 2)\n");
            (void) fscanf(stdin, "%d", &building.type);
            (void) getchar(); /* Eat last EOLN */

        }
        while (!(building.type >= CHG && building.type < INVALID_TYPE));

        do
        {
            printf("Enter quad (N = 0, S = 1, E = 2, W = 3)\n");
            (void) fscanf(stdin, "%d", &building.quad);
            (void) getchar(); /* Eat last EOLN */

        }
        while (!(building.quad >= N && building.quad < INVALID_QUAD));

        /* Write the bldg_data to the file */
        fwrite(&building, sizeof(building), 1, BuildingFileDescriptor);

        printf("Enter building's X coordinate (<=0 to stop); \n");
        (void) fscanf(stdin, "%d", &building.location.x);
        (void) getchar(); /* Eat last EOLN */

    }

    /* End with 0 0 */
    xLength = 0;

    fwrite(&xLength, sizeof(int), 1, BuildingFileDescriptor);
    fwrite(&xLength, sizeof(int), 1, BuildingFileDescriptor);

    fclose(BuildingFileDescriptor);

    (void) getchar(); /* Eat last EOLN */
}


bool CheckDirectionChar(char direction) {
    char check = tolower(direction);
    switch (check) {
        case 'n':
            return true;
        case 'e':
            return true;
        case 's':
            return true;
        case 'w':
            return true;
        default:
            return false;
    }
}
bool CheckBuildingChar(char building) {
    char check = tolower(building);
    switch (check) {
        case 'b':
            return true;
        case 's':
            return true;
        case 'c':
            return true;
        default:
            return false;
    }
}