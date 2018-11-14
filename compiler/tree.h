#pragma once
#ifndef _TREE_H
#define _TREE_H_
#include<string>
#include<vector>
#include"grammarDefinition.h"
using namespace GrammarSym;
using std::string;
using std::vector;

class Tree
{
public:
	Tree();

private:
	int type;
	string info;
	vector<Tree*> child;
};





#endif // !_TREE_H
