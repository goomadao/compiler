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
	symTable = new symbolTable();
	runTable = new sTable();
	AstNode* currentNode = syntaxTree;
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
				




				errorAdd(currentToekn.getPos(), "Expect \"" + GrammarDefinition::GrammarSymTypes[parsing.top()] + "\", while found \"" + currentToekn.getVal() + "\"");

				//���Ȳ鿴�Լ�������ֵܽڵ��Ƿ���ڷֺź�end��
				//������ڣ���ֱ��ֹͣѰ��follow��
				//��������ڣ��Ϳ�ʼ�򸸽ڵ�ݹ飬�ȼ�¼����ڵ��follow����Ȼ��Ҳ�鿴����ڵ������ֵܽڵ��Ƿ���ڷֺš�end���߱�־���������С���㣬һֱ���ϵݹ飬ֱ���ֵܽڵ����ҵ��˷ֺš�end���߱�־���������С�����ֹͣ�ݹ顣
				//Ȼ��ʼɨ��token��
				//������token��֮ǰ�ļ�¼���棬��ô˵��Ӧ�����´����token��ʼ������
				//������ڣ������ɨ����һ��token��ֱ��ɨ�赽end���ֺŻ��߱�־���������С���㣬ǿ���ж�ɨ�裬���Ҵ��������¿�ʼ����
				//Ȼ��������tokenȷ��Ӧ�ðѷ���ջ����Щ���ֳ�ջ����Щ���ֱ���������Ȼ�����¿�ʼ�﷨����

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				while (tmp != syntaxTree)//һֱ���ϵݹ���follow����ֱ������; end����.
				{
					cout << "currentNode:  " << GrammarDefinition::GrammarSymTypes[tmp->getType()] << "\n";
					bool flag = 0;
					for (int i = tmp->getWz() + 1; i < tmp->getFather()->child.size(); ++i)
					{
						if (tmp->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::END ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
						{
							flag = 1;
							break;
						}
					}
					if (flag)
						break;





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

					//���������; end����.����Ӧ�ô��������¿�ʼ�﷨�����ˣ������Ķ�Ҫ����
					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::SEMICOLON ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::END ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PERIOD)
					{
						while (parsing.top() != GrammarSymSpace::SEMICOLON &&
							parsing.top() != GrammarSymSpace::END &&
							parsing.top() != GrammarSymSpace::PERIOD)//��ջ����; end����.֮��ķ���ȫ����ջ����������
						{
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (currentNode->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::END ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
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
				while (1)//һֱ�ݹ鵽���ڵ��ҵ����е�follow�е��ս��
				{
					cout << "currentNode:  " << GrammarDefinition::GrammarSymTypes[tmp->getType()] << "\n";
					bool flag = 0;
					for (int i = tmp->getWz() + 1; i < tmp->getFather()->child.size(); ++i)
					{
						if (tmp->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::END ||
							tmp->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
						{
							flag = 1;
							break;
						}
					}
					if (flag)
						break;




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
						//if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
						//{
						//	//������ս����follow�����е�ǰָ���token���ҵ��˿��Լ������з�����Node
						//	flag = 1;
						//	break;
						//}
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



					if (lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::SEMICOLON ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::END ||
						lexerTypeToGrammarType(currentToekn) == GrammarSymSpace::PERIOD)
					{
						while (parsing.top() != GrammarSymSpace::SEMICOLON &&
							parsing.top() != GrammarSymSpace::END &&
							parsing.top() != GrammarSymSpace::PERIOD)
						{
							parsing.pop();
						}
						bool flag = 0;
						while (1)
						{
							for (int i = currentNode->getWz() + 1; i < currentNode->getFather()->child.size(); ++i)
							{
								if (currentNode->getFather()->child[i]->getType() == GrammarSymSpace::SEMICOLON ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::END ||
									currentNode->getFather()->child[i]->getType() == GrammarSymSpace::PERIOD)
								{
									flag = 1;
									currentNode = currentNode->getFather()->child[i];
									break;
								}
							}
							if (flag)
								break;
							currentNode = currentNode->getFather();
						}
						terminalMatch = 1;
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
	AstNode* baocun = syntaxTree;
	printSyntaxTree(syntaxTree);//����﷨����Ҷ�ڵ㣨���ܻ�����ţ�
	syntaxTree = baocun;



	semanticAnalysis();//�������



	if (errorList.empty())
	{
		genPCode();
		printPCode();
	}


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
	checkSymbolTable();
}

void Parser::checkSymbolTable()
{
	AstNode* currentNode = syntaxTree;
	symbolTable* currentTable = symTable;
	while (1)
	{
		/*int js = 0;
		symbolTable* tst = currentTable;
		while (tst != nullptr)
		{
			++js;
			tst = tst->getPre();
		}
		cout << "currentNode: " << GrammarDefinition::GrammarSymTypes[currentNode->getType()] << "  Level:" << js << "\n";*/




		if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
		{
			currentTable = new symbolTable(currentTable);
		}
		if (currentNode->getType() == GrammarSymSpace::ID)//�����ǰ�ڵ���ID�����ϵݹ��������������ʹ��
		{
			int flag = getUseType(currentNode);
			if (flag == -4)
			{
				if (!currentTable->findVar(currentNode->getInfo()) && !currentTable->findConst(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \""+currentNode->getInfo()+"\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -3)
			{
				if (!currentTable->findProcedure(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined procedure \"" + currentNode->getInfo() + "\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -2)
			{
				if (!currentTable->findVar(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \"" + currentNode->getInfo() + "\"");
				}
				/*else
				{
					currentTable->printTable();
				}*/
			}
			else if (flag == -1)
			{
				if (!currentTable->findConst(currentNode->getInfo()))
				{
					errorAdd(currentNode->getPos(), "Undefined identifier \"" + currentNode->getInfo() + "\"");
				}
			}
			else if (flag == 1)
			{
				if (!currentTable->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->addConst(currentNode->getInfo());
					if (currentNode->getFather()->child.size() == 3)
					{
						bool flag = 1;
						AstNode* tmpNode = currentNode->getFather()->child[2];
						for (int i = 0; i < tmpNode->getInfo().size(); ++i)
						{
							if (!isdigit(tmpNode->getInfo()[i]))
							{
								flag = 0;
								break;
							}
						}
						if (flag)
						{
							currentTable->addCConst(currentNode->getInfo(), stoi(currentNode->getFather()->child[2]->getInfo()));
							cout << "constSymbol:" << currentNode->getInfo() << "   Val:" << stoi(currentNode->getFather()->child[2]->getInfo()) << "\n";
						}
						else
						{
							currentTable->addCConst(currentNode->getInfo());
						}
					}
					//cout << "const�����ɹ�\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\""+currentNode->getInfo()+"\" has been defined, redefinition is not allowed.");
				}
			}
			else if (flag == 2)
			{
				if (!currentTable->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->addVar(currentNode->getInfo());
					//cout << "var�����ɹ�\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\"" + currentNode->getInfo() + "\" has been defined, redefinition is not allowed.");
				}
			}
			else if (flag == 3)
			{
				if (!currentTable->getPre()->findWithNoRecursive(currentNode->getInfo()))
				{
					currentTable->getPre()->addProcedure(currentNode->getInfo());
					//cout << "procedure�����ɹ�\n";
				}
				else
				{
					errorAdd(currentNode->getPos(), "\"" + currentNode->getInfo() + "\" has been defined, redefinition is not allowed.");
				}
			}
			else
			{
				errorAdd(currentNode->getPos(), "Unknown error.");
			}
		}




		//������һ���ڵ�
		if (!currentNode->child.empty())
		{
			currentNode = currentNode->child[0];
		}
		else
		{
			while (currentNode->getWz() == currentNode->getFather()->child.size() - 1)
			{
				currentNode = currentNode->getFather();
				if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
					currentTable = currentTable->getPre();
				if (currentNode == syntaxTree)
				{
					return;
				}
			}
			currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];
		}
	}

}


/*
-4������var����const����ȥ����
-3������procedure����
-2������var����
-1������const����
 0��������
 1��������const
 2��������var
 3��������procedure
*/
int Parser::getUseType(AstNode * node)
{
	if (node->getType() == GrammarSymSpace::CONSTDEFINITION)//����const
	{
		return 1;
	}
	if (node->getType() == GrammarSymSpace::VARDEFINITION)//����var
	{
		return 2;
	}
	if (node->getType() == GrammarSymSpace::PROCEDUREHEADER)//����procedure
	{
		return 3;
	}
	if (node->getType() == GrammarSymSpace::ASSIGHNSTATEMENT ||
		node->getType() == GrammarSymSpace::READVAR)//��ֵ���Ͷ�������ʹ��var
	{
		return -2;
	}
	if (node->getType() == GrammarSymSpace::CALLSTATEMENT)//������䣬ʹ��procedure
	{
		return -3;
	}
	if (node->getType() == GrammarSymSpace::STATEMENTTABLE)
	{
		if (node->child[0]->getType() == GrammarSymSpace::ID)//��ֵ��䣬ʹ��var
			return -2;
		if (node->child[1]->getType() == GrammarSymSpace::ID)//������䣬ʹ��procedure
			return -3;
		return 0;//����
	}
	if (node->getType() == GrammarSymSpace::FACTOR)
	{
		return -4;//������constҲ������var
	}
	if (node == syntaxTree)//�ҵ����ڵ�
		return 0;
	return getUseType(node->getFather());//�����ݹ��򸸽ڵ����
}

void Parser::genPCode()
{
	AstNode* currentNode = syntaxTree;
	sTable* currentTable = runTable;
	pcode.push_back(pCode("JMP", 0, 0x3f3f3f));
	while (1)
	{
		if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
		{
			currentTable = new sTable(currentTable);
		}




		if (currentNode->getType() == GrammarSymSpace::ID)//�����ǰ�ڵ���ID�����ϵݹ��������������ʹ��
		{
			int flag = getUseType(currentNode);
			if (flag == 1)
			{
				currentTable->addConst(currentNode->getInfo(), stoi(currentNode->getFather()->child[2]->getInfo()));
				//cout << "const�����ɹ�\n";
			}
			else if (flag == 2)
			{
				currentTable->addVar(currentNode->getInfo());
				//cout << "var�����ɹ�\n";
			}
			else if (flag == 3)
			{
				currentTable->getPre()->addProcedure(currentNode->getInfo());
				//cout << "procedure�����ɹ�\n";
			}
			else
			{
				errorAdd(currentNode->getPos(), "Unknown error.");
			}
		}

		else if (currentNode->getType() == GrammarSymSpace::STATEMENT)
		{
			handleStatement(currentNode, currentTable);
			while (!currentNode->child.empty())
			{
				currentNode = currentNode->child[currentNode->child.size() - 1];
			}
		}

		else if (currentNode->getType() == GrammarSymSpace::STATEMENTTABLE)
		{
			handleStatementTable(currentNode, currentTable);
			while (!currentNode->child.empty())
			{
				currentNode = currentNode->child[currentNode->child.size() - 1];
			}
		}




		//������һ���ڵ�
		if (!currentNode->child.empty())
		{
			currentNode = currentNode->child[0];
		}
		else
		{
			while (currentNode->getWz() == currentNode->getFather()->child.size() - 1)
			{
				currentNode = currentNode->getFather();
				if (currentNode->getType() == GrammarSymSpace::PROCEDURELIST)
				{
					currentTable = currentTable->getPre();
					if (currentNode->getFather()->getType() == GrammarSymSpace::PROCEDUREBODY)
					{
						currentTable->getPre()->setProcedure(currentNode->getFather()->getFather()->child[0]->child[1]->getInfo(), pcode.size());//table��procedure id���ܶ�λ����
						pcode.push_back(pCode("INT", 0, 3 + currentTable->getVarSize()));
					}
					else if (currentNode->getFather()->getType() == GrammarSymSpace::DEFINITIONPART)
					{
						startJump = pcode.size();
						pcode[0].a2 = pcode.size();
						pcode.push_back(pCode("INT", 0, 3 + currentTable->getVarSize()));
					}
				}
				if (currentNode->getType() == GrammarSymSpace::PROCEDUREBODY ||
					currentNode->getType() == GrammarSymSpace::SUBPROCEDURE)
				{
					pcode.push_back(pCode("OPR", 0, 0));
				}
				if (currentNode == syntaxTree)
				{
					return;
				}
			}
			currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];
		}
	}
}

void Parser::handleExpression(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::TERM)
	{
		handleTerm(currentNode->child[0], currentTable);
		handleExpressionPlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::PLUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		handleExpressionPlus(currentNode->child[2], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::MINUS)
	{
		//ToDo:���ʽ��ͷΪ-ʱ������
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 1));
		handleExpressionPlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleExpressionPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::PLUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 2));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::MINUS)
	{
		handleTerm(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 3));
	}
	else
	{
		return;
	}
	handleExpressionPlus(currentNode->child[2], currentTable);
}

void Parser::handleTerm(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleFactor(currentNode->child[0], currentTable);
	handleTermPlus(currentNode->child[1], currentTable);
}

void Parser::handleTermPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::TIMES)
	{
		handleFactor(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 4));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::DIVISION)
	{
		handleFactor(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 5));
	}
	else
	{
		return;
	}
	handleTermPlus(currentNode->child[2], currentTable);
}

void Parser::handleFactor(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::LEFTPARENT)
	{
		handleExpression(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::INTEGER)
	{
		pcode.push_back(pCode("LIT", 0, stoi(currentNode->child[0]->getInfo())));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::ID)
	{
		if (currentTable->judgeVarOrConst(currentNode->child[0]->getInfo()))
		{
			pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
			pcode.push_back(pCode("LOD", tmp.pre, tmp.off));
		}
		else
		{
			pcode.push_back(pCode("LIT", 0, currentTable->findConst(currentNode->child[0]->getInfo())));
		}
	}
}

void Parser::handleCondition(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ODD)
	{
		handleExpression(currentNode->child[1], currentTable);
		pcode.push_back(pCode("OPR", 0, 6));
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::EXPRESSION)
	{
		handleExpression(currentNode->child[0], currentTable);
		handleConditionPlus(currentNode->child[1], currentTable);
	}
}

