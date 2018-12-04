#pragma once
#ifndef _ASTNODE_H_
#define _ASTNODE_H_
#include<string>
using std::string;



class AstNode
{
public:
	AstNode();
private:
	int type;
	string info;

};

#endif // !_ASTNODE_H_
