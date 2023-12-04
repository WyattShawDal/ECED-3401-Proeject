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
const char* eventFile = "EventFile.txt";
const char* lastDeliveries = "lastDeliveries";
const char* relativeCustomerFile = "RelativeCustomerFile.dat";
const char* customerFile = "CustomerFile.txt";
const char* deliveryFile = "DeliveryFile.dat";

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
    else if(OPERATION == READ_WRITE_BINARY) {
        return (*FPointer = fopen(fileName, "wb+")) != NULL;
    }
    else {
        printf("Invalid File Opening Command.. Exiting");
        exit(-1);
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
    //printf("You entered %d x %d, which forms %d Cols and %d Rows\n", numBuildingsX, numBuildingsY, MAX_COLS, MAX_ROWS);

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
    //printf("You entered directions Street : %c, Avenue : %c\n", toupper(streetDirection), toupper(avenueDirection));

    fread(&building, sizeof(building), 1, FileDescriptor);
    while (strcmp(building.buildingLabel, "XX") != 0)
    {
        ConvertBuildingCords(&building);
        //printf("Building XY: %d %d\nBuilding Quadrant: %d\nBuilding Type: %d\n", building.location.x, building.location.y, building.quad, building.type);
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

        //printf("Time = %d, Event = %c, Origin = %d, Destination = %d, Weight = %d\n", time,eventName, origin, destination, weight);
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

    LastDeliveryEntry last; //Holds structs to place in lastDeliveries file
    last.lastDelivery = -1; //No deliveries occured yet
    OpenTargetFile(READ_WRITE_BINARY,lastDeliveries,&LastDeliveryDescriptor);

    OpenTargetFile(READ_TEXT, customerFile, &FileDescriptor);
    OpenTargetFile(WRITE_BINARY, relativeCustomerFile, &RelCustomerFileDescriptor);
    char line[sizeof(Customer)];

    fgets(line, MAXSTRLEN, FileDescriptor); // Read and discard text header
    // Read and display each record sequentially
    while (fgets(line, sizeof(line), FileDescriptor) != NULL) {
        sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);
        //write out customer info into a binary file for relative accessing
        fwrite(&customerInfo, sizeof(Customer), 1, RelCustomerFileDescriptor);

        //Create an entry in the lastDelivery file with ID corresponding to the txt cust file, and last delivery -1
        last.custNum = customerInfo.custNum;
        fwrite(&last,sizeof(LastDeliveryEntry),1,LastDeliveryDescriptor);
    }
    // Close the file
    fclose(FileDescriptor);
    fclose(RelCustomerFileDescriptor);
}

void CreateDeliveryFile() {
    OpenTargetFile(READ_WRITE_BINARY,deliveryFile,&DeliveryFileDescriptor);
    //Write header
    DeliveryEntry h = {.header.firstEmptyDelivery = 500};
    fwrite(&h,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);

}

LastDeliveryEntry GetLastDelivery(int ID) {
    //Return entry from the file containing ID's and last deliveries
    LastDeliveryEntry last;
    fseek(LastDeliveryDescriptor,(ID - CUSTOMERBASE)*sizeof(LastDeliveryEntry),SEEK_SET);
    fread(&last,sizeof(LastDeliveryEntry),1,LastDeliveryDescriptor);
    return last;
}

void AddToDeliveryFile(OrderData order) {
    LastDeliveryEntry recentDelivery = GetLastDelivery(order.activeCustomers[0].custNum); //Get origin customer's last delivery
    //Create new record with data passed in by order
    DeliveryEntry newEntry;
    newEntry.data = order;

    newEntry.data.nextDelivery = recentDelivery.lastDelivery; //New record points to current last delivery
    DeliveryEntry oldHeader = ReadDeliveryFile(HEADER,0); //Get the header from the delivery file
    newEntry.data.packageNumber = oldHeader.header.firstEmptyDelivery; //New entry gets the first available package number
    recentDelivery.lastDelivery = oldHeader.header.firstEmptyDelivery; //Record on the last delivery file points to the new entry


    //Overwrite the last delivery file to point to the new entry
    fseek(LastDeliveryDescriptor,(recentDelivery.custNum - CUSTOMERBASE)*sizeof(Customer),SEEK_SET);
    fwrite(&recentDelivery,sizeof(Customer),1,LastDeliveryDescriptor);

    //Add new delivery to delivery file in the first available package number, place at index (package num - 500 + 1)*sizeof record, 1 skips the header
    fseek(DeliveryFileDescriptor,(oldHeader.header.firstEmptyDelivery + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
    fwrite(&newEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);

    //First available package number is incremented, placed in the header
    oldHeader.header.firstEmptyDelivery++;
    fseek(DeliveryFileDescriptor,0,SEEK_SET);
    fwrite(&oldHeader,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
}

DeliveryEntry ReadDeliveryFile(int mode, int packageNum) {
    DeliveryEntry returnEntry;

    if(mode == HEADER) {
        //Seek to start
        fseek(DeliveryFileDescriptor,0,SEEK_SET);
        fread(&returnEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
    } else {
        //Seek to position (Package number - 500 + 1)* sizeof entry 1 skips past the header
        fseek(DeliveryFileDescriptor,(packageNum + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
        fread(&returnEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
    }
    return returnEntry;
}

void PrintDeliveries(int mode, int custID) {
    DeliveryEntry del;
    if (mode == ALL) {
        fseek(DeliveryFileDescriptor, sizeof(DeliveryEntry), SEEK_SET);
        printf("All packages:\n");
        //Until all deliveries have been read
        while (fread(&del, sizeof(DeliveryEntry), 1, DeliveryFileDescriptor) != 0) {
            printf("Package number: %d Origin customer: %d Destination customer: %d Pickup time: %d Dropoff time: %d\n",
                   del.data.packageNumber,
                   del.data.activeCustomers[0].custNum,
                   del.data.activeCustomers[1].custNum,
                   del.data.pickupTime,
                   del.data.dropTime);
        }
    }
    else {
        int delivery = (GetLastDelivery(custID)).lastDelivery; //From last delivery file, get customer's last delivery
        DeliveryEntry deliveryRec;
        printf("Customer %d has:\n", custID);
        if(delivery == -1) printf("No deliveries\n");
        //Until no more deliveries left (earlier delivery should point to -1)
        while(delivery != -1) {
            //Find delivery in relative delivery file, print info
            fseek(DeliveryFileDescriptor,(delivery + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
            fread(&deliveryRec,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
            printf("Package number: %d Destination customer: %d Pickup time: %d Dropoff time: %d\n",
                   deliveryRec.data.packageNumber,
                   deliveryRec.data.activeCustomers[1].custNum,
                   deliveryRec.data.pickupTime,
                   deliveryRec.data.dropTime);
            //Move to next delivery
            delivery = deliveryRec.data.nextDelivery;
        }
    }
    printf("\n");
}



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
