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
const char* vehicleFile = "VehicleFile.dat";

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
    else if(OPERATION == READ_WRITE_BINARY_NEW) {
        return (*FPointer = fopen(fileName, "wb+")) != NULL;
    }
    else if(OPERATION == READ_WRITE_BINARY_EXISTING) {
        return (*FPointer = fopen(fileName,"rb+")) != NULL;
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
    fwrite(&xLength, sizeof(int), 1, BuildFileDescriptor);
    fwrite(&yLength, sizeof(int), 1, BuildFileDescriptor);

    do {
        printf("Enter Direction of first street (1 for E, 2 for W): ");
        fscanf(stdin, "%d", &streetDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(!(streetDirection >= STREET_E && streetDirection <= STREET_W));

    fwrite(&streetDirection, sizeof(int), 1, BuildFileDescriptor);

    do {
        printf("Enter Direction of first street (4 for N, 5 for S): ");
        fscanf(stdin, "%d", &avenueDirection);
        (void) getchar(); /* Eat last EOLN */

    }while(!(avenueDirection >= AVENUE_N && avenueDirection <= AVENUE_S));

    fwrite(&avenueDirection, sizeof(int), 1, BuildFileDescriptor);

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
        fwrite(&building, sizeof(building), 1, BuildFileDescriptor);

        printf("Enter Building's Label (XX to exit): ");
        (void) fscanf(stdin, "%s", building.buildingLabel);
        (void) getchar(); /* Eat last EOLN */
        building.location.x = building.buildingLabel[0] - 'A';
        building.location.y = building.buildingLabel[1] - 'A';
        if(strcmp(building.buildingLabel, "XX") == 0) {
            building.quad = INVALID_QUAD;
            building.type = INVALID_TYPE;
            printf("Exit Code Chosen\n");
            fwrite(&building, sizeof(building), 1, BuildFileDescriptor);

            break;
        }

    }

    /* End with 0 0 for read function to know it's made it to the end */
    xLength = 0;
    fwrite(&xLength, sizeof(int), 1, BuildFileDescriptor);
    fwrite(&xLength, sizeof(int), 1, BuildFileDescriptor);

    fclose(BuildFileDescriptor);

    (void) getchar(); /* Eat last EOLN */
}

void ReadBuildFile(int * streetDir, int * avenueDir) {
    OpenTargetFile(READ_BINARY, "GenerationFile.dat", &BuildFileDescriptor);
    int numBuildingsX, numBuildingsY;
    char streetDirection, avenueDirection;
    BuildingData building;

    fread(&numBuildingsX, sizeof(int), 1, BuildFileDescriptor);
    fread(&numBuildingsY, sizeof(int), 1, BuildFileDescriptor);
    MAX_COLS = numBuildingsX * 4 + 1; //Building widths are 3, with a road to the left are 4, then add one road to the right
    MAX_ROWS = numBuildingsY * 4 + 1; //Same justification, but for adding road below
    cellHeight = screenWidth/MAX_COLS;
    cellWidth = screenWidth/MAX_COLS;
    //printf("You entered %d x %d, which forms %d Cols and %d Rows\n", numBuildingsX, numBuildingsY, MAX_COLS, MAX_ROWS);

    fread(streetDir, sizeof(int), 1, BuildFileDescriptor);
    fread(avenueDir, sizeof(int), 1, BuildFileDescriptor);

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

    fread(&building, sizeof(building), 1, BuildFileDescriptor);
    while (strcmp(building.buildingLabel, "XX") != 0)
    {
        ConvertBuildingCords(&building);
        //printf("Building XY: %d %d\nBuilding Quadrant: %d\nBuilding Type: %d\n", building.location.x, building.location.y, building.quad, building.type);
        /* Read next record */
        AddBuilding(&StableList, &ChargerList, building);
        fread(&building, sizeof(building), 1, BuildFileDescriptor);
    }
    (void) getchar();
    fclose(BuildFileDescriptor);
}
/*
 * Event File is a sequential, tab seperated file. So, to read it we can just do normal reading procedures while parsing out data
 */
void ReadEventFile(char *fileName) {
    char line[MAXSTRLEN];
    char eventName;
    int time, origin, destination, weight;
    EventData currentEvent;
    OpenTargetFile(READ_TEXT, fileName, &BuildFileDescriptor);

    fgets(line, sizeof(line), BuildFileDescriptor);
    while(fgets(line, sizeof(line), BuildFileDescriptor) != NULL) {
        sscanf(line, "%d\t%c\t%d\t%d\t%d\t", &time, &eventName, &origin, &destination, &weight);

        //printf("Time = %d, Event = %c, Origin = %d, Destination = %d, Weight = %d\n", time,eventName, origin, destination, weight);
        currentEvent.time = time;
        currentEvent.type = eventName;
        currentEvent.originID = origin;
        currentEvent.destinationID = destination;
        currentEvent.weight = weight;
        AddEvent(&EventList, currentEvent);

    }
    fclose(BuildFileDescriptor);
}

void CreateRelativeCustomerFile() {
    Customer customerInfo;

    LastDeliveryEntry last; //Holds structs to place in lastDeliveries file
    last.lastDelivery = -1; //No deliveries occured yet
    OpenTargetFile(READ_WRITE_BINARY_NEW,lastDeliveries,&LastDeliveryDescriptor);

    OpenTargetFile(READ_TEXT, customerFile, &BuildFileDescriptor);
    OpenTargetFile(WRITE_BINARY, relativeCustomerFile, &RelCustomerFileDescriptor);
    char line[sizeof(Customer)];

    fgets(line, MAXSTRLEN, BuildFileDescriptor); // Read and discard text header
    // Read and display each record sequentially
    while (fgets(line, sizeof(line), BuildFileDescriptor) != NULL) {
        sscanf(line, "%d\t%s\t%s\t%s\t%s\t%d", &customerInfo.custNum, customerInfo.firstName, customerInfo.lastName, customerInfo.building, customerInfo.entrance, &customerInfo.floor);
        //write out customer info into a binary file for relative accessing
        fwrite(&customerInfo, sizeof(Customer), 1, RelCustomerFileDescriptor);

        //Create an entry in the lastDelivery file with ID corresponding to the txt cust file, and last delivery -1
        last.custNum = customerInfo.custNum;
        fwrite(&last,sizeof(LastDeliveryEntry),1,LastDeliveryDescriptor);
    }
    // Close the file
    fclose(BuildFileDescriptor);
    fclose(RelCustomerFileDescriptor);
}

void CreateDeliveryFile() {
    OpenTargetFile(READ_WRITE_BINARY_NEW,deliveryFile,&DeliveryFileDescriptor);
    //Write header
    DeliveryEntry h = {.header.firstEmptyDelivery = 500};
    fwrite(&h,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);

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
    fseek(LastDeliveryDescriptor,(recentDelivery.custNum - CUSTOMERBASE)*sizeof(LastDeliveryEntry),SEEK_SET);
    fwrite(&recentDelivery,sizeof(LastDeliveryEntry),1,LastDeliveryDescriptor);

    //Add new delivery to delivery file in the first available package number, place at index (package num - 500 + 1)*sizeof record, 1 skips the header
    fseek(DeliveryFileDescriptor,(oldHeader.header.firstEmptyDelivery + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
    fwrite(&newEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);

    //First available package number is incremented, placed in the header
    oldHeader.header.firstEmptyDelivery++;
    fseek(DeliveryFileDescriptor,0,SEEK_SET);
    fwrite(&oldHeader,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
}

LastDeliveryEntry GetLastDelivery(int ID) {
    //Return entry from the file containing ID's and last deliveries
    LastDeliveryEntry last;
    fseek(LastDeliveryDescriptor,(ID - CUSTOMERBASE)*sizeof(LastDeliveryEntry),SEEK_SET);
    fread(&last,sizeof(LastDeliveryEntry),1,LastDeliveryDescriptor);
    return last;
}

DeliveryEntry ReadDeliveryFile(int mode, int packageNum) {
    DeliveryEntry returnEntry;

    if(mode == HEADER) {
        //Seek to start
        fseek(DeliveryFileDescriptor,0,SEEK_SET);
        fread(&returnEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
    } else {
        //Seek to position (Package number - 500 + 1)* sizeof entry, + 1 skips past the header
        fseek(DeliveryFileDescriptor,(packageNum + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
        fread(&returnEntry,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
    }
    return returnEntry;
}

void PrintDeliveries(int mode, int ID) {
    DeliveryEntry del;
    LastDeliveryEntry last;
    switch (mode) {
        case ALL: //Print all packages
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
            break;
        case CUSTOMER: //Print all packages from one customer (ID)
            last = GetLastDelivery(ID);
            if (last.custNum != ID) { //if GetLastDelivery doesn't return a record equal to the desired custID, that ID doesn't exist
                printf("%d is not a valid customer ID\n\n", ID);
                return;
            }
            int delivery = last.lastDelivery; //From last delivery file, get customer's last delivery
            DeliveryEntry deliveryRec;
            printf("Customer %d has:\n", ID);
            if (delivery == -1) printf("No deliveries\n");
            //Until no more deliveries left (earlier delivery should point to -1)
            while (delivery != -1) {
                //Find delivery in relative delivery file, print info
                fseek(DeliveryFileDescriptor, (delivery + 1 - DELIVERYBASE) * sizeof(DeliveryEntry), SEEK_SET);
                fread(&deliveryRec, sizeof(DeliveryEntry), 1, DeliveryFileDescriptor);
                printf("Package number: %d Destination customer: %d Pickup time: %d Dropoff time: %d\n",
                       deliveryRec.data.packageNumber,
                       deliveryRec.data.activeCustomers[1].custNum,
                       deliveryRec.data.pickupTime,
                       deliveryRec.data.dropTime);
                //Move to next delivery
                delivery = deliveryRec.data.nextDelivery;
            }
            break;
        case PACKAGE: //Print a specific package number (ID)
            fseek(DeliveryFileDescriptor,(ID + 1 - DELIVERYBASE)*sizeof(DeliveryEntry),SEEK_SET);
            fread(&deliveryRec,sizeof(DeliveryEntry),1,DeliveryFileDescriptor);
            if(deliveryRec.data.packageNumber != ID)
                //If delivery record does not match passed in ID, record doesn't exist
                printf("Package %d does not exist\n", ID);
            else
                printf("Orign customer: %d Destination customer: %d Time: %d\n",
                       deliveryRec.data.activeCustomers[0].custNum,
                       deliveryRec.data.activeCustomers[1].custNum,
                       deliveryRec.data.dropTime - deliveryRec.data.pickupTime
                       );
            break;
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

void CreateVehicleFile(int MODE) {
    int choice;
    if(MODE == KEEP_FILE) {
        //User chooses whether to overwrite or create new file
        printf("Overwrite vehicle file? 1 for YES, 0 for NO: ");
        scanf("%d", &choice);
    } else
        choice = NEW_FILE; //Overwrite file

    if(choice) {
        //Open to overwrite or make new file
        OpenTargetFile(READ_WRITE_BINARY_NEW,vehicleFile,&VehicleFileDescriptor);

        VehicleEntry entry;
        //No entries in the file yet, intialize the header to point to -1 for every EVIN
        for(int i = 0;i < MAX_VEHICLES_FILE;i++)
            entry.header.recentEntry[i] = -1;
        entry.header.nextOpenEntry = 0;
        fwrite(&entry,sizeof(VehicleEntry),1,VehicleFileDescriptor);
        fclose(VehicleFileDescriptor);
        //Open to read/write new file

    }
    OpenTargetFile(READ_WRITE_BINARY_EXISTING,vehicleFile,&VehicleFileDescriptor);
    if(!choice)
        PrintVehicleFile(-1); //Print contents of file if it isn't overwritten
}

void AddToVehicleFile(VehicleEntry newEntry) {
    VehicleEntry oldHeader = GetVehicleEntry(HEADER,0);

    int oldNext = oldHeader.header.recentEntry[newEntry.data.EVIN - EVINBASE]; //Given EVIN's old most recent entry
    newEntry.data.nextEntry = oldNext; //New entry points to former most recent entry

    int writeIndex = oldHeader.header.nextOpenEntry; //Index to write to is fetched from the header
    oldHeader.header.recentEntry[newEntry.data.EVIN - EVINBASE] = writeIndex; //Update header to say EVIN's most recent delivery is the position being written to
    oldHeader.header.nextOpenEntry++; //Header now indicates that position is filled

    fseek(VehicleFileDescriptor,0,SEEK_SET); //Overwrite the header with new info
    fwrite(&oldHeader,sizeof(VehicleEntry),1,VehicleFileDescriptor);

    //Seek to index being written to + 1 to skip over the header.
    fseek(VehicleFileDescriptor,(writeIndex + 1)*sizeof(VehicleEntry),SEEK_SET);
    fwrite(&newEntry,sizeof(VehicleEntry),1,VehicleFileDescriptor);

    printf("\n");


}

VehicleEntry GetVehicleEntry (int MODE, int recordNum) {
    VehicleEntry returnEntry;
    int index;
    switch(MODE) {
        case HEADER: //Get the header entry
            fseek(VehicleFileDescriptor,0,SEEK_SET);
            fread(&returnEntry,sizeof(VehicleEntry),1,VehicleFileDescriptor);
            break;
        case ENTRY:
            fseek(VehicleFileDescriptor,(recordNum + 1)*sizeof(VehicleEntry),SEEK_SET);
            fread(&returnEntry,sizeof(VehicleEntry),1,VehicleFileDescriptor);
            break;
        case VEHICLE_NUMBER:
            fseek(VehicleFileDescriptor,0,SEEK_SET);
            fread(&returnEntry,sizeof(VehicleEntry),1,VehicleFileDescriptor); //Get header
            index = returnEntry.header.recentEntry[recordNum - EVINBASE];
            if(index != -1) {
                //Entry exists for given EVIN
                fseek(VehicleFileDescriptor,(index + 1)*sizeof(VehicleEntry),SEEK_SET);
                fread(&returnEntry,sizeof(VehicleEntry),1,VehicleFileDescriptor);
            }
            else
                returnEntry.data.EVIN = -1; //No entry for given EVIN
            break;
    }

    return returnEntry;
};

void PrintVehicleFile(int EVIN) {
    int index = 0;
    VehicleEntry current;
    if(EVIN == -1) { //Passed in EVIN -1, print all vehicle records
        printf("Vehicle file contains:\n");
        fseek(VehicleFileDescriptor,sizeof(VehicleEntry),SEEK_SET);
        while(fread(&current,sizeof(VehicleEntry),1,VehicleFileDescriptor) != 0) {
            PrintVehicleEntry(current);
        }
    }
    else if(!IsValidEVIN(EVIN)) { //EVIN out of range
        printf("EVIN %d out of range\n",EVIN);
    }
    else if((current = GetVehicleEntry(VEHICLE_NUMBER,EVIN)).data.EVIN != -1) {//Get first record, if it exists print all records for that EVIN
        PrintVehicleEntry(current);
        index = current.data.nextEntry;
        while(index != -1) {
            fseek(VehicleFileDescriptor,(index + 1)*sizeof(VehicleEntry),SEEK_SET);
            fread(&current,sizeof(VehicleEntry),1,VehicleFileDescriptor);
            PrintVehicleEntry(current);
            index = current.data.nextEntry;
        }
    } else {//GetVehicleEntry returned -1, meaning no entry for that EVIN exists
        printf("No records exist for EVIN %d\n",EVIN);
    }
    printf("\n");
}

void PrintVehicleEntry(VehicleEntry entry) {
    printf("EVIN: %d Next Entry: %d Stable: %s Ticks moving: %d\n",
           entry.data.EVIN,
           entry.data.nextEntry,
           entry.data.stats.lastStable.buildingLabel,
           entry.data.stats.ticksMoving);
}

void RecordFinalVehicleStates(void) {
    VehicleEntry newEntry;
    AEDVNode * currentVehicle = InactiveList;
    while(currentVehicle != NULL) {
        newEntry.data.EVIN = currentVehicle->data.EVIN;
        newEntry.data.stats = currentVehicle->data.stats;
        AddToVehicleFile(newEntry);

        currentVehicle = currentVehicle->next;
    }
};

void CloseFiles(void) {
    fclose(VehicleFileDescriptor);
    fclose(DeliveryFileDescriptor);
}

bool IsValidEVIN(int EVIN) {
    return ((EVIN > EVINBASE) && (EVIN < EVINBASE + MAX_VEHICLES_FILE));
}