void Parser::handleConditionPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleExpression(currentNode->child[1], currentTable);
	switch (currentNode->child[0]->getType())
	{
	case GrammarSymSpace::EQUAL:
		pcode.push_back(pCode("OPR", 0, 8));
		return;
	case GrammarSymSpace::NOTEQUAL:
		pcode.push_back(pCode("OPR", 0, 9));
		return;
	case GrammarSymSpace::LESSTHAN:
		pcode.push_back(pCode("OPR", 0, 10));
		return;
	case GrammarSymSpace::GREATEREQUAL:
		pcode.push_back(pCode("OPR", 0, 11));
		return;
	case GrammarSymSpace::GREATERTHAN:
		pcode.push_back(pCode("OPR", 0, 12));
		return;
	case GrammarSymSpace::LESSEQUAL:
		pcode.push_back(pCode("OPR", 0, 13));
		return;
	default:
		break;
	}
}

void Parser::handleStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ASSIGHNSTATEMENT)
	{
		handleAssignStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CALLSTATEMENT)
	{
		handleCallStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::COMPOUNDSTATEMENT)
	{
		handleCompoundStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CONTIDITIONSTATEMENT)
	{
		handleConditionStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::LOOPSTATEMENT)
	{
		handleLoopStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::READSTATEMENT)
	{
		handleReadStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::WRITESTATEMENT)
	{
		handleWriteStatement(currentNode->child[0], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::REPEATSTATEMENT)
	{
		handleRepeatStatement(currentNode->child[0], currentTable);
	}
}

