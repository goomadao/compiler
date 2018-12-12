/*Todo:
	i>>10�ı���û�н��

*/


#include "pch.h"
#include "parser.h"

Parser::Parser(string t):lexer(t),currentToekn(lexer.nextToken())
{
	for (int i = 0; i < lexer.errorTokens.size(); ++i)
	{
		if (lexer.errorTokens[i].getVal() == ":")
			errorAdd(lexer.errorTokens[i].getPos(), "\":\"must be followed by \"=\"");
		else
			errorAdd(lexer.errorTokens[i].getPos(), "Unknown token \"" + lexer.errorTokens[i].getVal() + "\"");
	}
	parsing.push(PROCEDURES);
	syntaxTree = new AstNode(PROCEDURES, 0, currentToekn.getPos());
	currentNode = syntaxTree;
	while (!parsing.empty())
	{
		cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
		if (parsing.top() == GrammarSymSpace::PERIOD&&currentToekn.getType() == symbol::PERIODSYM)
		{
			cout << "�﷨�������\n";
			break;
		}
		if (parsing.top() == GrammarSymSpace::EMPTY)//ջ��Ϊ�գ�ֱ�ӳ�ջ����������Ѱ����һ���÷����Ľڵ㣨�򸸽ڵ�ݹ飩
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
		if (isTerminal(parsing.top()))//ջ�����ս������ʣ�����ȶԣ�ƥ���򶼳�ջ��������������
		{
			if (parsing.top() == lexerTypeToGrammarType(currentToekn))//�ս��ƥ��
			{
				currentNode->setInfo(currentToekn.getVal());//��������ڵ������=token��Val
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
			else//��ƥ�䣬��ʼ������
			{
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "��" << currentToekn.getVal() << "��ƥ��!\n";
				
				//���ҳ�������ڵ����ϵݹ����и��ڵ������follow����һֱ�ҵ����ڵ�
				//���жϵ�ǰָ���token����ȡ��һ��token��ֱ�����token��֮ǰ�ҵ���follow���е�ĳ���ս��ƥ��
				//currentNode��ʼ�򸸽ڵ�ݹ飬ֱ������ڵ��follow���а������token
				errorAdd(currentToekn.getPos(), "Expect \"" + GrammarDefinition::GrammarSymTypes[parsing.top()] + "\", while found \"" + currentToekn.getVal() + "\"");

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				while (tmp != syntaxTree)//һֱ�ݹ鵽���ڵ��ҵ����е�follow�е��ս��
				{
					tmp = tmp->getFather();
					set<int>::iterator it;
					for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
					{
						follows.insert(*it);
					}
				}
				while (1)
				{
					
					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//��ǰָ���token��follows��
					{
						if (currentNode->getType() == lexerTypeToGrammarType(currentToekn))
						{
							terminalMatch = 1;
							cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
							break;
						}
						parsing.pop();
						while (currentNode != syntaxTree)
						{
							for (int i = currentNode->getWz(); i < currentNode->getFather()->child.size() - 1; ++i)
							{
								parsing.pop();
							}//���Ȱѷ���ջ������ڵ����ڵĲ���ʽ�л�û�����Ĳ��ֳ�ջ����ΪҪ������
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//һ���Ƿ��ս��
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//������ս����follow�����е�ǰָ���token���ҵ��˿��Լ������з�����Node
								cout << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "\n\n";
								flag = 1;
								break;
							}
						}
						if (flag)
							break;
					}
					if (lexer.tokensEnd())
					{
						cout << "�����ѿգ��﷨����ʧ��\n";
						return;
					}
					currentToekn = lexer.nextToken();//������һ��token
				}
				if (flag)//����Ҫ��������ս����ջ����һ�����ս�����ܺ�currentTokenƥ��
				{
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

				}
				else if (!terminalMatch)//�Ҳ���ƥ��
				{
					cout << "�޷������ô����﷨����ʧ��\n";
					return;
				}
				else//ֱ���ս��ƥ���ˣ������������Ƕ���ĳ�����ţ�ɾȥ���ɣ�ɾȥ����Ϊ�������������token��
				{
					;
				}




			}
		}
		else//ջ��Ϊ���ս������Ԥ�������Ӧ����Ŀ���ò���ʽ�滻��������������
		{
			//Ԥ�����û�ж�Ӧ����Ŀ�����������
			if (grammar.parsingTable.find(parsingNode(parsing.top(), lexerTypeToGrammarType(currentToekn)))->second.empty())
			{
				cout << "����(" << GrammarDefinition::GrammarSymTypes[parsing.top()] << "," << currentToekn.getVal() << "),Ԥ�����û�ж�Ӧ����Ŀ\n";
				
				errorAdd(currentToekn.getPos(), "Unexpected token \"" + currentToekn.getVal() + "\"");


				//���ҳ�������ڵ����ϵݹ����и��ڵ������follow����һֱ�ҵ����ڵ�(����ڵ㲻Ҫ�ң���Ϊ����ڵ㿪ʼ��ƥ�䣡)
				//���жϵ�ǰָ���token����ȡ��һ��token��ֱ�����token��֮ǰ�ҵ���follow���е�ĳ���ս��ƥ��
				//currentNode��ʼ�򸸽ڵ�ݹ飬ֱ������ڵ��follow���а������token������ڵ�ҲҪ��飩

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				//set<int>::iterator it;//�ȼ��뵱ǰ�ڵ��follow��
				//for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
				//{
				//	follows.insert(*it);
				//}
				while (tmp != syntaxTree)//һֱ�ݹ鵽���ڵ��ҵ����е�follow�е��ս��
				{
					tmp = tmp->getFather();
					set<int>::iterator it;
					for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
					{
						follows.insert(*it);
					}
				}
				while (1)
				{

					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//��ǰָ���token��follows��
					{
						if (currentNode->getType() == lexerTypeToGrammarType(currentToekn))
						{
							terminalMatch = 1;
							cout << "currentNode:" << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "   currentToken:" << currentToekn.getVal() << "\n";
							break;
						}
						parsing.pop();
						if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
						{
							//������ս����follow�����е�ǰָ���token���ҵ��˿��Լ������з�����Node
							flag = 1;
							break;
						}
						while (currentNode != syntaxTree)
						{
							for (int i = currentNode->getWz(); i < currentNode->getFather()->child.size() - 1; ++i)
							{
								parsing.pop();
							}//���Ȱѷ���ջ������ڵ����ڵĲ���ʽ�л�û�����Ĳ��ֳ�ջ����ΪҪ������
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//һ���Ƿ��ս��
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//������ս����follow�����е�ǰָ���token���ҵ��˿��Լ������з�����Node
								flag = 1;
								break;
							}
						}
						if (flag)
							break;
					}
					if (lexer.tokensEnd())
					{
						cout << "�����ѿգ��﷨����ʧ��\n";
						return;
					}
					currentToekn = lexer.nextToken();//������һ��token
				}
				if (flag)//����Ҫ��������ս����ջ����һ�����ս�����ܺ�currentTokenƥ��
				{
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

				}
				else if (!terminalMatch)//�Ҳ���ƥ��
				{
					cout << "�޷������ô����﷨����ʧ��\n";
					return;
				}
				else//ֱ���ս��ƥ���ˣ������������Ƕ���ĳ�����ţ�ɾȥ���ɣ�ɾȥ����Ϊ�������������token��
				{
					;
				}

				



			}
			else//Ԥ������ҵ��˶�Ӧ����Ŀ���ò���ʽ�滻��
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
	printSyntaxTree(syntaxTree);//����﷨����Ҷ�ڵ㣨���ܻ�����ţ�
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

void Parser::errorAdd(Position p, string m)
{
	errorList.push(errorMessage(p, m));
}

void Parser::printErrorList()
{
	if (errorList.empty())
	{
		cout << "No error found.\n";
		return;
	}
	priority_queue<errorMessage>tmp;
	while (!errorList.empty())
	{
		cout << errorList.top();
		tmp.push(errorList.top());
		errorList.pop();
	}
	while (!tmp.empty())
	{
		errorList.push(tmp.top());
		tmp.pop();
	}
}

void Parser::semanticAnalysis()
{
	semanticAnalysis();
}

void Parser::checkSymbolTable()
{
}

