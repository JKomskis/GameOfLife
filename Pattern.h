#ifndef PATTERN_H_
#define PATTERN_H_

#include "Board.h"
#include <vector>
#include <string>

using namespace std;

class Pattern: public Board {

public:

	Pattern(std::string filename);
	void Rotate();

};



#endif /* PATTERN_H_ */
