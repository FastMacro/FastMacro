#include "macros.h"
#include <QDebug>
#include <string>

Macros::Macros(const QString &macrosName, Command *command)
{
	commandList = new Command*[1];
	commandList[0] = command;
	commandListSize = 1;
	name = macrosName;
}

Macros::Macros(const QString &macrosName, Command **commands, int size)
{
	commandList = commands;
	commandListSize = size;
	name = macrosName;
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

QPair<Command **, int> Macros::getCommandList()
{
	QPair<Command **, int> cPair(commandList, commandListSize);
	return cPair;
}
