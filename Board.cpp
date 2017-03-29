// ConsoleApplication4.cpp : Defines the entry point for the console application.
//
//This is a test

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
		cerr << "File not opened '" << filename << "'" << endl;
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

void Board::toggle(int r, int c)	//toggles the cell from true to false or false to true
{
	if (matrix[r][c] == false)	//if the cell at r,c is false, make it true
	{
		matrix[r][c] = true;
	}
	else	//otherwise, make it false
	{
		matrix[r][c] = false;
	}
}

int Board::numNeigh(int r, int c)
{
	int count = 0;

	if (r != 0 && c != 0 && matrix[r - 1][c - 1] == 1)	//check that this cell is not on the top or left edge and that the number to the top left exists and is not 0
		count++;
	if (r != 0 && matrix[r - 1][c] == 1)	//check that this cell is not at the top and that the cell directly above it exists and is not 0
		count++;
	if (r != 0 && c + 1 != width && matrix[r - 1][c + 1] == 1)	//check that this cell is not at the top or at the right edge and that the cell to the top right exists and is not 0
		count++;
	if (c != 0 && matrix[r][c - 1] == 1)	//check that this cell is not at the left edge and that the cell directly to the left exists and is not 0
		count++;
	if (c + 1 != width && matrix[r][c + 1] == 1)	//check that this cell is not on the right edge and that the cell directly to the right exists and is not 0
		count++;
	if (r + 1 != height && c != 0 && matrix[r + 1][c - 1] == 1)	//check that this cell is not on the bottom edge and not on the left and that the cell to the bottom left exists and is not 0
		count++;//1
	if (r + 1 != height && matrix[r + 1][c] == 1)	//check if the cell is not on the bottom edge and that the cell below it exists and is not 0
		count++;
	if (r + 1 != height && c + 1 != width && matrix[r + 1][c + 1] == 1)	//check if the cell is not on the bottom or right edge and that the cell to the bottom right exists and is not 0
		count++;


	if(wrapAround) {                                                               //check to make sure this board has wrapAround enabled
		if (r = 0 && matrix[height - 1][c] == 1) {               //check if this cell is on the top row (but not necessarily in a corner). If it is, it should count the bottom row and same column as a neighbor.
			count++;
		}
		if (c = 0 && matrix[r][width - 1] == 1) {                //check if this cell is on the left column (but not necessarily a corner). Wraps around the left column to the right.
			count++;
		}
		if (r + 1 == height && matrix[0][c]) {                              //check if this cell is on the bottom row (but not necessarily a corner). Wraps around the bottom row to the top.
			count++;
		}
		if (c + 1 == width && matrix[r][0]) {                               //check if this cell is on the right column (but not necessarily a corner). Wraps around the right column to the left.
			count++;
		}
		if (c == 0 && r == 0 && matrix[height - 1][width - 1] == 1) {                  //check if this cell is on the top left corner. Wraps around the top left corner to the bottom right corner.
			count++;
		}
		if (c == 0 && r + 1 == height && matrix[0][width - 1] == 1) {                  //check if this cell is on the bottom left corner. Wraps around the bottom left corner to the top right corner.
			count++;
		}
		if (c + 1 == width && r == 0 && matrix[height - 1][0] == 1) {                  //check if this cell is on the top right corner. Wraps around the top right corner to the bottom left corner.
			count++;
		}
		if (c + 1 == width && r + 1 == height && matrix[0][0] == 1) {                  //check if this cell is on the bottom right corner. Wraps around the bottom right corner to the top left corner.
			count++;
		}

	}

	return count;
}



void Board::runIteration()
{

	int **nMatrix = new int*[height];

	for (int i = 0; i < height; i++)
	{
		nMatrix[i] = new int[width];
	}
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
		{
			nMatrix[r][c] = numNeigh(r, c);
			//std::cout << nMatrix[r][c] << " ";
		}
		//std::cout << endl;
	}
	//std::cout << "Now we are looking at if the cells do what they are supposed to:\n";
	for(int r = 0; r < height; r++)
	{
		for(int c = 0; c < width; c++)
		{
			if(matrix[r][c] == 1)	//if the cell is alive (equal to 1)
			{
				if(nMatrix[r][c] < 2)	//Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
				{
					toggle(r, c);
					//std::cout << "For " << r << ", " << c << " we changed it to a 0 because we had <2: " << nMatrix[r][c] <<endl;
				}
				else if(nMatrix[r][c] > 3)	//Any live cell with more than three live neighbours dies, as if by overpopulation.
				{
					toggle(r, c);
					//std::cout << "For " << r << ", " << c << " we changed it to a 0 because we had >3: " << nMatrix[r][c] << endl;
				}
				//if neither of these is true, the cell stays alive
			}
			else	//if the cell is dead (equal to 0)
			{
				if(nMatrix[r][c] == 3)	//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
				{
					toggle(r, c);
					//std::cout << "For " << r << ", " << c << " we changed it to a 1 because we had ==3: " << nMatrix[r][c] << endl;
				}
			}
		}
	}

}

void Board::runIteration(int runs)	//runs the interation the correct number of times
{
	for(int i = 0; i < runs; i++)
	{
		runIteration();
		std::cout << "This is the matrix after " << i + 1 << " iterations.\n";
		getMatrix();
		cout << endl;
	}
}

bool** Board::getMatrix()
{
	/*for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}*/

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

	//check if the pattern will fit in the pattern matrix
	if(heightOfSaved <= height || widthOfSaved <= width)
	{
		cerr << "Saved Pattern is larger than board" << endl;
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

	test.runIteration(3);
	std::cout << "This is the matrix after 3 runIterations\n";
	test.getMatrix();
	cout << endl;

    return 0;
}*/
