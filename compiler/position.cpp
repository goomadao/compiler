#include"position.h"
#include<iostream>
using std::cout;

Position::Position()
{
	row = col = 0;
}

Position::Position(int r, int c)
{
	row = r;
	col = c;
}

void Position::printPos()
{
	cout << "(" << row << "," << col << ")";
}