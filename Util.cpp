#include "Util.h"

using namespace std;

int fs_atoi(ifstream &input)
{
	string l;
	getline(input, l);
	if(input.eof())
		throw "EOF Reached";
	return atoi(l.c_str());
}

bool endsWith(string s, string ending)
{
	return (s.rfind(ending) + ending.length()) == s.length();
}

bool startsWith(string s, string starting)
{
	return (s.find(starting) + starting.length()) == 0;
}

string separator()
{
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
    return "\\";
#else
    return "/";
#endif
}

set<int> rule2set(string input)
{
	set<int> ret;
	for(auto character : input)
		ret.insert(atoi((const char*)&character));
	return ret;
}

string set2rule(set<int> input)
{
	string ret = "";
	for(auto value : input)
		ret += '0' + char(value);
	return ret;
}
