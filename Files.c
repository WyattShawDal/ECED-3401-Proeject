/*
 * Author: Wyatt Shaw
 * Created on: 2023-11-22
 * Module Info: The following module declares all the functions related to the reading/writing of information
 * to/from a file
*/

#include "files.h"
const char* genFile = "genFile.dat";
const char* eventFile = "eventFile.txt";
const char* relativeCustomerFile = "RelativeCustomerFile.dat";

/**
 * @brief create a binary file for writing for generation
 *
 */

bool OpenTargetFile(Operation OPERATION, const char *fileName, FILE **FPointer) {

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
    BUILDINGDATA building;

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

    printf("Enter Building's Row (<1 to break, 1 is first row): ");
    (void) fscanf(stdin, "%d", &building.location.x);
    (void) getchar(); /* Eat last EOLN */

    while(building.location.x > 0){
        printf("Enter building's Column (<1 to break): ");
        (void) fscanf(stdin, "%d", &building.location.y);
        (void) getchar(); /* Eat last EOLN */
        do
        {
            printf("Enter building type (Charge = 0; Stable = 1, Both = 2): ");
            (void) fscanf(stdin, "%d", &building.type);
            (void) getchar(); /* Eat last EOLN */
        }
        while (!(building.type >= CHG && building.type < INVALID_TYPE));

        do
        {
            printf("Enter quad (N = 0, S = 1, E = 2, W = 3): ");
            (void) fscanf(stdin, "%d", &building.quad);
            (void) getchar(); /* Eat last EOLN */

        }
        while (!(building.quad >= N && building.quad < INVALID_QUAD));

        /* Write the bldg_data to the file */
        fwrite(&building, sizeof(building), 1, FileDescriptor);

        printf("Enter building's X coordinate (<=0 to stop): ");
        (void) fscanf(stdin, "%d", &building.location.x);
        (void) getchar(); /* Eat last EOLN */

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
    int xLength, yLength;
    char streetDirection, avenueDirection;
    BUILDINGDATA building;

    fread(&xLength, sizeof(int), 1, FileDescriptor);
    fread(&yLength, sizeof(int), 1, FileDescriptor);
    MAX_COLS = xLength*4+1;
    MAX_ROWS = yLength*4+1;
    cellHeight = screenWidth/MAX_COLS;
    cellWidth = screenWidth/MAX_COLS;
    printf("You entered %d x %d, which forms %d Cols and %d Rows\n", xLength, yLength, MAX_COLS, MAX_ROWS);

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
    while (building.location.x > 0)
    {
        printf("Building XY: %d %d\nBuilding Quadrant: %d\nBuilding Type: %d\n", building.location.x, building.location.y, building.quad, building.type);
        /* Read next record */
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
    int iteration = 0;
    int time, origin, destination, weight;
    EVENT currentEvent;
    OpenTargetFile(READ_TEXT, fileName, &FileDescriptor);

    //TODO add an fgets to eat the header
    while(fgets(line, sizeof(line), FileDescriptor) != NULL) {
        if(iteration == 0) {
            printf("Header Values..\n");
        }
        else {
            sscanf(line, "%d\t%c\t%d\t%d\t%d\t", &time, &eventName, &origin, &destination, &weight);

            printf("Time = %d, Event = %c, Origin = %d, Destination = %d, Weight = %d\n", time,eventName, origin, destination, weight);
            currentEvent.time = time;
            currentEvent.type = eventName;
            currentEvent.originID = origin;
            currentEvent.destinationID = destination;
            currentEvent.weight = weight;
            AddEvent(&EventList, currentEvent);
        }
        iteration++;
    }
    (void) getchar();
    fclose(FileDescriptor);
}
void ReadCustomerFile(int originCode, int destinationCode) {
    Customer customerInfo;
    OpenTargetFile(READ_TEXT, "CustomerFile.txt", &FileDescriptor);
    int originGoal = originCode - CUSTOMERBASE;
    int destinationGoal = destinationCode - CUSTOMERBASE;
    int iteration = 0;
    char line[sizeof(Customer)];

    fgets(line, MAXSTRLEN, FileDescriptor); // Read and discard a line
    // Read and display each record sequentially
    while (fgets(line, sizeof(line), FileDescriptor) != NULL) {
        if(iteration == originGoal) {
            sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);
            printf(" CustNum: %d", customerInfo.custNum);
            printf("  First Name: %s", customerInfo.firstName);
            printf("  Last Name: %s", customerInfo.lastName);
            printf("  Building: %s", customerInfo.building);
            printf("  Entrance: %s", customerInfo.entrance);
            printf("  Floor: %d\n", customerInfo.floor);
        }
        else if(iteration == destinationGoal) {
            sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);
            printf(" CustNum: %d", customerInfo.custNum);
            printf("  First Name: %s", customerInfo.firstName);
            printf("  Last Name: %s", customerInfo.lastName);
            printf("  Building: %s", customerInfo.building);
            printf("  Entrance: %s", customerInfo.entrance);
            printf("  Floor: %d\n", customerInfo.floor);
        }
        else {
            //printf("Wrong Record..\n");
        }
        iteration++;
        // Add other fields as needed

    }

    // Close the file
    fclose(FileDescriptor);

}

void ReadTextCustomerFile() {
    Customer customerInfo;
    OpenTargetFile(READ_TEXT, "CustomerFile.txt", &FileDescriptor);
    OpenTargetFile(WRITE_BINARY, "RelativeCustomerFile.dat", &RelCustomerFileDescriptor);
    char line[sizeof(Customer)];

    fgets(line, MAXSTRLEN, FileDescriptor); // Read and discard a line
    // Read and display each record sequentially
    while (fgets(line, sizeof(line), FileDescriptor) != NULL) {
        sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);
        printf(" CustNum: %d", customerInfo.custNum);
        printf("  First Name: %s", customerInfo.firstName);
        printf("  Last Name: %s", customerInfo.lastName);
        printf("  Building: %s", customerInfo.building);
        printf("  Entrance: %s", customerInfo.entrance);
        printf("  Floor: %d\n", customerInfo.floor);
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
