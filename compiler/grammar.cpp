


#include "pch.h"

#include "grammar.h"
#include<algorithm>
#include<iomanip>
using namespace GrammarSym;
Grammar::Grammar()
{
	getOriginGrammar();
	getOriginList();

	/*cout << "----------------ԭʼ�ķ���------------------\n";
	printGrammarList();*/
	//transportToList();
	eliminateLeftRecursive();
	//leftFactoring();

	

	
	
	getFirst();
	

	getFollow();
	

	getParsingTable();
	
	//�����list
	cout << "----------------�����ķ���------------------\n";
	printGrammarList();
	cout << "-----------------first:---------------\n";
	printFirst();
	
	cout << "----------------follow:----------------\n";
	printFollow();
	cout << "----------------parsingTable:----------------\n";
	printParsingTable();
	
}

void Grammar::getOriginGrammar()
{
	grammarSet.push_back(GrammarDefinition(PROCEDURES, vector<int>{SUBPROCEDURE, PERIOD}));
	grammarSet.push_back(GrammarDefinition(SUBPROCEDURE, vector<int>{DEFINITIONPART, STATEMENT}));
	grammarSet.push_back(GrammarDefinition(DEFINITIONPART, vector<int>{CONSTDEFINITIONPART, VARDEFINITIONPART, PROCEDURELIST}));
	grammarSet.push_back(GrammarDefinition(CONSTDEFINITIONPART, vector<int>{CONST, CONSTDEFINITIONTABLE, SEMICOLON}));
	grammarSet.push_back(GrammarDefinition(CONSTDEFINITIONPART, vector<int>{EMPTY}));
	grammarSet.push_back(GrammarDefinition(CONSTDEFINITIONTABLE, vector<int>{CONSTDEFINITIONTABLE, COMMA, CONSTDEFINITION}));
	grammarSet.push_back(GrammarDefinition(CONSTDEFINITIONTABLE, vector<int>{CONSTDEFINITION}));
	grammarSet.push_back(GrammarDefinition(CONSTDEFINITION, vector<int>{ID, EQUAL, INTEGER}));
	grammarSet.push_back(GrammarDefinition(VARDEFINITIONPART, vector<int>{VAR, VARDEFINITIONTABLE, SEMICOLON}));
	grammarSet.push_back(GrammarDefinition(VARDEFINITIONPART, vector<int>{EMPTY}));
	grammarSet.push_back(GrammarDefinition(VARDEFINITIONTABLE, vector<int>{VARDEFINITIONTABLE, COMMA, VARDEFINITION}));
	grammarSet.push_back(GrammarDefinition(VARDEFINITIONTABLE, vector<int>{VARDEFINITION}));
	grammarSet.push_back(GrammarDefinition(VARDEFINITION, vector<int>{ID}));
	grammarSet.push_back(GrammarDefinition(PROCEDURELIST, vector<int>{PROCEDURELIST, SEMICOLON, PROCEDUREHEADER, PROCEDUREBODY}));
	grammarSet.push_back(GrammarDefinition(PROCEDURELIST, vector<int>{PROCEDUREHEADER, PROCEDUREBODY}));
	grammarSet.push_back(GrammarDefinition(PROCEDURELIST, vector<int>{EMPTY}));
	grammarSet.push_back(GrammarDefinition(PROCEDUREHEADER, vector<int>{PROCEDURE, ID, SEMICOLON}));
	grammarSet.push_back(GrammarDefinition(PROCEDUREBODY, vector<int>{SUBPROCEDURE, SEMICOLON}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{ASSIGHNSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{CALLSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{COMPOUNDSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{CONTIDITIONSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{LOOPSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{READSTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{WRITESTATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENT, vector<int>{EMPTY}));
	grammarSet.push_back(GrammarDefinition(ASSIGHNSTATEMENT, vector<int>{ID, BECOMES, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CALLSTATEMENT, vector<int>{CALL, ID}));
	grammarSet.push_back(GrammarDefinition(COMPOUNDSTATEMENT, vector<int>{BEGIN, STATEMENTTABLE, END}));
	grammarSet.push_back(GrammarDefinition(STATEMENTTABLE, vector<int>{STATEMENTTABLE, SEMICOLON, STATEMENT}));
	grammarSet.push_back(GrammarDefinition(STATEMENTTABLE, vector<int>{STATEMENT}));
	grammarSet.push_back(GrammarDefinition(CONTIDITIONSTATEMENT, vector<int>{IF, CONDITION, THEN, STATEMENT}));
	grammarSet.push_back(GrammarDefinition(LOOPSTATEMENT, vector<int>{WHILE, CONDITION, DO, STATEMENT}));
	grammarSet.push_back(GrammarDefinition(READSTATEMENT, vector<int>{READ, LEFTPARENT, READVARTABLE, RIGHTPARENT}));
	grammarSet.push_back(GrammarDefinition(READVARTABLE, vector<int>{READVARTABLE, COMMA, READVAR}));
	grammarSet.push_back(GrammarDefinition(READVARTABLE, vector<int>{READVAR}));
	grammarSet.push_back(GrammarDefinition(READVAR, vector<int>{ID}));
	grammarSet.push_back(GrammarDefinition(WRITESTATEMENT, vector<int>{WRITE, LEFTPARENT, WRITEEXPRESSIONTABLE, RIGHTPARENT}));
	grammarSet.push_back(GrammarDefinition(WRITEEXPRESSIONTABLE, vector<int>{WRITEEXPRESSIONTABLE, COMMA, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(WRITEEXPRESSIONTABLE, vector<int>{EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{ODD, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{EXPRESSION, EQUAL, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{EXPRESSION, LESSTHAN, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{EXPRESSION, LESSEQUAL, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{EXPRESSION, GREATERTHAN, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(CONDITION, vector<int>{EXPRESSION, GREATEREQUAL, EXPRESSION}));
	grammarSet.push_back(GrammarDefinition(EXPRESSION, vector<int>{PLUS, TERM}));
	grammarSet.push_back(GrammarDefinition(EXPRESSION, vector<int>{MINUS, TERM}));
	grammarSet.push_back(GrammarDefinition(EXPRESSION, vector<int>{TERM}));
	grammarSet.push_back(GrammarDefinition(EXPRESSION, vector<int>{EXPRESSION, PLUS, TERM}));
	grammarSet.push_back(GrammarDefinition(EXPRESSION, vector<int>{EXPRESSION, MINUS, TERM}));
	grammarSet.push_back(GrammarDefinition(TERM, vector<int>{TERM, TIMES, FACTOR}));
	grammarSet.push_back(GrammarDefinition(TERM, vector<int>{TERM, DIVISION, FACTOR}));
	grammarSet.push_back(GrammarDefinition(TERM, vector<int>{ FACTOR}));
	grammarSet.push_back(GrammarDefinition(FACTOR, vector<int>{ID}));
	grammarSet.push_back(GrammarDefinition(FACTOR, vector<int>{INTEGER}));
	grammarSet.push_back(GrammarDefinition(FACTOR, vector<int>{LEFTPARENT, EXPRESSION, RIGHTPARENT}));
	
	//������vectot
	/*for (int i = 0; i < grammarSet.size(); ++i)
	{
		cout << i + 1 << ":\t" << grammarSet[i];
	}*/
}

void Grammar::getOriginList()
{
	grammarList.push_back(list< vector<int> >{vector<int>{PROCEDURES}, vector<int>{SUBPROCEDURE, PERIOD}});
	grammarList.push_back(list< vector<int> >{vector<int>{SUBPROCEDURE}, vector<int>{DEFINITIONPART, STATEMENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{DEFINITIONPART}, vector<int>{CONSTDEFINITIONPART, VARDEFINITIONPART, PROCEDURELIST}});
	grammarList.push_back(list< vector<int> >{vector<int>{CONSTDEFINITIONPART}, vector<int>{CONST, CONSTDEFINITIONTABLE, SEMICOLON}, vector<int>{EMPTY}});
	grammarList.push_back(list< vector<int> >{vector<int>{CONSTDEFINITIONTABLE}, vector<int>{CONSTDEFINITIONTABLE, COMMA, CONSTDEFINITION}, vector<int>{CONSTDEFINITION}});
	grammarList.push_back(list< vector<int> >{vector<int>{CONSTDEFINITION}, vector<int>{ID, EQUAL, INTEGER}});
	grammarList.push_back(list< vector<int> >{vector<int>{VARDEFINITIONPART}, vector<int>{VAR, VARDEFINITIONTABLE, SEMICOLON}, vector<int>{EMPTY}});
	grammarList.push_back(list< vector<int> >{vector<int>{VARDEFINITIONTABLE}, vector<int>{VARDEFINITIONTABLE, COMMA, VARDEFINITION}, vector<int>{VARDEFINITION}});
	grammarList.push_back(list< vector<int> >{vector<int>{VARDEFINITION}, vector<int>{ID}});
	grammarList.push_back(list< vector<int> >{vector<int>{PROCEDURELIST}, vector<int>{PROCEDURELIST, PROCEDUREHEADER, PROCEDUREBODY}, vector<int>{EMPTY}});
	grammarList.push_back(list< vector<int> >{vector<int>{PROCEDUREHEADER}, vector<int>{PROCEDURE, ID, SEMICOLON}});
	grammarList.push_back(list< vector<int> >{vector<int>{PROCEDUREBODY}, vector<int>{SUBPROCEDURE, SEMICOLON}});
	grammarList.push_back(list< vector<int> >{vector<int>{STATEMENT}, vector<int>{ASSIGHNSTATEMENT}, vector<int>{CALLSTATEMENT}, vector<int>{COMPOUNDSTATEMENT}, vector<int>{CONTIDITIONSTATEMENT}, vector<int>{LOOPSTATEMENT}, vector<int>{READSTATEMENT}, vector<int>{WRITESTATEMENT}, vector<int>{EMPTY}});
	grammarList.push_back(list< vector<int> >{vector<int>{ASSIGHNSTATEMENT}, vector<int>{ID, BECOMES, EXPRESSION}});
	grammarList.push_back(list< vector<int> >{vector<int>{CALLSTATEMENT}, vector<int>{CALL, ID}});
	grammarList.push_back(list< vector<int> >{vector<int>{COMPOUNDSTATEMENT}, vector<int>{BEGIN, STATEMENTTABLE, END}});
	grammarList.push_back(list< vector<int> >{vector<int>{STATEMENTTABLE}, vector<int>{STATEMENTTABLE, SEMICOLON, STATEMENT}, vector<int>{STATEMENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{CONTIDITIONSTATEMENT}, vector<int>{IF, CONDITION, THEN, STATEMENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{LOOPSTATEMENT}, vector<int>{WHILE, CONDITION, DO, STATEMENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{READSTATEMENT}, vector<int>{READ, LEFTPARENT, READVARTABLE, RIGHTPARENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{READVARTABLE}, vector<int>{READVARTABLE, COMMA, READVAR}, vector<int>{READVAR}});
	grammarList.push_back(list< vector<int> >{vector<int>{READVAR}, vector<int>{ID}});
	grammarList.push_back(list< vector<int> >{vector<int>{WRITESTATEMENT}, vector<int>{WRITE, LEFTPARENT, WRITEEXPRESSIONTABLE, RIGHTPARENT}});
	grammarList.push_back(list< vector<int> >{vector<int>{WRITEEXPRESSIONTABLE}, vector<int>{WRITEEXPRESSIONTABLE, COMMA, EXPRESSION}, vector<int>{EXPRESSION}});
	
	
	
	//grammarList.push_back(list< vector<int> >{vector<int>{CONDITION}, vector<int>{ODD, EXPRESSION}, vector<int>{EXPRESSION, EQUAL, EXPRESSION}, vector<int>{EXPRESSION, LESSTHAN, EXPRESSION}, vector<int>{EXPRESSION, LESSEQUAL, EXPRESSION}, vector<int>{EXPRESSION, GREATERTHAN, EXPRESSION}, vector<int>{EXPRESSION, GREATEREQUAL, EXPRESSION}});
	
	//�ֶ��������ʽ������
	grammarList.push_back(list< vector<int> >{vector<int>{CONDITION}, vector<int>{ODD, EXPRESSION}, vector<int>{EXPRESSION, EXPRESSIONPLUSPLUS}});
	grammarList.push_back(list< vector<int> >{vector<int>{EXPRESSIONPLUSPLUS}, vector<int>{EQUAL, EXPRESSION}, vector<int>{LESSTHAN, EXPRESSION}, vector<int>{LESSEQUAL, EXPRESSION}, vector<int>{GREATERTHAN, EXPRESSION}, vector<int>{GREATEREQUAL, EXPRESSION}});
	
	
	grammarList.push_back(list< vector<int> >{vector<int>{EXPRESSION}, vector<int>{PLUS, TERM}, vector<int>{MINUS, TERM}, vector<int>{TERM}, vector<int>{EXPRESSION, PLUS, TERM}, vector<int>{EXPRESSION, MINUS, TERM}});
	grammarList.push_back(list< vector<int> >{vector<int>{TERM}, vector<int>{FACTOR}, vector<int>{TERM, TIMES, FACTOR}, vector<int>{TERM, DIVISION, FACTOR}});
	grammarList.push_back(list< vector<int> >{vector<int>{FACTOR}, vector<int>{ID}, vector<int>{INTEGER}, vector<int>{LEFTPARENT, EXPRESSION, RIGHTPARENT}});
	

/*
	grammarList.push_back(list< vector<int> >{vector<int>{EXPRESSION}, vector<int>{EXPRESSION, PLUS, TERM}, vector<int>{TERM } });
	grammarList.push_back(list< vector<int> >{vector<int>{TERM}, vector<int>{TERM, TIMES, FACTOR}, vector<int>{FACTOR}});
	grammarList.push_back(list< vector<int> >{vector<int>{FACTOR}, vector<int>{ID}, vector<int>{LEFTPARENT, EXPRESSION, RIGHTPARENT}});
*/

	////�����list
	//int djt = 1;
	//list< list< vector<int> > >::iterator it = grammarList.begin();//it:list <list< vector<int> > >��������ָ��ĳ��list< vector<int> >
	//for (; it != grammarList.end(); ++it, ++djt)
	//{
	//	cout << djt << ":\t";
	//	list< vector<int> >::iterator iter = it->begin();//iter:list< vector<int> >��������ָ��ĳ��vector<int>
	//	for (; iter != it->end(); ++iter)
	//	{
	//		for (int i = 0; i < iter->size(); ++i)
	//		{
	//			cout << GrammarDefinition::GrammarSymTypes[iter->at(i)] << " ";
	//		}
	//		list< vector<int> >::iterator temp = iter;
	//		++temp;
	//		if (iter == it->begin())
	//		{
	//			cout << "\t\t==>";
	//		}
	//		else if (temp != it->end())
	//		{
	//			cout << "|";
	//		}
	//	}
	//	cout << "\n";
	//}
}

void Grammar::printGrammarList()
{
	int djt = 1;
	list< list< vector<int> > >::iterator it = grammarList.begin();//it:list <list< vector<int> > >��������ָ��ĳ��list< vector<int> >
	for (; it != grammarList.end(); ++it, ++djt)
	{
		cout << djt << ":\t";
		list< vector<int> >::iterator iter = it->begin();//iter:list< vector<int> >��������ָ��ĳ��vector<int>
		for (; iter != it->end(); ++iter)
		{
			for (int i = 0; i < iter->size(); ++i)
			{
				if (iter == it->begin())
					cout << left << setw(16) << GrammarDefinition::GrammarSymTypes[iter->at(i)] << " ";
				else
					cout << GrammarDefinition::GrammarSymTypes[iter->at(i)] << " ";
			}
			list< vector<int> >::iterator temp = iter;
			++temp;
			if (iter == it->begin())
			{
				cout << "==>";
			}
			else if (temp != it->end())
			{
				cout << "|";
			}
		}
		cout << "\n";
	}
}

//�����List����
/*
void Grammar::transportToList()
{
	for (int i = 0; i < grammarSet.size(); ++i)
	{
		list< int > tempList;
		tempList.push_back(grammarSet[i].leftPart());
		for (int j = 0; j < grammarSet[i].rightPartSize(); ++j)
		{
			tempList.push_back(grammarSet[i].rightPartAt(j));
		}
		grammarList.push_back(tempList);
	}



	//������list
	list< list<int> >::iterator it;
	int djt = 1;
	for (it = grammarList.begin(); it != grammarList.end(); ++it, ++djt)
	{
		list<int>tempList = *it;
		cout << djt << ":\t" << GrammarDefinition::GrammarSymTypes[tempList.front()] << "\t\t==>";
		tempList.pop_front();
		while (!tempList.empty())
		{
			cout << GrammarDefinition::GrammarSymTypes[tempList.front()] << " ";
			tempList.pop_front();
		}
		cout << "\n";
	}


}
*/

void Grammar::eliminateLeftRecursive()
{
	//չ�����ʽ
	list< list< vector<int> > >::iterator i1, i2;
	list< vector<int> >::iterator i3, i4;
	for (i1 = grammarList.begin(); i1 != grammarList.end(); ++i1)//Ai
	{
		for (i2 = grammarList.begin(); i2 != i1; ++i2)//Aj
		{
			i3 = i1->begin();//Ai��
			for (++i3; i3 != i1->end();)//����Ai��ÿһ���Ҳ�
			{
				vector<int> searchVec = *i3;//ĳ���Ҳ�
				int leftNonTerminal = i2->front().at(0);//Aj��
				bool isFind = 0;
				if (searchVec[0] == leftNonTerminal)
				{
					isFind = 1;
					i4 = i2->begin();//ָ��Aj�󲿵ĵ�����
					for (++i4; i4 != i2->end(); ++i4)//ָ��Ajĳ���Ҳ��ĵ�����
					{
						vector<int> res = *i4;
						res.insert(res.end(), searchVec.begin() + 1, searchVec.end());
						i1->insert(i3, res);
					}
					i3 = i1->erase(i3);
				}
				if (!isFind)
				{
					++i3;
				}
			}
		}

		//����ֱ����ݹ�
		bool hasLeftRecursive = 0;
		i4 = i1->begin();
		vector< vector<int> >tempVector;//�����ݹ��зǵݹ鲿�֣���Ai->Aj���еĦ�
		//cout << "\n" << GrammarDefinition::GrammarSymTypes[i1->front()[0]] << ":----------------------------------\n";
		for (++i4; i4 != i1->end();)
		{
			if (i4->at(0) == i1->front()[0])
			{
				//cout << GrammarDefinition::GrammarSymTypes[i4->at(0)] << "ƥ�䣡\n";
				hasLeftRecursive = 1;
				vector<int> nonRecursivePart(i4->begin() + 1, i4->end());
				tempVector.push_back(nonRecursivePart);
				i4 = i1->erase(i4);
			}
			else
			{
				++i4;
			}
		}
		if (hasLeftRecursive)
		{
			i4 = i1->begin();
			for (++i4; i4 != i1->end(); ++i4)
			{
				i4->push_back(i1->front()[0] + 28);//�ڷǵݹ����ʽ�Ҷ���󶼼����µķ��ս������Ai->�ñ�ΪAi->��Ai'
			}

			for (int i = 0; i < tempVector.size(); ++i)
			{
				tempVector[i].push_back(i1->front()[0] + 28);//����ݹ����ʽ�зǵݹ鲿�ֵ��������µķ��ս����->��Ai'
			}

			list< vector<int> >newGrammar;
			newGrammar.push_back(vector<int>{i1->front()[0] + 28});
			for (int i = 0; i < tempVector.size(); ++i)
			{
				newGrammar.push_back(tempVector[i]);
			}
			newGrammar.push_back(vector<int>{EMPTY});
			list< list< vector<int> > >::iterator insert_iter = i1;
			grammarList.insert(++insert_iter, newGrammar);
			++i1;
		}




	}



}


void Grammar::leftFactoring()
{
}

void Grammar::getFirst()
{
	for (int i = 0; i < 88; ++i)
	{
		calculateFirst(i);
	}
}

void Grammar::calculateFirst(int x)
{
	if (first.find(x) != first.end())//�Ѿ��������First����
	{
		return;
	}
	if (x > 56)//�ս��
	{
		first.insert(pair<int, set<int> >{x, { x }});//�ս����First���Ͼ���������
	}
	else//���ս��
	{
		first.insert(pair<int, set<int> >{x, set<int>{}});
		list< list< vector<int> > >::iterator i1 = grammarList.begin();//ѭ���ҵ������������ս��Ϊ��˵����в���ʽ�����������ʽ�����fitst��
		list< vector<int> >::iterator i2;//ָ��ò���ʽ��ĳһ���Ҷ�
		vector<int>::iterator i3;//ָ�����ʽ��ĳһ���Ҷ��е�ĳ����ʶ��
		set<int>::iterator i4;
		for (; i1 != grammarList.end(); ++i1)
		{
			if (i1->begin()->at(0) == x)
			{
				i2 = i1->begin();
				for (++i2; i2 != i1->end(); ++i2)
				{
					for (i3 = i2->begin(); i3 != i2->end(); ++i3)
					{
						calculateFirst(*i3);
						bool flag = false;
						for (i4 = first.find(*i3)->second.begin(); i4 != first.find(*i3)->second.end(); ++i4)
						{
							if (*i4 != EMPTY)
								first.find(x)->second.insert(*i4);
							else
								flag = true;
						}
						if (!flag)
						{
							break;
						}
						if (flag && ++i3 == i2->end())
						{
							first.find(x)->second.insert(EMPTY);
						}
						--i3;
					}
				}

			}
		}



	}
}

void Grammar::printFirst()
{
	map<int, set<int> >::iterator i1;
	set<int>::iterator i2;
	for (i1 = first.begin(); i1 != first.end(); ++i1)
	{
		cout << left << setw(16) << GrammarDefinition::GrammarSymTypes[i1->first] << ":";
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			cout << GrammarDefinition::GrammarSymTypes[*i2] << ", ";
		}
		cout << "\n";
	}
}

void Grammar::getFollow()
{
	follow.insert(pair<int, set<int> >{0, set<int>{ENDEOF}});
	//follow.insert(pair<int, set<int> >{EXPRESSION, set<int>{ENDEOF}});
	int oldSize = 0, newSize = 0x3f3f3f;
	for (int i = 0; i < 88; ++i)
	{
		follow.insert(pair<int, set<int> >{i, set<int>{}});
	}
	while (oldSize != newSize)
	{
		calculateFollow();
		oldSize = newSize;
		newSize = getFollowSize();
	}
}

void Grammar::calculateFollow()
{
	list< list< vector<int> > >::iterator i1;//ĳ������ʽ
	list< vector<int> >::iterator i2;//ĳ��vector
	vector<int>::iterator i3;//ĳ����ʶ��
	set<int>::iterator i4;
	for (i1 = grammarList.begin(); i1 != grammarList.end(); ++i1)
	{
		i2 = i1->begin();
		for (++i2; i2 != i1->end(); ++i2)
		{
			if (i2->size() == 0)
			{
				continue;
			}
			else if (i2->size() == 1 && i2->at(0) < 57)
			{
				for (i4 = follow.find(i1->begin()->at(0))->second.begin(); i4 != follow.find(i1->begin()->at(0))->second.end(); ++i4)
				{
					follow.find(i2->at(0))->second.insert(*i4);
				}
			}
			else if (i2->size() > 1)
			{
				for (i3 = i2->begin(); i3 != i2->end(); ++i3)
				{
					if (*i3 > 56)
					{
						continue;
					}
					++i3;
					if (i3 == i2->end())
					{
						--i3;
						for (i4 = follow.find(i1->begin()->at(0))->second.begin(); i4 != follow.find(i1->begin()->at(0))->second.end(); ++i4)
						{
							follow.find(*i3)->second.insert(*i4);
						}
						continue;
					}
					vector<int>::iterator it = i3;
					--i3;
					for (; it != i2->end(); ++it)
					{
						bool flag = false;//�ж�first������û�Ц�
						for (i4 = first.find(*it)->second.begin(); i4 != first.find(*it)->second.end(); ++i4)
						{
							if (*i4 != EMPTY)
							{
								follow.find(*i3)->second.insert(*i4);
							}
							else
							{
								flag = true;
							}
						}
						if (flag)
						{
							continue;
						}
						else
						{
							break;
						}
					}
					if (it == i2->end())//˵��first�����㵽�����һ������i3��������б��ʽ�����Ƴ���
					{
						for (i4 = follow.find(i1->begin()->at(0))->second.begin(); i4 != follow.find(i1->begin()->at(0))->second.end(); ++i4)
						{
							follow.find(*i3)->second.insert(*i4);
						}
					}
				}
			}
		}
	}
}

int Grammar::getFollowSize()
{
	map<int, set<int> >::iterator it;
	int sum = 0;
	for (it = follow.begin(); it != follow.end(); ++it)
	{
		sum += it->second.size();
	}
	return sum;
}

void Grammar::printFollow()
{
	map<int, set<int> >::iterator i1;
	set<int>::iterator i2;
	for (i1 = follow.begin(); i1 != follow.end(); ++i1)
	{
		cout << left << setw(16) << GrammarDefinition::GrammarSymTypes[i1->first] << ":";
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			cout << GrammarDefinition::GrammarSymTypes[*i2] << ", ";
		}
		cout << "\n";
	}
}

void Grammar::getParsingTable()
{
	for (int i = 0; i < 57; ++i)
	{
		for (int j = 57; j < 89; ++j)
		{
			parsingTable.insert(pair<parsingNode, vector<int> >{parsingNode(i, j), vector<int>{}});
		}
	}

	list< list< vector<int> > >::iterator i1;//ĳ������ʽ
	list< vector<int> >::iterator i2;//��˻���ĳ���Ҷ�
	vector<int>::iterator i3;//ĳ����ʶ��
	set<int>::iterator i4;
	for (i1 = grammarList.begin(); i1 != grammarList.end(); ++i1)
	{
		i2 = i1->begin();
		for (++i2; i2 != i1->end(); ++i2)
		{
			/*cout << left << setw(14) << GrammarDefinition::GrammarSymTypes[i1->front()[0]] << "->";
			for (int i = 0; i < i2->size(); ++i)
			{
				cout << GrammarDefinition::GrammarSymTypes[i2->at(i)] << " ";
			}
			cout << "\n";*/
			for (i3 = i2->begin(); i3 != i2->end(); ++i3)
			{
				bool flag = false;//�жϿɲ������Ƴ���
				for (i4 = first.find(*i3)->second.begin(); i4 != first.find(*i3)->second.end(); ++i4)//i3����ı�ʶ����first���е�ÿһ���ս��
				{
					if (*i4 == EMPTY)
					{
						flag = true;
						continue;
					}
					if (parsingTable.find(parsingNode(i1->front()[0], *i4))->second.size() != 0)//�ýڵ��Ѿ���Ӧ��ĳһ������ʽ��
					{
						cout << "\nfirst���ж�����\n";
						cout << GrammarDefinition::GrammarSymTypes[*i4] << "\n";
						cout << GrammarDefinition::GrammarSymTypes[i1->front()[0]] << "->\n";
						cout << "�ɣ�\n";
						for (vector<int>::iterator tempit = parsingTable.find(parsingNode(i1->front()[0], *i4))->second.begin(); tempit != parsingTable.find(parsingNode(i1->front()[0], *i4))->second.end(); ++tempit)
						{
							cout << GrammarDefinition::GrammarSymTypes[*tempit] << " ";
						}
						cout << "\n�£�\n";
						for (vector<int>::iterator tempit = i2->begin(); tempit != i2->end(); ++tempit)
						{
							cout << GrammarDefinition::GrammarSymTypes[*tempit] << " ";
						}
						cout << "\n\n";
						//return;
					}
					parsingTable.find(parsingNode(i1->front()[0], *i4))->second = *i2;
				}
				if (flag)
				{
					continue;
				}
				else
				{
					break;
				}
			}
			if (i3 == i2->end())
			{
				for (i4 = follow.find(i1->front()[0])->second.begin(); i4 != follow.find(i1->front()[0])->second.end(); ++i4)
				{
					if (*i4 == EMPTY)
					{
						continue;
					}
					if (parsingTable.find(parsingNode(i1->front()[0], *i4))->second.size() != 0)
					{
						cout << "\nfollow���ж�����\n";
						cout << GrammarDefinition::GrammarSymTypes[*i4] << "\n";
						cout << GrammarDefinition::GrammarSymTypes[i1->front()[0]] << "->\n";
						cout << "�ɣ�\n";
						for (vector<int>::iterator tempit = parsingTable.find(parsingNode(i1->front()[0], *i4))->second.begin(); tempit != parsingTable.find(parsingNode(i1->front()[0], *i4))->second.end(); ++tempit)
						{
							cout << GrammarDefinition::GrammarSymTypes[*tempit] << " ";
						}
						cout << "\n�£�\n";
						for (vector<int>::iterator tempit = i2->begin(); tempit != i2->end(); ++tempit)
						{
							cout << GrammarDefinition::GrammarSymTypes[*tempit] << " ";
						}
						cout << "\n\n";
						//return;
					}
					parsingTable.find(parsingNode(i1->front()[0], *i4))->second = *i2;
				}
			}
		}
	}

}

void Grammar::printParsingTable()
{
	map<parsingNode, vector<int> >::iterator i1;
	vector<int>::iterator i2;
	for (i1 = parsingTable.begin(); i1 != parsingTable.end(); ++i1)
	{
		if (i1->second.size() == 0)
		{
			continue;
		}
		cout << left << setw(14) << GrammarDefinition::GrammarSymTypes[i1->first.nonTerminal] << left << setw(14) << GrammarDefinition::GrammarSymTypes[i1->first.terminal] << ":   ";
		for (i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			cout << GrammarDefinition::GrammarSymTypes[*i2] << " ";
		}
		cout << "\n";
	}
}


