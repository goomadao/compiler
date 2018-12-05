#include "pch.h"
#include "parser.h"

Parser::Parser(string t):lexer(t),currentToekn(lexer.nextToken())
{
	parsing.push(PROCEDURES);
	syntaxTree = new AstNode(PROCEDURES, 0, currentToekn.getPos());
	currentNode = syntaxTree;
	while (!parsing.empty())
	{
		cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
		if (parsing.top() == GrammarSymSpace::EMPTY)
		{
			currentNode->setInfo("��");
			parsing.pop();
			while (currentNode->getWz() == currentNode->getFatherSize() - 1)//�������ڵ��ں��ӽڵ��������һ�����͵ݹ������ҵ����ĸ��ڵ㲢������ڵ��ں��ӽڵ��в������һ��
			{
				currentNode = currentNode->getFather();
				if (currentNode == syntaxTree)//һֱ�ҵ����ڵ㶼���ں��ӽڵ��е����һ�����򱨴�
				{
					cout << "�޷��ҵ���һ���������Ľڵ�\n";
					if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
						break;
					else
						return;
				}
			}
			if (currentNode != syntaxTree)
				currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//�ҵ����ں��ӽڵ��в������һ���Ľڵ㣬��һ���������Ľڵ���Ǻ��ӽڵ�������ڵ�ĺ����Ǹ�

			cout << "\n";
			continue;
		}
		if (isTerminal(parsing.top()))
		{
			if (parsing.top() == lexerTypeToGrammarType(currentToekn))
			{
				currentNode->setInfo(currentToekn.getVal());//�ս��ƥ�䣬��������ڵ������=token��Val
				while (currentNode->getWz() == currentNode->getFatherSize() - 1)//�������ڵ��ں��ӽڵ��������һ�����͵ݹ������ҵ����ĸ��ڵ㲢������ڵ��ں��ӽڵ��в������һ��
				{
					currentNode = currentNode->getFather();
					if (currentNode == syntaxTree)//һֱ�ҵ����ڵ㶼���ں��ӽڵ��е����һ�����򱨴�
					{
						cout << "�޷��ҵ���һ���������Ľڵ�\n";
						if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
							break;
						else
							return;
					}
				}
				if (currentNode != syntaxTree)
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//�ҵ����ں��ӽڵ��в������һ���Ľڵ㣬��һ���������Ľڵ���Ǻ��ӽڵ�������ڵ�ĺ����Ǹ�
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "��" << currentToekn.getVal() << "ƥ��,��ջ\n";
				parsing.pop();
				if (lexer.tokensEnd() && parsing.empty())
					;
				else
					currentToekn = lexer.nextToken();
			}
			else
			{
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "��" << currentToekn.getVal() << "��ƥ��!\n";
				return;
			}
		}
		else
		{
			if (grammar.parsingTable.find(parsingNode(parsing.top(), lexerTypeToGrammarType(currentToekn)))->second.empty())
			{
				cout << "����(" << GrammarDefinition::GrammarSymTypes[parsing.top()] << "," << currentToekn.getVal() << "),Ԥ�����û�ж�Ӧ����Ŀ\n";
				return;
			}
			else
			{
				int top = parsing.top();
				
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "��ջ;";

				parsing.pop();

				vector<int>::iterator i = grammar.parsingTable.find(parsingNode(top, lexerTypeToGrammarType(currentToekn)))->second.end();
				--i;
				stack<int>expand;
				vector<AstNode*>temp;
				for (; i != grammar.parsingTable.find(parsingNode(top, lexerTypeToGrammarType(currentToekn)))->second.begin(); --i)
				{
					if (*i == EMPTY)
						//continue;
					cout << GrammarDefinition::GrammarSymTypes[*i] << " ";
					parsing.push(*i);
					expand.push(*i);
				}
				//if (*i != EMPTY)
				{
					cout << GrammarDefinition::GrammarSymTypes[*i];
					parsing.push(*i);
					expand.push(*i);
				}
				int count = 0;
				while (!expand.empty())
				{
					temp.push_back(new AstNode(expand.top(), count++, currentToekn.getPos(), currentNode));//count��ʾ��Vector�е�����,���ڵ���currentNode
					expand.pop();
				}
				currentNode->child = temp;
				if (temp.empty())
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//�������ڵ��ں��ӽڵ��������һ�����͵ݹ������ҵ����ĸ��ڵ㲢������ڵ��ں��ӽڵ��в������һ��
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//һֱ�ҵ����ڵ㶼���ں��ӽڵ��е����һ�����򱨴�
						{
							cout << "�޷��ҵ���һ���������Ľڵ�\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								;
							else
								return;
						}
					}
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//�ҵ����ں��ӽڵ��в������һ���Ľڵ㣬��һ���������Ľڵ���Ǻ��ӽڵ�������ڵ�ĺ����Ǹ�
				}
				else
					currentNode = temp[0];//currentNode�Ѿ���ջ�ˣ����Ժ��ӽڵ��еĵ�һ��һ�����ΪcurrentNode
				cout << "��ջ\n";
			}
		}
		cout << "ջ:";
		printParsing();
		cout << "\n";
	}
	printSyntaxTree(syntaxTree);
}

