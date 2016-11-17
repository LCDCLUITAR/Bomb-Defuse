#pragma once
class GameClues{
protected:
	int barcodeAnswer[4];
public:
	GameClues();
	~GameClues();

	void getLockNum(int[]);
};

int randomNumGen();
