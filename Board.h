//Header file for the board class
#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <vector>
#include <string>
#include "Formats.h"
#include "Util.h"

class Board{

protected:	//protected variables

	std::vector<std::vector<bool>> matrix;	//boolean matrix that will store cells
	int height;								//height of matrix
	int width;								//width of matrix
	bool wrapAround;						//allow the board to wrap around or not
	int iterations;							//number of iterations that have been run
	int births;								//number of births so far 
	int deaths;								//number of deaths so far
	bool isSaved;							//once the board has been modified, this is false
	set<int> birthRule;						//sets the birthRule
	set<int> survivalRule;					//sets the survivalRule

public:	//public functions and variables

	Board(bool wrapAround, int height, int width);	//a constructor for the Board class if height, width, and wraparound options are chosen
	Board(std::string filename);					//a constructor for the board class if just a filename is given
	void toggle(int x, int y);						//toggles the cell from true to false or false to true
	void randomize(double ratio);					//allows a board to be randomly generated
	void runIteration();							//runs one iteration (for example, when the user presses the "Enter" key in the GameOfLife)
	void runIteration(int runs);					//runs the interation the correct number of times
	void addPattern(std::string fileName, int x, int y);	//allows the user to add an existing pattern to the board by calling with the filename, along with an x and y position
	void addPattern(std::vector<std::vector<bool>>, int x, int y);	//allows the user to add an existing pattern to the board by calling with the actual bool matrix, along with an x and y position
	void printBoard();								//prints the board as a matrix of 1s and 0s - good for testing purposes
	void saveState(std::string fileName);			//save a given state or board, given a name for the file
	std::vector<std::vector<bool>>& getMatrix();	//returns the matrix
	int numNeigh(int r, int c);						//counts how many live neighbours a given cell has
	int getHeight();								//returns the height of the board
	int getWidth();									//returns the width of the board
	int getIterations();							//returns the number of iterations that were run
	int getBirths();								//returns the number of births that occurred
	int getDeaths();								//returns the number of deaths that occurred
	bool getIsSaved();								//returns a boolean value of if the board has been saved
	void setBirthRule(set<int> input);				//set a new birthing rule set
	void setSurvivalRule(set<int> input);			//set a new survival rule set
	string getBirthRule();					//return a formatted rule string for births
	string getSurvivalRule();				//return a formatted rule string for survival
};


#endif /* BOARD_H_ */
