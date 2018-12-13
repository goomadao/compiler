#include "pch.h"
#include "symbolTable.h"

symbolTable::symbolTable()
{
	pre = nullptr;
	//c = set<string>{};
	//v = set<string>{};
	//p = set<string>{};
}

symbolTable::symbolTable(symbolTable * pres)
{
	pre = pres;
	//c = set<string>{};
	//v = set<string>{};
	//p = set<string>{};
}

symbolTable * symbolTable::getPre()
{
	return pre;
}

bool symbolTable::findVar(string s)
{
	symbolTable* tmp = this;
	while (tmp != nullptr)
	{
		if (tmp->v.find(s) != tmp->v.end())
			return true;
		tmp = tmp->pre;
	}
	return false;
}

bool symbolTable::findConst(string s)
{
	symbolTable* tmp = this;
	while (tmp != nullptr)
	{
		if (tmp->c.find(s) != tmp->c.end())
			return true;
		tmp = tmp->pre;
	}
	return false;
}

bool symbolTable::findProcedure(string s)
{
	symbolTable* tmp = this;
	while (tmp != nullptr)
	{
		if (tmp->p.find(s) != tmp->p.end())
			return true;
		tmp = tmp->pre;
	}
	return false;
}

bool symbolTable::findWithNoRecursive(string s)
{
	if (v.find(s) == v.end() && c.find(s) == c.end() && p.find(s) == p.end())
		return false;
	return true;
}

void symbolTable::addConst(string s)
{
	c.insert(s);
}

void symbolTable::addVar(string s)
{
	v.insert(s);
}

void symbolTable::addProcedure(string s)
{
	p.insert(s);
}

void symbolTable::printTable()
{
	int js = 1;
	symbolTable* tmp = this;
	while (tmp->pre != nullptr)
	{
		++js;
		tmp = tmp->pre;
	}
	cout << "Level:" << js;
	cout << "\nconst: ";
	for (set<string>::iterator it = c.begin(); it != c.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << "\nvar: ";
	for (set<string>::iterator it = v.begin(); it != v.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << "\nprocedure: ";
	for (set<string>::iterator it = p.begin(); it != p.end(); ++it)
	{
		cout << *it << " ";
	}
	cout << "\n";
}
