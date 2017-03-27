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
			if(r != 0 && c != 0 && matrix[r-1][c-1])
				count++;
			if(r != 0 && matrix[r-1][c])
				count++;
			if(r != 0 && c+1 != width && matrix[r-1][c+1])
				count++;
			if(c != 0 && matrix[r][c-1])
				count++;
			if(c+1 != width && matrix[r][c+1])
				count++;
			if(r+1 != height && c != 0 && matrix[r+1][c-1])
				count++;
			if(r+1 != height && matrix[r+1][c])
				count++;
			if(r+1 != height && c+1 != width && matrix[r+1][c+1])
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

	getline(in, line);
	heightOfSaved = atoi(line.c_str());
	getline(in, line);
	widthOfSaved = atoi(line.c_str());

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

/*int main()
{

	int height;
	int width;

	cout << "Height of board: ";
	cin >> height;

	cout << "Width of board: ";
	cin >> width;

	Board test(false, height, width);

	test.addPattern("patterntest.txt", 1, 1);

	test.getMatrix();
	cout << endl;

	test.runIteration();

	test.getMatrix();

    return 0;
}*/


