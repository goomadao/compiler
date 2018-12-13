#pragma once
#ifndef _PARSER_H_
#define _PARSER_H_
#include"AstNode.h"
#include"lexer.h"
#include"grammar.h"
#include"ErrorMessage.h"
#include"symbolTable.h"
#include<string>
#include<stack>
#include<queue>
#include<iostream>
using std::string;
using std::stack;
using std::priority_queue;
using std::cout;
using namespace GrammarSymSpace;

class Parser
{
public:
	Parser(string t);

	void printTokens();
	int lexerTypeToGrammarType(Token g);
	bool isTerminal(int s);
	void printParsing();
	void printSyntaxTree(AstNode * node);
	void errorAdd(Position p, string m);
	void printErrorList();

	void semanticAnalysis();
	void checkSymbolTable();
	int getUseType(AstNode* node);

private:
	AstNode* syntaxTree;
	//AstNode* currentNode;
	Lexer lexer;
	Grammar grammar;
	stack<int>parsing;//LL1语法分析栈
	Token currentToekn;//当前分析到的token，即输入中剩余代码中的第一个token
	priority_queue<errorMessage, vector<errorMessage>, greater<errorMessage> >errorList;
	symbolTable* symTable;


};


#endif // !_PARSER_H_
