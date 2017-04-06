/*
 * pattern.h
 *
 *  Created on: Apr 5, 2017
 *      Author: erikabryant
 */

#ifndef PATTERN_H_
#define PATTERN_H_

#include <fstream>
#include <string>

using namespace std;

class Pattern: public Board {

private:	//we initialize our private fields

	bool** patternMatrix;
	int heightOfPattern;
	int widthOfPattern;
	int heightRotatedPattern;
	int widthRotatedPattern;

public:	//we initialize our public fields

	Pattern(std::string filename);
	void rotateSquare(int size);
	void rotate();
	int getPatternHeight();
	int getPatternWidth();

};

Pattern::Pattern(std::string filename)	//opens the file called filename
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
	{
		cerr << "File not opened" << endl;
	}
	string line;

	getline(in, line);					//read out the heightOfSaved
	heightOfPattern = atoi(line.c_str());	//make it an int
	getline(in, line);					//read out the widthOfSaved
	widthOfPattern = atoi(line.c_str());	//make it an int
	
	//just to initialize widthRotatedPattern && heightRotatedPattern
	heightRotatedPattern = heightOfPattern;
	widthRotatedPattern = widthOfPattern;

	//initialize new patternMatrix
	patternMatrix = new bool *[heightOfPattern];
	for(int i = 0; i < heightOfPattern; i++)
	{
		patternMatrix[i] = new bool[widthOfPattern];
	}

	//store file values into the patternMatrix
	int row = 0;
	while(getline(in, line))
	{
		for(int i = 0; i < widthOfPattern; i++)
		{
			patternMatrix[row][i] = line.at(i) == '1';
		}
		row++;
	}

	//check if the pattern will fit in the pattern matrix
	if(height < heightOfPattern || width < widthOfPattern)
	{
		cerr << "Saved Pattern is larger than board" << endl;
	}
}

//for square patterns
void Pattern::rotateSquare(int size)
{
	int patternCopy[15][15] = {{0}};
	for(int i =0; i<size; i++){
		for(int j =0; j<size; j++)
		{
            patternCopy [i][j] = patternMatrix[i][j];
        }
    }
	//heightRotatedPattern && widthRotatedPattern stay the same, so no need to change them
}

//for rectangle patterns
void Pattern::rotate()
{
	int rotateRectangle[widthRotatedPattern][heightRotatedPattern] = {{0}};
	for(int i=0; i<height; i++)
	{
		for (int j=0; j<width; j++)
		{
		rotateRectangle[j][i] = patternMatrix[i][j];
		}
	}
	int temp = widthRotatedPattern;
	widthRotatedPattern = heightRotatedPattern;
	heightRotatedPattern = temp;
}

int Pattern::getPatternHeight()	//retrieves the height of the pattern
{
	return this->heightOfPattern;
}

int Pattern::getPatternWidth()
{
	return this->widthOfPattern;
}


#endif /* PATTERN_H_ */

