#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "Board.h"
#include "Formats.h"
#include "Util.h"

Board * loadLife(string filename)
{
	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	// check the header
	getline(in, line);
	if (line.compare("#Life 1.05"))
		throw "Invalid Life 1.05 Header";

	// skip filler lines
	while(line.size() == 0 || line.at(0) == '#')
		getline(in, line);
	// cache the lines to avoid having to re-read
	string boardLines[20];
	int width = 0, height = 0;
	do
	{
		 boardLines[height++] = line;
		 if(width != 0 && (unsigned int)width != line.length())
			throw "Error: Non-rectangular matrix";
		 width = line.length();
		 
	}while(getline(in, line));
	
	in.close();
	
	// create & apply the data set
	Board *ret = new Board(true, height, width);
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			if(boardLines[i].at(j) == '*')
				ret->toggle(i, j);

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
			
			// handle RLE number
			if ('0' <= c && c <= '9')
			{
				count = count * 10 + (c - '0');
				continue;
			}
			
			// handle newline / quit chars
			if (c == '$')
			{
				y++;
				x = 0;
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
	Board *test = load("formats/sample.life");
	test->printBoard();
	cout << endl;
	
	Board *rle = load("formats/sample1.rle");
	rle->printBoard();
	cout << endl;
	
	Board *rle2 = load("formats/sample2.rle");
	rle2->printBoard();
	cout << endl;
	
	return 0;
}
*/
