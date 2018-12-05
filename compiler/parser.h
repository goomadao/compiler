#pragma once
#ifndef _PARSER_H_
#define _PARSER_H_
#include"AstNode.h"
#include"lexer.h"
#include"grammar.h"
#include<string>
#include<stack>
using std::string;
using std::stack;
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

private:
	AstNode* syntaxTree;
	AstNode* currentNode;
	Lexer lexer;
	Grammar grammar;
	stack<int>parsing;
	Token currentToekn;


};


#endif // !_PARSER_H_
