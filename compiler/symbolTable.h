#pragma once
#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_
#include<set>
#include<string>
using std::set;
using std::string;

class symbolTable
{
private:
	symbolTable* pre;
	set<string>v;
	set<string>c;
	set<string>p;
public:
	symbolTable();
	symbolTable(symbolTable * pres);
};


#endif // !_SYMBOLTABLE_H_
