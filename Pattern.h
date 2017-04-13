#ifndef PATTERN_H_
#define PATTERN_H_

#include <fstream>
//#include <string>
#include "Board.h"

using namespace std;

class Pattern: public Board {
	//bool** patternMatrix;
	//int heightOfPattern;
	//int widthOfPattern;

public:

	Pattern(std::string filename);
	void rotateSquare(int size);
	void Rotate();
	int getHeight();
	int getWidth();
	vector<vector<bool>> getMatrix();
	void trim();

};



#endif /* PATTERN_H_ */
