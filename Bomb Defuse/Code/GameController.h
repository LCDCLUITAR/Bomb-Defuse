#pragma once
#include <windows.h>
#include <windowsx.h>

#include "defines.h"
#include "abort.h"
#include "gamerenderer.h"

#include "imagefilenamelist.h"
#include "debug.h"
#include "timer.h"
#include "sprite.h"
#include "object.h"
#include "GameClues.h"

class GameController : public GameClues{
private:
	int caseLocation;  // 0: Front, 1: Top, 2: Left, 3: Right, 4: Bottom  'Rotation Location of Briefcase'

	int lockNum0;
	int lockNum1;
	int lockNum2;
	int lockNum3;
	int barcodeNum[5];

	int shapeResult[4];
	int shapeToDraw;
	int shapeRepeats[6];

	int strike;
public:
	GameController();
	~GameController();

	int caseRotation(int, int, int&);
	int lockNumberCtrl(int, int, int&, int&);
	int checkStage1();
	void randomNumGen(int range_max, int range_min, string);

	void getBarcodeArr(int arr[]);
	void setCaseLocation(int newLoc) {
		caseLocation = newLoc;
	}

	string isShapeResult(int, int, bool&);
	int shapeScreen(int, int);
	void fixShapeIndex();
	int getShapeToDraw() {
		return shapeToDraw;
	}
};
