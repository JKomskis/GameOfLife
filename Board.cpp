// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <iostream>
#include "board.h"
#include "methodsForEngine.cpp"
#include <fstream>

class Board {
private:
	bool wrapAround;
	int height;
	int width;
	bool** matrix;
	
public: 
	Board(bool wrapAround, int height, int width);

	
};



Board::Board(bool wrap, int h, int w)  {
	height = h;
	width = w;
	wrapAround = wrap;

	bool this->wrapAround = wrapAround;
	int this->height = height;
	int this->width = width;

	bool** matrix = new bool*[width];

	for (int i = 0; i < width; i++) {
		matrix[i] = new bool[height];
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i][j] = false;
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}

	for (int i = 0; i < height; i++) {
		delete matrix[i];
	}
	delete matrix;
}

void Board::toggle(int &x, int &y)	//make sure the pointer stuff works
{
	if (matrix[y][x] == false)
	{
		matrix[y][x] = true;
	}
	else {
		matrix[y][x] = false;
	}
}

bool** Board::getMatrix()
{
	return this->matrix;
}

//do we need to include int height/width???????????
void Board::saveState(string fileName, int height, int width)
{
	ofstream out(fileName);
	out << height << "\n";	//first line tells the program the height of the saved matrix
	out << width << "\n";	//second line tells the program the width of the saved matrix

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			out << matrix[i][j];
		}
		out << "\n";
	}
	out.close();
}

void Board::addPattern(string fileName, int x, int y)
{
	ifstream in;
	in.open(fileName.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}

	int heightOfSaved;
	int widthOfSaved;
	string line;

	//getline(fileName, line);
	//now convert to the heightOfSaved
	//getline(fileName, line);
	//now convert this to the widthOfSaved


}


int main()
{
	bool wrapAround;
	int width;
	int height;
	bool** matrix;

	std::cout << "What height do you want the board to have" << std::endl;
	std::cin >> height;
	
	std::cout << "What width do you want the board to have" << std::endl;
	std::cin >> width;

	Board test(true, height, width);
	test.getMatrix();

	std::cin >> width;


    return 0;
}

