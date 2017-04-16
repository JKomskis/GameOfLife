#include "Util.h"

using namespace std;

/* reads in a line from input, converts it to integer and returns the result
 */
int fs_atoi(ifstream &input)
{
	string l;
	getline(input, l);
	if(input.eof())
		throw "EOF Reached";
	return atoi(l.c_str());
}


/* test that string s ends with ending parameter
 */
bool endsWith(string s, string ending)
{
	return (s.rfind(ending) + ending.length()) == s.length();
}

/* test that string s starts with ending parameter
 */
bool startsWith(string s, string starting)
{
	return (s.find(starting) + starting.length()) == 0;
}

/* helper to return path/directory seperator character
 * depending on the build environment
 */
string separator()
{
#if defined WIN32 || defined _WIN32 || defined __CYGWIN__
    return "\\";
#else
    return "/";
#endif
}

/* converts a string, eg: "123"
 * to an integer set eg {1, 2, 3}
 */
set<int> rule2set(string input)
{
	set<int> ret;
	for(auto character : input)
		if(isdigit(character))
			ret.insert(atoi((const char*)&character));
	return ret;
}

/* converts an integer set eg {1, 2, 3}
 * to an string, eg: "123"
 */
string set2rule(set<int> input)
{
	string ret = "";
	for(auto value : input)
		ret += '0' + char(value);
	return ret;
}
