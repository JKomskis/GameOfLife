#ifndef PATTERN_H_
#define PATTERN_H_

#include <fstream>
#include "Board.h"

using namespace std;

class Pattern: public Board {

public:

	Pattern(std::string filename);
	void Rotate();

};



#endif /* PATTERN_H_ */
