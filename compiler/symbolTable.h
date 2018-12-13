#pragma once
#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_
#include<set>
#include<string>
#include<iostream>
using std::set;
using std::string;
using std::cout;
using std::ostream;

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
	symbolTable* getPre();
	bool findVar(string s);
	bool findConst(string s);
	bool findProcedure(string s);
	bool findWithNoRecursive(string s);
	void addConst(string s);
	void addVar(string s);
	void addProcedure(string s);
	void printTable();
};


#endif // !_SYMBOLTABLE_H_
