#ifndef PATTERN_H_
#define PATTERN_H_

#include <fstream>
#include <string>

using namespace std;

class Pattern: public Board {

private:

	bool** patternMatrix;
	int heightOfPattern;
	int widthOfPattern;

public:

	Pattern(std::string filename);
	void rotateSquare(int size);
	void rotate(int rot);
	int getPatternHeight();
	int getPatternWidth();

};



#endif /* PATTERN_H_ */

