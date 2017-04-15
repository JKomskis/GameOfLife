//Header file for the board class
#ifndef FORMATS_H_
#define FORMATS_H_

#include "Util.h"
#include <set>

using namespace std;


typedef struct{
	int x;
	int y;
} coords;

BoardData loadLife(string filename);
BoardData loadRLE(string filename);
BoardData loadFormat(string filename);
BoardData loadBRD(string filename);
#endif /* FORMATS_H_ */