void Parser::handleAssignStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	currentNode = currentNode->child[2];
	handleExpression(currentNode, currentTable);
	pos tmp = currentTable->findVar(currentNode->getFather()->child[0]->getInfo());
	pcode.push_back(pCode("STO", tmp.pre, tmp.off));
}

void Parser::handleCallStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	pcode.push_back(pCode("JMP", 0, currentTable->findProcedure(currentNode->child[1]->getInfo())));
}

void Parser::handleCompoundStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleStatementTable(currentNode->child[1], currentTable);
}

void Parser::handleConditionStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleCondition(currentNode->child[1], currentTable);
	pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
	handleStatement(currentNode->child[3], currentTable);
	for (int i = pcode.size() - 1; i > -1; --i)
	{
		if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
		{
			pcode[i].a2 = pcode.size();
			break;
		}
	}
	handleConditionStatementPlus(currentNode->child[4], currentTable);
}

void Parser::handleConditionStatementPlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ELSE)
	{
		for (int i = pcode.size() - 1; i > -1; --i)
		{
			if (pcode[i].op == "JPC")
			{
				++pcode[i].a2;
				break;
			}
		}
		pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
		handleStatement(currentNode->child[1], currentTable);
		for (int i = pcode.size() - 1; i > -1; --i)
		{
			if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
			{
				pcode[i].a2 = pcode.size();
				break;
			}
		}
	}
}

