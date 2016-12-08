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

class GameController : public GameClues {
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
	int cardResult[4];
	int cardCheck[4];
	int wireResult[2];
	int wireCheck;
	int strike;
public:
	GameController();
	~GameController();

	void resetController();
	int caseRotation(int, int, int&);
	int lockNumberCtrl(int, int, int&, int&);
	int cardNumberControl(int, int, int&, int&);
	int checkStage1();
	void randomNumGen(int range_max, int range_min, string);

	void getBarcodeArr(int arr[]);
	void getCardArr(int arr1[]);
	void getWireArr(int arr2[]);
	void setCaseLocation(int newLoc) {
		caseLocation = newLoc;
	}

	string isShapeResult(int, int, bool&);
	int isCardResult();
	int isWireResult();
	int shapeScreen(int, int);
	int cardScreen(int, int, int);
	int wireScreen(int, int);
	void fixShapeIndex();
	int getShapeToDraw() {
		return shapeToDraw;
	}
	void addStrike() { strike++; }
	int getStrikes() { return strike; }
};
