// ConsoleApplication4.cpp : Defines the entry point for the console application.
//
//This is a test

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Board.h"

using namespace std;

Board::Board(bool wrap, int h, int w): matrix(h, vector<bool> (w, 0))
{
	this->height = h;
	this->width = w;

	this->wrapAround = wrap;

	this->iterations = 0;
	this->births = 0;
	this->deaths = 0;
	/*matrix = new bool*[height];

	for (int i = 0; i < height; i++)
	{
		matrix[i] = new bool[width];
	}*/

	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			matrix[i][j] = 0;
		}
	}*/
}

Board::Board(string filename)
{
	matrix = loadFormat(filename);
	height = sizeof(matrix) / sizeof(matrix[0]);
	width = sizeof(matrix[0]) / sizeof(bool);
	wrapAround = true;

}

void Board::toggle(int r, int c)	//toggles the cell from true to false or false to true
{
	matrix[r][c] = !matrix[r][c];
}

void Board::randomize(double ratio)
{
	ratio = (ratio < 0) ? 0 : ratio;
	ratio = (ratio > 1) ? 1 : ratio;
	for (int r = 0; r < height; r++)
	{
		for (int c = 0; c < width; c++)
		{
			if (((double)std::rand()/RAND_MAX) <= ratio) {
				toggle(r, c);
			}
		}
	}
}

int Board::numNeigh(int r,int c)
{
	/*creating a count variable, setting it equal to -1 if cell is alive (thus
	avoiding counting it twice)*/
	int count = ((matrix[r][c]) ? -1 : 0);
	if (wrapAround)
	{
		for (int i = r-1; i <= r+1; i++)
		{
			for (int j = c-1; j <= c+1; j++)
			{
				//std::cout << "r+i " << r+i << "c+j " << c+j <<std::endl;
				if ( matrix[((i < 0) ? height+i : i) % height][((j < 0) ? width + j : j) % width])
					count++;
			}
		}
	}
	/*neighbor count nested for loops for wrapAround = false
	the ternary operators are used to detect when the cell is on the edge of
	the board*/
	else {
		for (int i = ((r == 0) ? 0: r - 1); i <= ((r == height - 1 ) ? r:r+1); i++)
		{
			for (int j = ((c == 0) ? 0: c - 1); j <= ((c == width - 1) ? c:c+1); j++)
			{
				//std::cout << "r+i " << r+i << "c+j " << c+j <<std::endl;
				if (matrix[i][j])
					count++;
			}
		}
	}
	//std::cout << "count:" << count <<endl;
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
					deaths++;
					//std::cout << "For " << r << ", " << c << " we changed it to a 0 because we had <2: " << nMatrix[r][c] <<endl;
				}
				else if(nMatrix[r][c] > 3)	//Any live cell with more than three live neighbours dies, as if by overpopulation.
				{
					toggle(r, c);
					deaths++;
					//std::cout << "For " << r << ", " << c << " we changed it to a 0 because we had >3: " << nMatrix[r][c] << endl;
				}
				//if neither of these is true, the cell stays alive
			}
			else	//if the cell is dead (equal to 0)
			{
				if(nMatrix[r][c] == 3)	//Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
				{
					toggle(r, c);
					births++;
					//std::cout << "For " << r << ", " << c << " we changed it to a 1 because we had ==3: " << nMatrix[r][c] << endl;
				}
			}
		}
	}
	for (int i = 0; i < height; i++)
	{
		delete nMatrix[i];
	}
	delete nMatrix;
	iterations++;

}

void Board::runIteration(int runs)	//runs the interation the correct number of times
{
	for(int i = 0; i < runs; i++)
	{
		runIteration();
		//std::cout << "This is the matrix after " << i + 1 << " iterations.\n";
		//getMatrix();
		//cout << endl;
	}
}

vector<vector<bool>>& Board::getMatrix()
{
	return this->matrix;
}

void Board::printBoard()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void Board::saveState(string fileName)
{
	ofstream out(fileName.c_str());
	out << height << endl;	//first line tells the program the height of the saved matrix
	out << width << endl;	//second line tells the program the width of the saved matrix
	out << wrapAround << endl;
	out << iterations << endl;
	out << births << endl;
	out << deaths << endl;

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

	heightOfSaved = fs_atoi(in);
	widthOfSaved = fs_atoi(in);
	cout << heightOfSaved << endl;
	//initialize new patternMatrix
	bool **patternMatrix = new bool *[heightOfSaved];
	for(int i = 0; i < heightOfSaved; i++)
	{
		patternMatrix[i] = new bool[widthOfSaved];
	}

	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i <widthOfSaved; i++)
		{
			patternMatrix[row][i] = line.at(i) == '1';
		}
		row++;
	}
	// 4/3/17 - JJK - Bad logic here - now fixed
	//check if the pattern will fit in the pattern matrix
	if((heightOfSaved + y) > height || (widthOfSaved + x) > width)
	{
		cerr << "Saved Pattern is larger than board" << endl;
	}

	//now place patternMatrix in matrix
	/*int countX = 0;
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
	}*/

	// pretty sure there's a bug here - 4/6/17 JJK
	// x is supposed to be horizontal
	// used to access the y component of the matrix
	for(int i = 0; i < heightOfSaved; i++)
		for(int j = 0; j < widthOfSaved; j++)
			matrix[x+i][y+j] = patternMatrix[i][j];
	in.close();

}

void Board::addPattern(vector<vector<bool>> patternMatrix, int y, int x)
{
	for(size_t i = 0; i < patternMatrix.size(); i++)
	{
		for(size_t j = 0; j < patternMatrix[0].size(); j++)
		{
			matrix[y+i][x+j] = patternMatrix[i][j];
		}
	}
}

int Board::getHeight()
{
	return height;
}

int Board::getWidth()
{
	return width;
}

int Board::getIterations()
{
	return iterations;
}

int Board::getBirths()
{
	return this->births;
}

int Board::getDeaths()
{
	return deaths;
}

/*
int main()
{

	int height;
	int width;

	cout << "Height of board: ";
	cin >> height;

	cout << "Width of board: ";
	cin >> width;

	cout << "Add a board called test\n";
	Board test(false, height, width);

	cout << "Print out the matrix:\n";
	test.printBoard();
	cout << endl;

	cout << "Add a pattern called patterntest.txt and then print out the matrix\n";
	test.addPattern("patterntest.txt", 1, 1);
	test.printBoard();
	cout << endl;

	//test.getMatrix();
	//cout << endl;

	cout << "Try running the runIteration function: \n";
	test.runIteration();

	cout << "This is the matrix now\n";
	test.printBoard();
	cout << endl;

	test.runIteration(3);
	cout << "This is the matrix after 3 runIterations\n";
	test.printBoard();
	cout << endl;

	cout << "Random Board\n";
	test.randomize();
	test.printBoard();
	cout << endl;

	while (true)
	{
		test.runIteration(100);
	}

    return 0;
}
*/
