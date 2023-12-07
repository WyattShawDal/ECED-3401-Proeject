/*
 *Author: Cameron Archibald
 *Created on: 23 Nov 2023
 *Module Info: Contains function definitions for one way algorithm
*/

#ifndef EXAMPLE_ONE_WAY_H
#define EXAMPLE_ONE_WAY_H

#include "dependencies.h"

InstructionNode* pathCalculation(Cord start, Cord end);

InstructionNode* reverseInstructions(TileNode* end);

void searchAdjacentTiles(int direction, Cord position, TileNode * parent);










#endif //EXAMPLE_ONE_WAY_H
