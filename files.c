/*
 * Author: Wyatt Shaw
 * Created on: 2023-11-22
 * Module Info: The following module declares all the functions related to the reading/writing of information
 * to/from a file
*/

#include "dependencies.h"
const char* genFile = "genFile.dat";
//#define TestingFunction
#ifdef TestingFunction
const char* eventFile = "TestEventFile.txt";
const char* relativeCustomerFile = "TestRelativeCustomerFile.dat";
const char* customerFile = "TestCustomerFile.txt";
#else
const char* eventFile = "eventFile.txt";
const char* relativeCustomerFile = "RelativeCustomerFile.dat";
const char* customerFile = "CustomerFile.txt";
#endif
/**
 * @brief create a binary file for writing for generation
 *
 */

bool OpenTargetFile(OPERATION OPERATION, const char *fileName, FILE **FPointer) {

    if(OPERATION == WRITE_BINARY) {
        return (*FPointer = fopen(fileName, "wb")) != NULL;
    }
    else if(OPERATION == READ_BINARY) {
        return (*FPointer = fopen(fileName, "rb")) != NULL;
    }
    else if(OPERATION == WRITE_TEXT) {
        return (*FPointer = fopen(fileName, "w")) != NULL;
    }
    else if(OPERATION == READ_TEXT) {
        return (*FPointer = fopen(fileName, "r")) != NULL;
    }
}

