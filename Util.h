#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <fstream>
//#include <string>
#include <stdlib.h>
//#include <ctime>
#include <vector>

int fs_atoi(std::ifstream &input);
bool endsWith(std::string s, std::string ending);

struct BoardData{
    bool wrapAround;
    int height;
    int width;
    int iterations;
    int births;
    int deaths;
    std::vector<std::vector<bool>> matrix;
};

#endif /* UTIL_H_ */
