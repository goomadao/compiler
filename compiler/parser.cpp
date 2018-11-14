#include "pch.h"
#include "parser.h"

Parser::Parser(string t):lexer(t)
{
}

void Parser::printTokens()
{
	lexer.printTokens();
}