/* Building File Functions */
void GenerateBuildFile() {
    int xLength, yLength;
    int streetDirection, avenueDirection;
    BuildingData building;

    printf("Enter #X and #Y Buildings: ");
    (void) fscanf(stdin, "%d %d", &xLength, &yLength);
    (void) getchar(); /* Eat last EOLN */

    printf("You inputted %d x %d buildings\n", xLength, yLength);
    fwrite(&xLength, sizeof(int), 1, FileDescriptor);
    fwrite(&yLength, sizeof(int), 1, FileDescriptor);

    do {
        printf("Enter Direction of first street (1 for E, 2 for W): ");
        fscanf(stdin, "%d", &streetDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(!(streetDirection >= STREET_E && streetDirection <= STREET_W));

    fwrite(&streetDirection, sizeof(int), 1, FileDescriptor);

    do {
        printf("Enter Direction of first street (4 for N, 5 for S): ");
        fscanf(stdin, "%d", &avenueDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(!(avenueDirection >= AVENUE_N && avenueDirection <= AVENUE_S));

    fwrite(&avenueDirection, sizeof(int), 1, FileDescriptor);

    printf("Enter Building's Label (XX to exit): ");
    (void) fscanf(stdin, "%s", building.buildingLabel);
    (void) getchar(); /* Eat last EOLN */
    building.location.x = building.buildingLabel[0] - 'A';
    building.location.y = building.buildingLabel[1] - 'A';

    while(building.location.x >= 0){
        do
        {
            printf("Enter building type (Charger = 0; Stable = 1, Both = 2): ");

            (void) fscanf(stdin, "%d", &building.type);
            (void) getchar(); /* Eat last EOLN */
        }
        while (!(building.type >= CHG && building.type < INVALID_TYPE));
        do
        {
            printf("Enter quad (NE = 0, NW = 1, SE = 2, SW = 3): ");
            (void) fscanf(stdin, "%d", &building.quad);
            (void) getchar(); /* Eat last EOLN */

        }
        while (!(building.quad >= NE && building.quad < INVALID_QUAD));

        /* Write the bldg_data to the file */
        fwrite(&building, sizeof(building), 1, FileDescriptor);

        printf("Enter Building's Label (XX to exit): ");
        (void) fscanf(stdin, "%s", building.buildingLabel);
        (void) getchar(); /* Eat last EOLN */
        building.location.x = building.buildingLabel[0] - 'A';
        building.location.y = building.buildingLabel[1] - 'A';
        if(strcmp(building.buildingLabel, "XX") == 0) {
            building.quad = INVALID_QUAD;
            building.type = INVALID_TYPE;
            printf("Exit Code Chosen\n");
            fwrite(&building, sizeof(building), 1, FileDescriptor);

            break;
        }

    }

    /* End with 0 0 for read function to know it's made it to the end */
    xLength = 0;
    fwrite(&xLength, sizeof(int), 1, FileDescriptor);
    fwrite(&xLength, sizeof(int), 1, FileDescriptor);

    fclose(FileDescriptor);

    (void) getchar(); /* Eat last EOLN */
}

void ReadBuildFile(int * streetDir, int * avenueDir) {
    OpenTargetFile(READ_BINARY, "GenerationFile.dat", &FileDescriptor);
    int numBuildingsX, numBuildingsY;
    char streetDirection, avenueDirection;
    BuildingData building;

    fread(&numBuildingsX, sizeof(int), 1, FileDescriptor);
    fread(&numBuildingsY, sizeof(int), 1, FileDescriptor);
    MAX_COLS = numBuildingsX * 4 + 1; //Building widths are 3, with a road to the left are 4, then add one road to the right
    MAX_ROWS = numBuildingsY * 4 + 1; //Same justification, but for adding road below
    cellHeight = screenWidth/MAX_COLS;
    cellWidth = screenWidth/MAX_COLS;
    printf("You entered %d x %d, which forms %d Cols and %d Rows\n", numBuildingsX, numBuildingsY, MAX_COLS, MAX_ROWS);

    fread(streetDir, sizeof(int), 1, FileDescriptor);
    fread(avenueDir, sizeof(int), 1, FileDescriptor);

    if(*streetDir == STREET_E) {
        streetDirection = 'E';
    }
    else {
        streetDirection = 'W';
    }
    if(*avenueDir == AVENUE_S) {
        avenueDirection = 'S';
    }
    else {
        avenueDirection = 'N';
    }
    printf("You entered directions Street : %c, Avenue : %c\n", toupper(streetDirection), toupper(avenueDirection));

    fread(&building, sizeof(building), 1, FileDescriptor);
    while (strcmp(building.buildingLabel, "XX") != 0)
    {
        ConvertBuildingCords(&building);
        printf("Building XY: %d %d\nBuilding Quadrant: %d\nBuilding Type: %d\n", building.location.x, building.location.y, building.quad, building.type);
        /* Read next record */
        AddBuilding(&StableList, &ChargerList, building);
        fread(&building, sizeof(building), 1, FileDescriptor);
    }
    (void) getchar();
    fclose(FileDescriptor);
}
/*
 * Event File is a sequential, tab seperated file. So, to read it we can just do normal reading procedures while parsing out data
 */
void ReadEventFile(char *fileName) {
    char line[MAXSTRLEN];
    char eventName;
    int time, origin, destination, weight;
    EventData currentEvent;
    OpenTargetFile(READ_TEXT, fileName, &FileDescriptor);

    fgets(line, sizeof(line), FileDescriptor);
    while(fgets(line, sizeof(line), FileDescriptor) != NULL) {
        sscanf(line, "%d\t%c\t%d\t%d\t%d\t", &time, &eventName, &origin, &destination, &weight);

        printf("Time = %d, Event = %c, Origin = %d, Destination = %d, Weight = %d\n", time,eventName, origin, destination, weight);
        currentEvent.time = time;
        currentEvent.type = eventName;
        currentEvent.originID = origin;
        currentEvent.destinationID = destination;
        currentEvent.weight = weight;
        AddEvent(&EventList, currentEvent);

    }
    fclose(FileDescriptor);
}

void CreateRelativeCustomerFile() {
    Customer customerInfo;
    OpenTargetFile(READ_TEXT, customerFile, &FileDescriptor);
    OpenTargetFile(WRITE_BINARY, relativeCustomerFile, &RelCustomerFileDescriptor);
    char line[sizeof(Customer)];

    fgets(line, MAXSTRLEN, FileDescriptor); // Read and discard text header
    // Read and display each record sequentially
    while (fgets(line, sizeof(line), FileDescriptor) != NULL) {
        sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);

        //write out customer info into a binary file for relative accessing
        fwrite(&customerInfo, sizeof(Customer), 1, RelCustomerFileDescriptor);
        // Add other fields as needed

    }
    // Close the file
    fclose(FileDescriptor);
    fclose(RelCustomerFileDescriptor);

}

/**
 *
 * @param CustomerID
 * @return Returns customer struct for orderHandler to process
 */
Customer GetCustomerInfo(int CustomerID) {
    Customer customer;
    OpenTargetFile(READ_BINARY, relativeCustomerFile, &RelCustomerFileDescriptor);
    //CustomerID's start at 1000, so subtract 1000 from id
    long relativePosition = (CustomerID-CUSTOMERBASE) * sizeof(customer);
    //Seek to record we want to read
    fseek(RelCustomerFileDescriptor, relativePosition, SEEK_SET);
    //read the customer we want into a customer struct
    fread(&customer, sizeof(customer), 1, RelCustomerFileDescriptor);
    //return the customer struct
    return customer;
}

void ConvertBuildingCords(BuildingData *building) {
    if(building->location.x > 0) {
        building->location.x = building->location.x*4 + TILESHIFT;
    }
    else {
        building->location.x += TILESHIFT;
    }
    if(building->location.y > 0) {
        building->location.y = building->location.y*4 + TILESHIFT;
    }
    else {
        building->location.y += TILESHIFT;
    }
    /* Converts stable location based on quadrant */
    switch (building->quad) {
        case NE:
            building->location.x +=1;
            building->location.y -=1;
            break;
        case NW:
            building->location.x -=1;
            building->location.y -=1;
            break;
        case SE:
            building->location.x +=1;
            building->location.y +=1;
            break;
        case SW:
            building->location.x -=1;
            building->location.y +=1;
            break;
        case INVALID_QUAD:
            break;
    }
}
