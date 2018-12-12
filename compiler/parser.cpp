/*Todo:
	i>>10的报错没有解决

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
			cout << "语法分析完成\n";
			break;
		}
		if (parsing.top() == GrammarSymSpace::EMPTY)//栈顶为空，直接出栈，并且向上寻找下一个该分析的节点（向父节点递归）
		{
			currentNode->setInfo("ε");
			parsing.pop();
			while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
			{
				currentNode = currentNode->getFather();
				if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
				{
					cout << "无法找到下一个待分析的节点\n";
					if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
						break;
					else
						return;
				}
			}
			if (currentNode != syntaxTree)
				currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

			cout << "\n";
			continue;
		}
		if (isTerminal(parsing.top()))//栈顶是终结符，和剩余代码比对，匹配则都出栈，否则进入错误处理
		{
			if (parsing.top() == lexerTypeToGrammarType(currentToekn))//终结符匹配
			{
				currentNode->setInfo(currentToekn.getVal());//设置这个节点的属性=token的Val
				while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
				{
					currentNode = currentNode->getFather();
					if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
					{
						cout << "无法找到下一个待分析的节点\n";
						if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
							break;
						else
							return;
					}
				}
				if (currentNode != syntaxTree)
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "和" << currentToekn.getVal() << "匹配,出栈\n";
				parsing.pop();
				if (lexer.tokensEnd() && parsing.empty())
					;
				else
					currentToekn = lexer.nextToken();
			}
			else//不匹配，开始错误处理
			{
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "和" << currentToekn.getVal() << "不匹配!\n";
				
				//先找出从这个节点向上递归所有父节点的所有follow集，一直找到根节点
				//先判断当前指向的token，再取下一个token，直到这个token和之前找到的follow集中的某个终结符匹配
				//currentNode开始向父节点递归，直到这个节点的follow集中包含这个token
				errorAdd(currentToekn.getPos(), "Expect \"" + GrammarDefinition::GrammarSymTypes[parsing.top()] + "\", while found \"" + currentToekn.getVal() + "\"");

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				while (tmp != syntaxTree)//一直递归到根节点找到所有的follow中的终结符
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
					
					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//当前指向的token在follows中
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
							}//首先把分析栈中这个节点所在的产生式中还没分析的部分出栈（因为要跳过）
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//一定是非终结符
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
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
						cout << "代码已空，语法分析失败\n";
						return;
					}
					currentToekn = lexer.nextToken();//读入下一个token
				}
				if (flag)//还需要将这个非终结符出栈，下一个非终结符才能和currentToken匹配
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								break;
							else
								return;
						}
					}
					if (currentNode != syntaxTree)
						currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

				}
				else if (!terminalMatch)//找不到匹配
				{
					cout << "无法跳出该错误，语法分析失败\n";
					return;
				}
				else//直接终结符匹配了，大多数情况下是多了某个符号，删去即可（删去表现为上面跳过了这个token）
				{
					;
				}




			}
		}
		else//栈顶为非终结符，查预测表，有相应的项目则用产生式替换，否则进入错误处理
		{
			//预测表中没有对应的项目，进入错误处理
			if (grammar.parsingTable.find(parsingNode(parsing.top(), lexerTypeToGrammarType(currentToekn)))->second.empty())
			{
				cout << "对于(" << GrammarDefinition::GrammarSymTypes[parsing.top()] << "," << currentToekn.getVal() << "),预测表中没有对应的项目\n";
				
				errorAdd(currentToekn.getPos(), "Unexpected token \"" + currentToekn.getVal() + "\"");


				//先找出从这个节点向上递归所有父节点的所有follow集，一直找到根节点(这个节点不要找，因为这个节点开始不匹配！)
				//先判断当前指向的token，再取下一个token，直到这个token和之前找到的follow集中的某个终结符匹配
				//currentNode开始向父节点递归，直到这个节点的follow集中包含这个token（这个节点也要检查）

				bool flag = 0, terminalMatch = 0;
				set<int>follows;
				AstNode* tmp = currentNode;
				//set<int>::iterator it;//先加入当前节点的follow集
				//for (it = grammar.follow.find(tmp->getType())->second.begin(); it != grammar.follow.find(tmp->getType())->second.end(); ++it)
				//{
				//	follows.insert(*it);
				//}
				while (tmp != syntaxTree)//一直递归到根节点找到所有的follow中的终结符
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

					if (follows.find(lexerTypeToGrammarType(currentToekn)) != follows.end())//当前指向的token在follows中
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
							//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
							flag = 1;
							break;
						}
						while (currentNode != syntaxTree)
						{
							for (int i = currentNode->getWz(); i < currentNode->getFather()->child.size() - 1; ++i)
							{
								parsing.pop();
							}//首先把分析栈中这个节点所在的产生式中还没分析的部分出栈（因为要跳过）
							cout << "1: "; printParsing();
							currentNode = currentNode->getFather();//一定是非终结符
							if (grammar.follow.find(currentNode->getType())->second.find(lexerTypeToGrammarType(currentToekn)) != grammar.follow.find(currentNode->getType())->second.end())
							{
								//这个非终结符的follow集中有当前指向的token，找到了可以继续进行分析的Node
								flag = 1;
								break;
							}
						}
						if (flag)
							break;
					}
					if (lexer.tokensEnd())
					{
						cout << "代码已空，语法分析失败\n";
						return;
					}
					currentToekn = lexer.nextToken();//读入下一个token
				}
				if (flag)//还需要将这个非终结符出栈，下一个非终结符才能和currentToken匹配
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								break;
							else
								return;
						}
					}
					if (currentNode != syntaxTree)
						currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个

				}
				else if (!terminalMatch)//找不到匹配
				{
					cout << "无法跳出该错误，语法分析失败\n";
					return;
				}
				else//直接终结符匹配了，大多数情况下是多了某个符号，删去即可（删去表现为上面跳过了这个token）
				{
					;
				}

				



			}
			else//预测表中找到了对应的项目，用产生式替换。
			{
				int top = parsing.top();
				
				cout << GrammarDefinition::GrammarSymTypes[parsing.top()] << "出栈;";

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
					temp.push_back(new AstNode(expand.top(), count++, currentToekn.getPos(), currentNode));//count表示在Vector中的索引,父节点是currentNode
					expand.pop();
				}
				currentNode->child = temp;
				if (temp.empty())
				{
					while (currentNode->getWz() == currentNode->getFatherSize() - 1)//如果这个节点在孩子节点中是最后一个，就递归向上找到他的父节点并且这个节点在孩子节点中不是最后一个
					{
						currentNode = currentNode->getFather();
						if (currentNode == syntaxTree)//一直找到根节点都是在孩子节点中的最后一个，则报错
						{
							cout << "无法找到下一个待分析的节点\n";
							if (parsing.top() == GrammarSymSpace::PERIOD && currentToekn.getType() == symbol::PERIODSYM)
								;
							else
								return;
						}
					}
					currentNode = currentNode->getFather()->child[currentNode->getWz() + 1];//找到了在孩子节点中不是最后一个的节点，下一个待分析的节点就是孩子节点中这个节点的后面那个
				}
				else
					currentNode = temp[0];//currentNode已经出栈了，所以孩子节点中的第一个一定会成为currentNode
				cout << "入栈\n";
			}
		}
		cout << "栈:";
		printParsing();
		cout << "\n";
	}
	printSyntaxTree(syntaxTree);//输出语法树的叶节点（可能会输出ε）
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

