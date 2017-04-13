/*
 * pattern.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: erikabryant
 */

#include <fstream>
//#include <string>
#include <iostream>
#include "Pattern.h"

//Constructor for the Pattern class
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

//Rotates a pattern by 90 degrees counter clockwise
void Pattern::Rotate()
{
	vector<vector<bool>> patternCopy(matrix[0].size(), vector<bool> (matrix.size(), 0));
	for(size_t i=0; i<patternCopy.size(); i++)
	{
		for (size_t j=0; j<patternCopy[0].size(); j++)
		{
		patternCopy[i][j] = matrix[matrix.size()-1-j][i];
		}
	}
	matrix = patternCopy;
}

//returns the height of the board
int Pattern::getHeight()
{
	return Board::getHeight();
}

//returns the width of the board
int Pattern::getWidth()
{
	return Board::getHeight();
}

//returns the board matrix
std::vector<std::vector<bool>> Pattern::getMatrix()
{
	return Board::getMatrix();
}

void Pattern::trim()
{
	unsigned int row = 0;
	unsigned int column = 0;
	for (unsigned int r = 0; r < matrix.size(); r++)
	{
		for (unsigned int c = 0; c < matrix[0].size(); c++)
		{
			if (matrix[r][c])
			{
				row = r;
				column = c;
				break;
			}
		}
	}
	unsigned int endRow = 0;
	unsigned int endColumn = 0;
	for (unsigned int r = matrix.size() -1 ; r >= 0; r--)
	{
		for (unsigned int c = matrix[0].size(); c >= 0; c--)
		{
			if (matrix[r][c])
			{
				endRow = r;
				endColumn = c;
				break;
			}
		}
	}
	vector<vector<bool>> newMatrix;
	for (unsigned int r = row; r <= endRow; r++)
	{
		vector<bool> temp;
		for (unsigned int c = column; c <= endColumn; c++)
		{
			temp.push_back(matrix[r][c]);
		}
		newMatrix.push_back(temp);
	}
	//delete matrix;
	matrix = newMatrix;

}
//For testing purposes
/*
int main() {
	Pattern *test = new Pattern("pattern/blob.rle");
	//std::vector<std::vector<bool>> matrix = loadFormat("rlepack/gosperglidergun.rle");
	for(unsigned int i = 0; i < test->getMatrix().size(); i++){
		for(unsigned int j = 0; j < test->getMatrix()[0].size(); j++)
		{
			std::cout << test->getMatrix()[i][j] << " ";
		}
		std::cout <<std::endl;
	}
	std::cout << std::endl <<std::endl;
	test->trim();
	for(unsigned int i = 0; i < test->getMatrix().size(); i++){
		for(unsigned int j = 0; j < test->getMatrix()[0].size(); j++)
		{
			std::cout << test->getMatrix()[i][j] << " ";
		}
		std::cout <<std::endl;
	}
	return 0;*/
}
