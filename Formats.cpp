#include "Formats.h"

BoardData loadLife(string filename)
{
	ifstream in;
	in.open(filename);

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
	char birthBuf[11];
	char survivalBuf[11];
	set<int> birthRule = {3,};
	set<int> survivalRule = {2, 3,};


	while(getline(in, line))
	{
		// skip empty lines
		if(line.size() == 0)
			continue;

		// handle rule lines
		if(startsWith(line, "#R "))
		{
			if(sscanf(line.c_str(), "#R %10s/%10s", birthBuf, survivalBuf) != 2)
				throw "Error Parsing Rules";
			birthRule = rule2set(birthBuf);
			survivalRule = rule2set(survivalBuf);
		}

		// skip all other comment lines
		if(line.at(0) == '#')
			continue;

		// handle the type specific stuff
		if (type == '5')
		{
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
			height = y;
		}
		else
		{
			// keep track of live coordinates
			sscanf(line.c_str(), "%d %d", &x, &y);
			coords c = {x, y};
			toggleList.push_back(c);

			// keep updating width/height to ensure all coords fit
			if(x > width)
				width = x;
			if(y > height)
				height = y;
		}
	}

	// since coords are 0 based, we need to bump the width & height by 1
	if(type == '6')
	{
		width++;
		height++;
	}
	in.close();
	// apply the data set
	BoardData ret = {true, height, width, 0, 0, 0,
		birthRule, survivalRule,
		vector<vector<bool>>(height, vector<bool> (width, 0))};
	//for(int i = 0; i < (int)toggleList.size(); i++)
	for(auto coord : toggleList)
		ret.matrix[coord.y][coord.x] = true;

	return ret;


}

BoardData loadRLE(string filename)
{
	ifstream in;
	in.open(filename);

	if (!in.is_open())
		throw "Error Opening File";
	string line;
	int width = 0, height = 0;

	// skip comments
	while (getline(in, line))
		if (line.at(0) == '#')
			continue;
		else
			break;

	string rules;
	char birthBuf[11];
	char survivalBuf[11];
	set<int> birthRule;
	set<int> survivalRule;

	// detected rule string
	// %*1s is a cheap way to discard the prefix chars [bBsS]
	if(sscanf(line.c_str(), "x = %d, y = %d, rule = %*1s%10s/%*1s%10s",
		&width, &height, birthBuf, survivalBuf) == 4)
	{
		birthRule = rule2set(birthBuf);
		survivalRule = rule2set(survivalBuf);
	}
	// no rule string, use defaults
	else
	{
		sscanf(line.c_str(), "x = %d, y = %d%*s", &width, &height);
		birthRule = {3,};
		survivalRule = {2, 3,};
	}

	BoardData ret = {true, height, width, 0, 0, 0,
		birthRule, survivalRule,
		vector<vector<bool>>(height, vector<bool> (width, 0))};
	int x = 0, y = 0;
	int count = 0;
	bool quit = false;
	while (getline(in, line) && !quit)
	{
		for (auto c : line)
		{
			// found a comment, get the next line
			if(c == '#')
				break;
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
				if (count > 0)
					y += count;
				else
					y++;
				x = 0;
				count = 0;
				continue;
			}
			if (c == '!')
			{
				quit = true;
				break;
			}

			// apply the alive or dead cell
			if (count)
			{
				for (int j = 0; j < count; j++)
				{
					if (c == 'o')
						ret.matrix[y][x] = true;
					x++;
				}
				count = 0;
			}
			else
			{
				if (c == 'o')
					ret.matrix[y][x] = true;
				x++;
			}
		}

	}
	in.close();

	return ret;
}

BoardData loadBRD(string filename)
{
	ifstream in;
	in.open(filename);

	if (!in.is_open())
		throw "Error Opening File";

	string line;

	// load relevant fields - 1 per line
	int height = fs_atoi(in);
	int width = fs_atoi(in);
	bool wrapAround = fs_atoi(in);
	int iterations = fs_atoi(in);
	int births = fs_atoi(in);
	int deaths = fs_atoi(in);
	set<int> birthRule;
	set<int> survivalRule;

	// load the rules (in the form of a string of numbers in ascii
	getline(in, line);
	birthRule = rule2set(line);
	getline(in, line);
	survivalRule = rule2set(line);

	BoardData ret = {wrapAround, height, width, iterations,
					births, deaths, birthRule, survivalRule,
					vector<vector<bool>>(height, vector<bool> (width, 0))};


	int row = 0;

	// read & apply the matrix
	while(getline(in, line))
	{
		for(int i = 0; i <width; i++)
			ret.matrix[row][i] = line.at(i) == '1';
		row++;
	}

	in.close();
	return ret;
}

BoardData loadFormat(string filename)
{
	if      (endsWith(filename, ".life") ||
			 endsWith(filename, ".lif"))
		return loadLife(filename);

	else if (endsWith(filename, ".rle"))
		return loadRLE(filename);

	else if (endsWith(filename, ".brd"))
		return loadBRD(filename);

	else
		throw "Unknown File Type";
}

/*
int main( int argc, char* args[] )
{
	Pattern *test;
	test = new Pattern("format-test-cases/twogun.rle");

	while(getchar())
	{
		test->printBoard();
		for(int i = 0; i < 99; i++)
			test->Rotate();
	}

	test->printBoard();
	test = new Board("format-test-cases/life_106.life");
	test->printBoard();
	test = new Board("format-test-cases/twogun.rle");
	test->printBoard();

	return 0;
}
*/
