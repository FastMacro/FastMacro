#include "macros.h"
#include <QDebug>
#include <string>
#include "keysetconverter.h"

Macros::Macros(MacrosOutputHolder *holder)
{
	commandList = holder->getCommandList();
	commandListSize = holder->getCommandListSize();
	name = holder->getName();
	type = holder->getType();
	if (!holder->getKeyset())
		keystring = holder->getKeystring();
	else if (type == "shortcut")
		keystring = KeySetConverter::getInstance()->toString(holder->getKeyset());
}

Macros::~Macros()
{
	delete commandList;
}

void Macros::exec()
{
	for (int i = 0; i < commandListSize; i++)
		commandList[i]->exec();
}

QString Macros::getName()
{
	return name;
}

QString Macros::getType() {
	return type;
}

QString Macros::getKeystring()
{
	return keystring;
}

QPair<Command **, int> Macros::getCommandList()
{
	QPair<Command **, int> cPair(commandList, commandListSize);
	return cPair;
}
