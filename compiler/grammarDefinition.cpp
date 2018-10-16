#include "pch.h"
#include "grammarDefinition.h"


string GrammarDefinition::GrammarSymTypes[89] =
{
		"����",
		"�ֳ���",
		"˵������",
		"����˵������",
		"���������",
		"��������",
		"����˵������",
		"������",
		"����",
		"����˵���б�",
		"�����ײ�",
		"������",
		"���",
		"��ֵ���",
		"���̵������",
		"�������",
		"�������",
		"����ѭ�����",
		"�����",
		"д���",
		"����",
		"��������",
		"������",
		"д���ʽ��",
		"����",
		"���ʽ",
		"��",
		"����",
		"����+",
		"�ֳ���+",
		"˵������+",
		"����˵������+",
		"���������+",
		"��������+",
		"����˵������+",
		"������+",
		"����+",
		"����˵���б�+",
		"�����ײ�+",
		"������+",
		"���+",
		"��ֵ���+",
		"���̵������+",
		"�������+",
		"�������+",
		"����ѭ�����+",
		"�����+",
		"д���+",
		"����+",
		"��������+",
		"������+",
		"д���ʽ��+",
		"����+",
		"���ʽ+",
		"��+",
		"����+",
		"���ʽ++",
		"const",
		"id",
		"integer",
		"var",
		"procedure",
		"call",
		"begin",
		"end",
		"=",
		".",
		";",
		",",
		":=",
		"if",
		"then",
		"while",
		"do",
		"read",
		"(",
		")",
		"write",
		"odd",
		"<",
		"<=",
		">",
		">=",
		"+",
		"-",
		"*",
		"/",
		"��",
		"$"
};


//GrammarDefinition::GrammarDefinition()
//{
//	;
//}
//
//GrammarDefinition::GrammarDefinition(int left)
//{
//	leftpart = left;
//}

GrammarDefinition::GrammarDefinition(int left, vector<int> right)
{
	leftpart = left;
	rightpart = right;
}

bool GrammarDefinition::operator==(const GrammarDefinition & g)
{
	return this->leftpart == g.leftpart && this->rightpart == g.rightpart;
}

bool GrammarDefinition::startWithNonterminal()
{
	if (rightpart.empty() || rightpart[0] > 55)
		return false;
	return true;
}

bool GrammarDefinition::theFirstNonterminalOfRightExpressionEqualToAnotherLeftPart(GrammarDefinition g)
{
	if (this->rightpart[0] == g.leftpart)
		return true;
	return false;
}

bool GrammarDefinition::isLeftRecursive()
{
	if (rightpart.empty())
		return false;
	if (leftpart == rightpart[0])
		return true;
	return false;
}

bool GrammarDefinition::isLeftPartEqual(GrammarDefinition g)
{
	if (this->leftpart == g.leftpart)
		return true;
	return false;
}

GrammarDefinition GrammarDefinition::changeTheFirstNonterminalOfRightExpressionToItsProduction()
{
	vector<int> tmp(this->rightpart.begin() + 1, this->rightpart.end());
	tmp.push_back(this->leftPart() + 28);
	return GrammarDefinition(this->leftPart() + 28, tmp);
}

GrammarDefinition GrammarDefinition::changeTheFirstNonterminalOfRightExpressionToItsProduction(GrammarDefinition g)
{

	vector<int>tmp = g.rightpart;

	tmp.insert(tmp.end(), this->rightpart.begin() + 1, this->rightpart.end());
	return GrammarDefinition(this->leftpart, tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminate(GrammarDefinition g)
{
	vector<int> tmp = g.rightpart;
	tmp.push_back(this->leftpart + 28);
	return GrammarDefinition(this->leftpart,tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminatePlus()
{
	vector<int> tmp(this->rightpart.begin() + 1, this->rightpart.end());
	tmp.push_back(this->leftpart + 28);
	return GrammarDefinition(this->leftpart + 28, tmp);
}

GrammarDefinition GrammarDefinition::generateSymWhenEliminatePlusEmpty()
{
	return GrammarDefinition(this->leftpart + 28, vector<int>{GrammarSym::EMPTY});
}

ostream & operator<<(ostream & out, const GrammarDefinition & g)
{
	// TODO: �ڴ˴����� return ���
	out <<"<"<< g.leftpart << ">" << GrammarDefinition::GrammarSymTypes[g.leftpart] << "\t\t" << "==>";
	for (int i = 0; i < g.rightpart.size(); ++i)
	{
		out <<"<"<<g.rightpart[i]<<">"<< GrammarDefinition::GrammarSymTypes[g.rightpart[i]];
	}
	out << "\n";
	return out;
}
