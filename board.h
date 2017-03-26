//Header file for the board class
#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <cstdlib>

using namespace std;

class Board{

private:

	bool ** matrix;
	bool wrapAround;
	int height;
	int width;
	//std::string fileName;	<--Do we need this in here????????

public:

	Board(bool wrapAround, int height, int width);
	Board(string filename);
	void toggle(int &x, int &y);
	void runIteration();
	void runIteration(int runs);
	void addPattern(string fileName, int x, int y);
	bool ** getMatrix();
	void saveState(string fileName);

};


#endif /* BOARD_H_ */
