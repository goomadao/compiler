#pragma once
#ifndef _PARSER_H_
#define _PARSER_H_
#include"tree.h"
#include"lexer.h"
#include"grammar.h"
#include<string>
using std::string;

class Parser
{
public:
	Parser(string t);

	void printTokens();

private:
	Tree syntaxTree;
	Lexer lexer;
	Grammar grammar;
};


#endif // !_PARSER_H_