void Parser::handleLoopStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	int jump = pcode.size();
	handleCondition(currentNode->child[1], currentTable);
	pcode.push_back(pCode("JPC", 0, 0x3f3f3f));
	//int jump = pcode.size();
	handleStatement(currentNode->child[3], currentTable);
	//handleCondition(currentNode->child[1], currentTable);
	for (int i = pcode.size() - 1; i > -1; --i)
	{
		if (pcode[i].op == "JPC" && pcode[i].a2 == 0x3f3f3f)
		{
			pcode[i].a2 = pcode.size() + 1;
			break;
		}
	}
	pcode.push_back(pCode("JMP", 0, jump));
}

void Parser::handleReadStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleReadVarTable(currentNode->child[2], currentTable);
}

void Parser::handleReadVarTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleReadVar(currentNode->child[0], currentTable);
	handleReadVarTablePlus(currentNode->child[1], currentTable);
}

void Parser::handleReadVarTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::COMMA)
	{
		handleReadVar(currentNode->child[1], currentTable);
		handleReadVarTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleReadVar(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
	pcode.push_back(pCode("RED", tmp.pre, tmp.off));
}

void Parser::handleWriteStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleWriteExpressionTable(currentNode->child[2], currentTable);
}

void Parser::handleWriteExpressionTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	handleExpression(currentNode->child[0], currentTable);
	pcode.push_back(pCode("WRT", 0, 0));
	handleWriteExpressionTablePlus(currentNode->child[1], currentTable);
}