void Parser::printTokens()
{
	lexer.printTokens();
}

int Parser::lexerTypeToGrammarType(Token g)
{
	int t = g.getType();
	switch (t)
	{
	case symbol::ID:
		return GrammarSym::ID;
	case symbol::NUM:
		return GrammarSym::INTEGER;
	case symbol::PLUSSYM:
		return GrammarSym::PLUS;
	case symbol::MINUSSYM:
		return GrammarSym::MINUS;
	case symbol::TIMESSYM:
		return GrammarSym::TIMES;
	case symbol::DIVISIONSYM:
		return GrammarSym::DIVISION;
	case symbol::EQLSYM:
		return GrammarSym::EQUAL;
	case symbol::NEQLSYM:
		return GrammarSym::NOTEQUAL;
	case symbol::LTSYM:
		return GrammarSym::LESSTHAN;
	case symbol::LESYM:
		return GrammarSym::LESSEQUAL;
	case symbol::GTSYM:
		return GrammarSym::GREATERTHAN;
	case symbol::GESYM:
		return GrammarSym::GREATEREQUAL;
	case symbol::BECOMESSYM:
		return GrammarSym::BECOMES;
	case symbol::LPARENSYM:
		return GrammarSym::LEFTPARENT;
	case symbol::RPARENSYM:
		return GrammarSym::RIGHTPARENT;
	case symbol::SEMICOLONSYM:
		return GrammarSym::SEMICOLON;
	case symbol::PERIODSYM:
		return GrammarSym::PERIOD;
	case symbol::COMMASYM:
		return GrammarSym::COMMA;
	case symbol::ODDSYM:
		return GrammarSym::ODD;
	case symbol::BEGINSYM:
		return GrammarSym::BEGIN;
	case symbol::ENDSYM:
		return GrammarSym::END;
	case symbol::IFSYM:
		return GrammarSym::IF;
	case symbol::THENSYM:
		return GrammarSym::THEN;
	case symbol::ELSESYM:
		return GrammarSym::ELSE;
	case symbol::WHILESYM:
		return GrammarSym::WHILE;
	case symbol::WRITESYM:
		return GrammarSym::WRITE;
	case symbol::READSYM:
		return GrammarSym::READ;
	case symbol::DOSYM:
		return GrammarSym::DO;
	case symbol::CALLSYM:
		return GrammarSym::CALL;
	case symbol::CONSTSYM:
		return GrammarSym::CONST;
	case symbol::VARSYM:
		return GrammarSym::VAR;
	case symbol::PROCSYM:
		return GrammarSym::PROCEDURE;
	case symbol::REPEATSYM:
		return GrammarSym::REPEAT;
	case symbol::UNTILSYM:
		return GrammarSym::UNTIL;

	default:
		break;
	}
	return 0;
}

bool Parser::isTerminal(int s)
{
	if (s > GrammarSym::EXPRESSIONPLUSPLUS)
		return true;
	return false;
}

void Parser::printParsing()
{
	stack<int>temp = parsing;
	while (!temp.empty())
	{
		cout << GrammarDefinition::GrammarSymTypes[temp.top() ] << " ";
		temp.pop();
	}
	cout << "\n";
}

void Parser::printSyntaxTree(AstNode * node)
{
	if (node->child.empty())
	{
		/*if (node->getInfo() == "")
			cout << GrammarDefinition::GrammarSymTypes[node->getType()] << "             aaa\n";
		else
			cout << node->getInfo() << "             bbb\n";*/
		cout << node->getInfo() << "\n";
		return;
	}
	for (int i = 0; i < node->child.size(); ++i)
	{
		printSyntaxTree(node->child[i]);
	}
}

