//Header file for the board class
#ifndef FORMATS_H_
#define FORMATS_H_

#include <string>
#include <cstdlib>
#include "Board.h"
#include <vector>

using namespace std;

vector<vector<bool>> loadLife(string filename);
vector<vector<bool>> loadRLE(string filename);
vector<vector<bool>> loadFormat(string filename);
vector<vector<bool>> loadBRD(string filename);
#endif /* FORMATS_H_ */