void Parser::handleWriteExpressionTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::COMMA)
	{
		handleExpression(currentNode->child[1], currentTable);
		pcode.push_back(pCode("WRT", 0, 0));
		handleWriteExpressionTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::handleRepeatStatement(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	int jump = pcode.size();
	handleStatementTable(currentNode->child[1], currentTable);
	handleCondition(currentNode->child[3], currentTable);
	pcode.push_back(pCode("JPC", 0, pcode.size() + 2));
	pcode.push_back(pCode("JMP", 0, jump));
}

void Parser::handleStatementTable(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::ID)
	{
		handleExpression(currentNode->child[2], currentTable);
		pos tmp = currentTable->findVar(currentNode->child[0]->getInfo());
		pcode.push_back(pCode("STO", tmp.pre, tmp.off));
		handleStatementTablePlus(currentNode->child[3], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CALL)
	{
		pcode.push_back(pCode("JMP", 0, currentTable->findProcedure(currentNode->child[1]->getInfo())));
		handleStatementTablePlus(currentNode->child[2], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::BEGIN)
	{
		handleStatementTable(currentNode->child[1], currentTable);
		handleStatementTablePlus(currentNode->child[3], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::CONTIDITIONSTATEMENT)
	{
		handleConditionStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::LOOPSTATEMENT)
	{
		handleLoopStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::READSTATEMENT)
	{
		handleReadStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::WRITESTATEMENT)
	{
		handleWriteStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::REPEATSTATEMENT)
	{
		handleRepeatStatement(currentNode->child[0], currentTable);
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
	else if (currentNode->child[0]->getType() == GrammarSymSpace::EMPTY)
	{
		handleStatementTablePlus(currentNode->child[1], currentTable);
	}
}

void Parser::handleStatementTablePlus(AstNode * n, sTable * s)
{
	AstNode* currentNode = n;
	sTable* currentTable = s;
	if (currentNode->child[0]->getType() == GrammarSymSpace::SEMICOLON)
	{
		handleStatement(currentNode->child[1], currentTable);
		handleStatementTablePlus(currentNode->child[2], currentTable);
	}
}

void Parser::printPCode()
{
	for (int i = 0; i < pcode.size(); ++i)
	{
		cout << i << "\t" << pcode[i].op << "  " << pcode[i].a1 << "\t" << pcode[i].a2 << "\n";
	}
}


