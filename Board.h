//Header file for the board class
#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <cstdlib>

using namespace std;

class Board{

private:

	bool ** matrix;
	int height;
	int width;

	bool wrapAround;

	int iterations;
	int births;
	int deaths;
	//std::string fileName;	<--Do we need this in here????????

public:

	Board(bool wrapAround, int height, int width);
	Board(string filename);
	void toggle(int x, int y);
	void randomize();
	void runIteration();
	void runIteration(int runs);
	void addPattern(string fileName, int x, int y);
	void printBoard();
	void saveState(string fileName);
	bool ** getMatrix();
	int numNeigh(int r, int c);
	int countNeigh(int r, int c);
	int getHeight();
	int getWidth();
	int getIterations();
	int getBirths();
	int getDeaths();

};


#endif /* BOARD_H_ */
