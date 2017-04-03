//Header file for the board class
#ifndef FORMATS_H_
#define FORMATS_H_

#include <string>
#include <cstdlib>
#include "Board.h"

using namespace std;

Board * loadLife(string filename);
Board * loadRLE(string filename);
Board * loadFormat(string filename);
#endif /* FORMATS_H_ */

