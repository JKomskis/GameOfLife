// ConsoleApplication4.cpp : Defines the entry point for the console application.
//


#include <string>
#include <iostream>
#include "board.h"
#include <fstream>
#include <stdlib.h>

using namespace std;

Board::Board(bool wrap, int h, int w)
{
	this->height = h;
	this->width = w;
	this->wrapAround = wrap;

	matrix = new bool*[height];

	for (int i = 0; i < height; i++)
	{
		matrix[i] = new bool[width];
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			matrix[i][j] = 0;
		}
	}
}

Board::Board(string filename)
{

	wrapAround = false; //need to decide how to determine this from file

	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}

	string line;

	getline(in, line);
	height = atoi(line.c_str());

	getline(in, line);
	width = atoi(line.c_str());

	//initialize matrix
	matrix = new bool*[height];
	for (int i = 0; i < height; i++)
	{
		matrix[i] = new bool[width];
	}

	int row = 0;
	string a;
	while(getline(in, line))
	{
		for(int i = 0; i <width; i++)
		{
			a = line[i];
			matrix[row][i] = atoi(a.c_str());
		}
		row++;
	}

	in.close();

}

void Board::toggle(int x, int y)
{
	if (matrix[y][x] == false)
	{
		matrix[y][x] = true;
	}
	else {
		matrix[y][x] = false;
	}
}

void Board::runIteration()
{
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
		{
			int count = 0;
			if(r != 0 && c != 0 && matrix[r-1][c-1])	//check that this cell is not on the top or left edge and that the number to the top left exists and is not 0
				count++;
			if(r != 0 && matrix[r-1][c])	//check that this cell is not at the top and that the cell directly above it exists and is not 0
				count++;
			if(r != 0 && c+1 != width && matrix[r-1][c+1])	//check that this cell is not at the top or at the right edge and that the cell to the top right exists and is not 0
				count++;
			if(c != 0 && matrix[r][c-1])	//check that this cell is not at the left edge and that the cell directly to the left exists and is not 0
				count++;
			if(c+1 != width && matrix[r][c+1])	//check that this cell is not on the right edge and that the cell directly to the right exists and is not 0
				count++;
			if(r+1 != height && c != 0 && matrix[r+1][c-1])	//check that this cell is not on the bottom edge and not on the left and that the cell to the bottom left exists and is not 0
				count++;
			if(r+1 != height && matrix[r+1][c])	//check if the cell is not on the bottom edge and that the cell below it exists and is not 0
				count++;
			if(r+1 != height && c+1 != width && matrix[r+1][c+1])	//check if the cell is not on the bottom or right edge and that the cell to the bottom right exists and is not 0
				count++;

			if(matrix[r][c])
			{
				if(count < 2)
					toggle(r, c);
				else if(count > 3)
					toggle(r, c);
			}
			else
			{
				if(count == 3)
					toggle(r, c);
			}
		}
	}
}

bool** Board::getMatrix()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}

	return this->matrix;
}

void Board::saveState(string fileName)
{
	ofstream out(fileName.c_str());
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

	int heightOfSaved = 0;
	int widthOfSaved = 0;
	string line;

	getline(in, line);					//read out the heightOfSaved
	heightOfSaved = atoi(line.c_str());	//make it an int
	getline(in, line);					//read out the widthOfSaved
	widthOfSaved = atoi(line.c_str());	//make it an int

	//initialize new patternMatrix
	bool **patternMatrix = new bool *[heightOfSaved];
	for(int i = 0; i < heightOfSaved; i++)
	{
		patternMatrix[i] = new bool[widthOfSaved];
	}

	//store file values into the patternMatrix
	int row = 0;
	string a = "";
	while(getline(in, line))
	{
		for(int i = 0; i <widthOfSaved; i++)
		{
			a = line[i];
			patternMatrix[row][i] = atoi(a.c_str());
		}
		row++;
	}

	//now place patternMatrix in matrix
	int countX = 0;
	int countY = 0;
	for(int i = y; i < y + heightOfSaved; i++)
	{
		for(int j = x; j < x + widthOfSaved; j++)
		{
			matrix[i][j] = patternMatrix[countX][countY];
			countY++;
		}
		countX++;
		countY = 0;
	}

	in.close();

}

int Board::getHeight()
{
	return height;
}

int Board::getWidth()
{
	return width;
}

int main()
{

	int height;
	int width;

	cout << "Height of board: ";
	cin >> height;

	cout << "Width of board: ";
	cin >> width;
	
	std::cout << "Add a board called test\n";
	Board test(false, height, width);
	
	std::cout << "Print out the matrix:\n";
	test.getMatrix();
	cout << endl;

	std::cout << "Add a pattern called patterntest.txt and then print out the matrix\n";
	test.addPattern("patterntest.txt", 1, 1);
	test.getMatrix();
	cout << endl;

	//test.getMatrix();
	//cout << endl;

	std::cout << "Try running the runIteration function: \n";
	test.runIteration();

	std::cout << "This is the matrix now\n";
	test.getMatrix();
	cout << endl;

    return 0;
}


