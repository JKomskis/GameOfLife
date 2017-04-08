/*
 * pattern.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: erikabryant
 */

#include <fstream>
#include <string>
#include <iostream>
#include "pattern.h"

Pattern::Pattern(std::string filename):Board(filename)
{
	/*ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		//std::cerr << "File not opened" << std::endl;
	}
	string line;

	getline(in, line);					//read out the heightOfSaved
	heightOfPattern = atoi(line.c_str());	//make it an int
	getline(in, line);					//read out the widthOfSaved
	widthOfPattern = atoi(line.c_str());	//make it an int

	//initialize new patternMatrix
	matrix = new bool *[heightOfPattern];
	for(int i = 0; i < heightOfPattern; i++)
	{
		matrix[i] = new bool[widthOfPattern];
	}

	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i < widthOfPattern; i++)
		{
			matrix[row][i] = line.at(i) == '1';
		}
		row++;
	}

	//check if the pattern will fit in the pattern matrix
	if(height < heightOfPattern || width < widthOfPattern)
	{
		//cerr << "Saved Pattern is larger than board" << endl;
	}*/
}

//for square patterns
void Pattern::rotateSquare(int size)
{
	vector<vector<bool>> patternCopy(height, vector<bool> (width, 0));
	for(int i =0; i<size; i++)
	{
		for(int j =0; j<size; j++)
		{
            patternCopy[i][j] = matrix[i][j];
        }
    }
	matrix = patternCopy;
}

//for rectangle patterns
void Pattern::rotate(int rot)
{
	int **rotateRectangle = new int*[height];
	for(int i = 0; i < height; ++i)
	{
		rotateRectangle[i] = new int[width];
	}
	for(int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
		rotateRectangle[width][height] = matrix[height][width];
		}
	}
}

int Pattern::getHeight()
{
	return Board::getHeight();
}

int Pattern::getWidth()
{
	return Board::getHeight();
}

std::vector<std::vector<bool>> Pattern::getMatrix()
{
	return Board::getMatrix();
}

/*int main() {
	//Pattern *test = new Pattern("rlepack/gosperglidergun.rle");
	std::vector<std::vector<bool>> matrix = loadFormat("rlepack/gosperglidergun.rle");
	for(int i = 0; i < 9; i++){
		for(int j = 0; j < 36; j++)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout <<std::endl;
	}
	return 0;
}*/
