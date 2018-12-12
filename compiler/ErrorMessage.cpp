#include "pch.h"
#include "ErrorMessage.h"

errorMessage::errorMessage(Position p, string m)
{
	pos = p;
	msg = m;
}

bool errorMessage::operator<(const errorMessage & e) const
{
	return this->pos < e.pos;
}

bool errorMessage::operator>(const errorMessage & e) const
{
	return this->pos > e.pos;
}

ostream & operator<<(ostream & out, const errorMessage & e)
{
	out << e.pos << ":     \t" << e.msg << "\n";
	return out;
	// TODO: �ڴ˴����� return ���
}
