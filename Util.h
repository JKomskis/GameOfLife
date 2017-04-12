#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <fstream>
//#include <string>
#include <stdlib.h>
//#include <ctime>
#include <vector>

using namespace std;

int fs_atoi(ifstream &input);
bool endsWith(string s, string ending);
string separator();

struct BoardData{
    bool wrapAround;
    int height;
    int width;
    int iterations;
    int births;
    int deaths;
    vector<vector<bool>> matrix;
};

#endif /* UTIL_H_ */
