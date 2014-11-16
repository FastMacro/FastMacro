#include "macros.h"
#include <QDebug>
#include <string>

Macros::Macros(Command *command)
{
	commandList = new Command*[1];
	commandList[0] = command;
	commandListSize = 1;
}

Macros::Macros(Command **commands, int size)
{
	commandList = commands;
	commandListSize = size;
}

void Macros::exec()
{
	for (int i = 0; i < commandListSize; i++)
		commandList[i]->exec();
}

QPair<Command **, int> Macros::getCommandList()
{
	QPair<Command **, int> cPair(commandList, commandListSize);
	return cPair;
}
