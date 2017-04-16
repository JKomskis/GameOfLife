#include "Pattern.h"

//Constructor for the Pattern class
Pattern::Pattern(std::string filename):Board(filename)
{
	;
}

//Rotates a pattern by 90 degrees counter clockwise, can be used to rotate a pattern more than once or in opposite direction with modification.
void Pattern::Rotate()
{
	//copies original pattern before making changes to location of new rotated pattern board
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
