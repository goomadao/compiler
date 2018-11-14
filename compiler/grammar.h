
#pragma once
#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_
#include<vector>
#include<list>
#include<set>
#include<map>
#include"grammarDefinition.h"
using std::vector;
using std::list;
using std::set;
using std::map;
using namespace GrammarSym;

struct parsingNode
{
	int nonTerminal, terminal;
	parsingNode(int i, int j)
	{
		nonTerminal = i;
		terminal = j;
	}
	bool operator < (const parsingNode & other) const
	{
		if (nonTerminal == other.nonTerminal)
			return terminal < other.terminal;
		else
			return nonTerminal < other.nonTerminal;
	}
};

class Grammar
{
public:
	Grammar();
	void getOriginGrammar();
	void getOriginList();
	void printGrammarList();
	//void transportToList();
	void eliminateLeftRecursive();
	void leftFactoring();


	void getFirst();
	void calculateFirst(int x);
	void printFirst();

	void getFollow();
	void calculateFollow();
	int getFollowSize();
	void printFollow();

	void getParsingTable();
	void printParsingTable();


private:
	vector<GrammarDefinition> grammarSet;


	/*
	第一个vector：文法产生式左端
	第二个vector：第一个右部产生式
	第三个vector：第二个右部产生式
	……
	*/
	list< list< vector<int> > > grammarList;
	map<int, set<int> >first;
	map<int, set<int> >follow;
	map<parsingNode, vector<int> >parsingTable;
};


#endif // !GRAMMAR_H
