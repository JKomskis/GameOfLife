#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "Board.h"
#include "Formats.h"
#include "Util.h"
typedef struct{
	int x;
	int y;
} coords;

Board * loadLife(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	// check the header
	getline(in, line);
	char type = line.back();
	if (line.compare("#Life 1.0") != 1 ||
		(type != '5' && type != '6'))
		throw "Invalid Life Header";
	
	vector<coords> toggleList;
	int width = 0, height = 0;
	int x = 0, y = 0;
	if (type == '5')
	{
		while(getline(in, line))
		{
			if(line.size() == 0 || line.at(0) == '#')
				continue;
			if(width != 0 && width != (int)line.length())
				throw "Error: Non-rectangular matrix";
			width = line.length();
			for(x = 0; x < width; x++)
				if(line.at(x) == '*')
				{
					coords c = {x, y};
					toggleList.push_back(c);
				}
			y++;
		};
		height = y;
	}
	else
	{
		while(getline(in, line))
		{
			if(line.size() == 0 || line.at(0) == '#')
				continue;
			sscanf(line.c_str(), "%d %d", &x, &y);
			if(x > width)
				width = x;
			if(y > height)
				height = y;
			coords c = {x, y};
			toggleList.push_back(c);
		};
		width++;
		height++;
	}
	in.close();

	// create & apply the data set
	Board *ret = new Board(true, width, height);
	
	
	for(int i = 0; i < (int)toggleList.size(); i++)
	{
		ret->toggle(toggleList.at(i).y,
					toggleList.at(i).x);
		//cout << toggleList.at(i).x << "," << toggleList.at(i).y << endl;
	}

	return ret;


}

Board * loadRLE(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	//Board ret;
	int width = 0, height = 0;


	while (getline(in, line))
		if (line.at(0) == '#')
			continue;
		else
			break;
	// handle first line
	sscanf(line.c_str(), "x = %d, y = %d%*s", &width, &height);

	Board *ret = new Board(true, height, width);
	int x=0, y=0;
	while (getline(in, line))
	{
		if (line.at(0) == '#')
			continue;
		//cout << "LINE: " << line << endl;
		int count = 0;
		for (int i = 0; i < (int)line.length(); i++)
		{
			char c = line.at(i);
			// handle newlines
			if (c == '\n' || c == '\r')
				continue;
			// handle RLE number
			if ('0' <= c && c <= '9')
			{
				count = count * 10 + (c - '0');
				continue;
			}

			// handle board newline / quit chars
			if (c == '$')
			{
				y++;
				x = 0;
				count = 0;
				continue;
			}
			if (c == '!')
			{
				break;
			}

			// ok, actually print
			if (count)
			{
				for (int j = 0; j < count; j++)
				{
					if (c == 'o')
						ret->toggle(y, x);
					x++;
				}
				count = 0;
			}
			else
			{
				if (c == 'o')
					ret->toggle(y, x);
				x++;
			}
		}

	}

	in.close();

	return ret;
}

Board * loadFormat(string filename)
{
	if      (endsWith(filename, ".life") ||
			 endsWith(filename, ".lif"))
		return loadLife(filename);

	else if (endsWith(filename, ".rle"))
		return loadRLE(filename);

	else if (endsWith(filename, ".brd"))
		return new Board(filename);

	else
		throw "Unknown File Type";
}
/*
int main( int argc, char* args[] )
{
	Board *test = loadFormat("sample2.life");
	test->printBoard();
	cout << endl;
	
	return 0;
}
*/
