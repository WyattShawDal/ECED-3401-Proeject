/*
 *Author: Wyatt Shaw 
 *Created on: 2023-11-22
 *Module Info:
*/

#ifndef EXAMPLE_FILES_H
#define EXAMPLE_FILES_H
#include "dependencies.h"


#define TRUNCATE(name)	name[strlen(name)-1] = '\0'
#define NAMELEN	16	/* Filename length */
bool OpenTargetFile(Operation OPERATION, const char *fileName, FILE **FPointer);

void GenerateBuildFile();
void ReadBuildFile(int * streetDir, int * avenueDir);
void ReadEventFile(char *fileName);
void ReadCustomerFile(int originCode, int destinationCode);
void ReadTextCustomerFile();
Customer GetCustomerInfo(int CustomerID);



bool CheckDirectionChar(char direction);
bool CheckBuildingChar(char building);

/* File Pointers */


#endif //EXAMPLE_FILES_H
