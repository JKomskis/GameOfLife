#include "Formats.h"

#include <string>
#include <iostream>
#include "Board.cpp"
#include <fstream>
#include <stdlib.h>

Board loadLife105(string filename)
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
		 if(width != 0 && width != line.length())
			throw "Error: Non-rectangular matrix";
		 width = line.length();
		 
	}while(getline(in, line));
	
	// create & apply the data set
	Board ret(false, height, width);
	for(int i = 0; i < width; i++)
		for(int j = 0; j < height; j++)
			if(boardLines[i].at(j) == '*')
				ret.toggle(j, i);

	return ret;
	
	
}

Board loadRLE(string filename)
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
	sscanf(line.c_str(), "x = %d, y = %d", &width, &height);
	
	Board ret(false, height, width);
	int x=0, y=0;
	while (getline(in, line))
	{
		if (line.at(0) == '#')
			continue;
		//cout << "LINE: " << line << endl;
		int count = 0;
		for (int i = 0; i < line.length(); i++)
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
						ret.toggle(x, y);//cout << c; //cout << "IN WHILE: " << c << endl;
					x++;
				}
				count = 0;
			}
			else
			{
				if (c == 'o')
					ret.toggle(x, y);
				x++;
			}
		}
		
	}
	
	return ret;
	
	
}

int main( int argc, char* args[] )
{
	string filename = "formats/life105";
	Board test = loadLife105(filename);
	test.getMatrix();
	cout << endl;
	
	Board rle = loadRLE("formats/rle");
	rle.getMatrix();
	cout << endl;
	
	Board rle2 = loadRLE("formats/rle2");
	rle2.getMatrix();
	cout << endl;
	return 0;
}
