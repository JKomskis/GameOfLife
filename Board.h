//Header file for the board class
#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <vector>
#include <string>
#include "Formats.h"
#include "Util.h"

class Board{

protected:

	std::vector<std::vector<bool>> matrix;
	int height;
	int width;

	bool wrapAround;

	int iterations;
	int births;
	int deaths;
	bool isSaved;
	set<int> birthRule;
	set<int> survivalRule;

public:

	Board(bool wrapAround, int height, int width);
	Board(std::string filename);
	void toggle(int x, int y);
	void randomize(double ratio);
	void runIteration();
	void runIteration(int runs);
	void addPattern(std::string fileName, int x, int y);
	void addPattern(std::vector<std::vector<bool>>, int x, int y);
	void printBoard();
	void saveState(std::string fileName);
	std::vector<std::vector<bool>>& getMatrix();
	int numNeigh(int r, int c);
	int countNeigh(int r, int c);
	int getHeight();
	int getWidth();
	int getIterations();
	int getBirths();
	int getDeaths();
	bool getIsSaved();

};


#endif /* BOARD_H_ */
