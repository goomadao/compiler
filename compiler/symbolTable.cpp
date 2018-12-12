#include "pch.h"
#include "symbolTable.h"

symbolTable::symbolTable()
{
	pre = nullptr;
}

symbolTable::symbolTable(symbolTable * pres)
{
	pre = pres;
}
