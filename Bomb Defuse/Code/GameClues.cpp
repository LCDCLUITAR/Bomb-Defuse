#include "GameClues.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

GameClues::GameClues() {
	for (int i = 0; i < 4; i++)
		barcodeAnswer[i] = randomNumGen();
}

GameClues::~GameClues(){

}

void GameClues::getLockNum(int clueBarcode[]){
	clueBarcode = barcodeAnswer;
}

int randomNumGen() {
	srand(time(0));
	return rand() % 11;
}