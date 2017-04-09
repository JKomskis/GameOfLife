/*
 * pattern.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: erikabryant
 */

#include <fstream>
#include <string>
#include <iostream>
#include "Pattern.h"

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
	Pattern *test = new Pattern("rlepack/gosperglidergun.rle");
	//std::vector<std::vector<bool>> matrix = loadFormat("rlepack/gosperglidergun.rle");
	for(int i = 0; i < test->getMatrix().size(); i++){
		for(int j = 0; j < test->getMatrix()[0].size(); j++)
		{
			std::cout << test->getMatrix()[i][j] << " ";
		}
		std::cout <<std::endl;
	}
	std::cout << std::endl <<std::endl;
	test->Rotate();
	for(int i = 0; i < test->getMatrix().size(); i++){
		for(int j = 0; j < test->getMatrix()[0].size(); j++)
		{
			std::cout << test->getMatrix()[i][j] << " ";
		}
		std::cout <<std::endl;
	}
	return 0;
}*/
