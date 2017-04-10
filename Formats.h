//Header file for the board class
#ifndef FORMATS_H_
#define FORMATS_H_

#include <string>
#include <cstdlib>
//#include "Board.h"
#include "Util.h"

using namespace std;

BoardData loadLife(string filename);
BoardData loadRLE(string filename);
BoardData loadFormat(string filename);
BoardData loadBRD(string filename);
#endif /* FORMATS_H_ */
