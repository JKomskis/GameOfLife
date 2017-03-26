// ConsoleApplication4.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h" idk what this is
#include <string>
#include <iostream>
#include "board.h"
#include <fstream>
#include <stdlib.h>


Board::Board(bool wrap, int h, int w)  {
	height = h;
	width = w;
	wrapAround = wrap;

	matrix = new bool*[width];

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

	//do we need this?
	for (int i = 0; i < height; i++) {
		delete matrix[i];
	}
	delete matrix;
}

Board::Board(string filename)
{

	wrapAround = false; //need to decide how to determine this from file

	//initialize matrix
	matrix = new bool*[width];

	for (int i = 0; i < width; i++) {
		matrix[i] = new bool[height];
	}

	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}

	string line;

	line = getline(in, line);
	height = atoi(line.c_str());

	line = getline(in, line);
	width = atoi(line.c_str());

	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i <width; i++)
		{
			matrix[row][i] = (bool)line[i];
		}
		row++;
	}

	in.close();

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

void Board::saveState(string fileName)
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

	line = getline(in, line);
	heightOfSaved = atoi(line.c_str());

	line = getline(in, line);
	widthOfSaved = atoi(line.c_str());

	//initialize new patternMatrix
	bool ** patternMatrix = new bool *[heightOfSaved];
	for(int i = 0; i < heightOfSaved; i++)
	{
		patternMatrix[i] = new bool [widthOfSaved];
	}

	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i <widthOfSaved; i++)
		{
			patternMatrix[row][i] = (bool)line[i];
		}
		row++;
	}

	//now place patternMatrix in matrix
	int countX = 0;
	int countY = 0;
	for(int i = x; i < widthOfSaved+x; i++)
	{
		for(int j = y; j < heightOfSaved + y; j++)
		{
			matrix[j][i] = patternMatrix[countY][countX];
			countY++;
		}
		countX++;
	}

	in.close();
}


int main()
{

	int height;
	int width;

	std::cout << "What height do you want the board to have" << std::endl;
	std::cin >> height;

	std::cout << "What width do you want the board to have" << std::endl;
	std::cin >> width;

	Board test(true, height, width);
	test.getMatrix();

	std::cin >> width;


    return 0;
}


