#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <stdlib.h>
#include <vector>
#include <set>

using namespace std;

int fs_atoi(ifstream &input);
bool endsWith(string s, string ending);
string separator();
set<int> rule2set(string input);
string set2rule(set<int> input);

struct BoardData{
    bool wrapAround;
    int height;
    int width;
    int iterations;
    int births;
    int deaths;
	set<int> birthRule;
	set<int> survivalRule;
    vector<vector<bool>> matrix;
};

#endif /* UTIL_H_ */
