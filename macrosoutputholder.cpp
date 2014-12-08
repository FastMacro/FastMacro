#include "macrosoutputholder.h"

MacrosOutputHolder::~MacrosOutputHolder()
{
	delete keyset;
}

QString MacrosOutputHolder::getName()
{
	return name;
}

QString MacrosOutputHolder::getType()
{
	return type;
}

Command **MacrosOutputHolder::getCommandList()
{
	return commandList;
}

int MacrosOutputHolder::getCommandListSize()
{
	return commandListSize;
}

QString MacrosOutputHolder::getKeystring()
{
	return keystring;
}

QSet<QString>* MacrosOutputHolder::getKeyset()
{
	return keyset;
}
