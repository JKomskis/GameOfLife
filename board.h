//Header file for the board class
#include <string>
#include <cstdlib>
#include <iostream>
#include <fstream>
using namespace std;

class Board{
private:
	Board(bool wrapAround, int height, int width);
	bool ** matrix;
	bool wrapAround;
	int height;
	int width;
	//std::string fileName;	<--Do we need this in here????????
	
	
	
public:
	void toggle(int x, int y);
	void runIteration();
	void runIteration(int runs);
	void addPattern(string fileName, int x, int y);
	bool ** getMatrix();
	void saveState(string fileName, int height, int width);
	
};

Board(bool wrapAround, int height, int width)
{
	bool this->wrapAround = wrapAround;
	int this->height = height;
	int this->width = width;	
}

Board(string fileName)
{
	std::string this -> fileName = fileName;
}