#include "GameController.h"
#include <stdlib.h>     /* srand, rand */
#include <stdio.h>  
#include <time.h>       /* time */

GameController::GameController(){
	caseLocation = 0;
	lockNum0 = 9;
	lockNum1 = 9;
	lockNum2 = 9;
	lockNum3 = 9;
	strike = 0;
	randomNumGen(9, 0, "barcode");
	randomNumGen(23, 40, "shapes");
	fixShapeIndex();
}					 
GameController::~GameController(){

}


int GameController::caseRotation(int x, int y, int &currLoc) {
	// Rotations from Front View
	if (currLoc == 0) {
		// Top Arrow Selected from Front View
		if (x >= 0 && y >= 100 && y <= 144) {				// Briefcase Top
			caseLocation = currLoc = 1; // Top Location
			return 5;
		}
		// Bottom Arrow Selected
		if (x >= 0 && y >= 680 && y <= 768) {			// Briefcase Front
			caseLocation = currLoc = 4;
			return 8;
		}
		// Left Arrow Selected
		if (x <= 160 && y >= 100 && y <= 768) {			// Briefcase Left
			caseLocation = currLoc = 2;
			return 7;
		}
		// Right Arrow Selected
		if (x >= 865 && y >= 100 && y <= 768) {			// Briefcase Right
			caseLocation = currLoc = 3;
			return 6;
		}
	}
	// Rotations from Top View
	else if (currLoc == 1) {
		// Bottom Arrow Selected
		if (x >= 0 && y >= 600 && y <= 768) {			// Briefcase Front
			caseLocation = currLoc = 0;
			return 4;
		}
	}
	// Rotations from Left View
	else if (currLoc == 2) {
		// Right Arrow Selected
		if (x >= 800 && y >= 100 && y <= 768) {			// Briefcase Front
			caseLocation = currLoc = 0;
			return 4;
		}
	}
	// Rotations from Right View
	else if (currLoc == 3) {
		// Left Arrow Selected
		if (x <= 160 && y >= 100 && y <= 768) {			// Briefcase Front
			caseLocation = currLoc = 0;
			return 4;
		}
	}
	// Rotations from Bottom View
	else if (currLoc == 4) {
		// Top Arrow Selected
		if (x >= 0 && y >= 100 && y <= 160) {				// Briefcase Front
			caseLocation = currLoc = 0;
			return 4;
		}
	}
	return -1;
}

int GameController::lockNumberCtrl(int x, int y, int& currLoc, int& step){
	//lock number control
	if (x >= 420 && x <= 445 && y <= 450 && y >= 380 && currLoc == 1) {
		lockNum0++;
		step = lockNum0;
		if (lockNum0 == 18)
			lockNum0 = 8;
		return 0;

	}//number0 controller
	if (x >= 455 && x <= 485 && y <= 450 && y >= 380 && currLoc == 1) {
		lockNum1++;
		step = lockNum1;
		if (lockNum1 == 18)
			lockNum1 = 8;
		return 1;
	}//number1 controller
	if (x >= 490 && x <= 520 && y <= 450 && y >= 380 && currLoc == 1) {
		lockNum2++;
		step = lockNum2;
		if (lockNum2 == 18)
			lockNum2 = 8;
		return 2;
	}//number2 controller
	if (x >= 525 && x <= 555 && y <= 450 && y >= 380 && currLoc == 1) {
		lockNum3++;
		step = lockNum3;
		if (lockNum3 == 18)
			lockNum3 = 8;
		return 3;
	}//number3 controller

	return -1;
}

int GameController::checkStage1(){
	//if ((lockNum0 == barcodeNum[4]+9) && (lockNum1 == barcodeNum[1]+9) && (lockNum2 == barcodeNum[2]+9) && (lockNum3 == barcodeNum[3]+9)) {
	if ((lockNum0 == 9) && (lockNum1 == 9) && (lockNum2 == 9) && (lockNum3 == 10)) {
		return 1;
	}
	else if(strike >= 2) {
		return 2;
	}
	else {
		strike = strike + 1;
		return 0;
	}
}

void GameController::getBarcodeArr(int arr[]) {
	for (int i = 0; i < 5; i++)
		arr[i] = barcodeNum[i];
}

void GameController::randomNumGen(int range_max, int range_min, string clue){
	srand((unsigned)time(0));
	// Generate random numbers in the half-closed interval  
	// [range_min, range_max). In other words,  
	// range_min <= random number < range_max  
	if(clue == "barcode")
		for (int i = 0; i < 5; i++)
			barcodeNum[i] = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
	else if (clue == "shapes") {
		for (int i = 0; i < 5; i++)
			shapeResult[i] = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
	}
}

int GameController::shapeScreen(int x, int y) {
	int shapeIndex = -1;
	// Circle
	if (x >= 232 && x <= 265 && y >= 363 && y <= 394)
		shapeIndex = 29;// "Circle";  29 - 31
	// Triangle
	if (x >= 288 && x <= 320 && y >= 363 && y <= 394)
		shapeIndex = 35;// "Triangle";  35 - 37
	// Star
	if (x >= 345 && x <= 380 && y >= 363 && y <= 394)
		shapeIndex = 32;// "Star";  32 - 34
	// Trapezoid
	if (x >= 232 && x <= 265 && y >= 420 && y <= 449)
		shapeIndex = 38;// "Trapezoid";  38 - 40
	// Plus
	if (x >= 288 && x <= 320 && y >= 420 && y <= 449)
		shapeIndex = 26;// "Plus";  26 - 28
	// Square
	if (x >= 345 && x <= 380 && y >= 420 && y <= 449)
		shapeIndex = 23;// "Square";  23 - 25

	return shapeIndex;
}

// Return 0: green 1: yellow 2: red
string GameController::isShapeResult(int shapeIndex, int position, bool& clear) {
	static int shapeRep[6];
	static int x = 0;
	
	x++;
	if (clear == true) {
		x = 0;
		clear = false;
	}
	if (x <= 1) {
		for (int i = 0; i < 6; i++)
			shapeRep[i] = shapeRepeats[i];
	}

	for (int i = 0; i < 5; i++) {
		if (shapeIndex == shapeResult[i] && i == (position - 1)) {
			//shapeRep[i]--;
			return "Green";
		}
	}
	for (int i = 0; i < 5; i++){
		if (shapeIndex == shapeResult[i]/* && shapeRep[i] >= 1*/)
			return "Yellow";
	}

	return "Red";
}

void GameController::fixShapeIndex() {
	int shapeResultInt = 0;

	for (int i = 0; i < 4; i++) {
		if (shapeResult[i] >= 23 && shapeResult[i] <= 25) {			// Square
			shapeResult[i] = 23;
			shapeRepeats[0]++;
		}
		else if (shapeResult[i] >= 26 && shapeResult[i] <= 28) {	//Plus
			shapeResult[i] = 26;
			shapeRepeats[1]++;
		}
		else if (shapeResult[i] >= 29 && shapeResult[i] <= 31) {	// Circle
			shapeResult[i] = 29;
			shapeRepeats[2]++;
		}
		else if (shapeResult[i] >= 32 && shapeResult[i] <= 34) {	// Star
			shapeResult[i] = 32;
			shapeRepeats[3]++;
		}
		else if (shapeResult[i] >= 35 && shapeResult[i] <= 37) {	// Triangle
			shapeResult[i] = 35;
			shapeRepeats[4]++;
		}
		else if (shapeResult[i] >= 38 && shapeResult[i] <= 40) {	// Trapezoid
			shapeResult[i] = 38;
			shapeRepeats[5]++;
		}
		else
			shapeResult[i] = -1;
	}
}