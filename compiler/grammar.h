
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
using namespace GrammarSymSpace;

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

	map<parsingNode, vector<int> >parsingTable;
	map<int, set<int> >follow;
private:
	vector<GrammarDefinition> grammarSet;


	/*
	��һ��vector���ķ�����ʽ���
	�ڶ���vector����һ���Ҳ�����ʽ
	������vector���ڶ����Ҳ�����ʽ
	����
	*/
	list< list< vector<int> > > grammarList;
	map<int, set<int> >first;
	
	
};


#endif // !GRAMMAR_H
