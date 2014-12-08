#include "macrosoutputholder.h"


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
