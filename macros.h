#pragma once
#include "command.h"
#include <QPair>

class Macros
{
public:
	Macros(){}
	Macros(const QString &macrosName, Command *command);
	Macros(const QString &macrosName, Command **commands, int size);
	QPair<Command **, int> getCommandList();
	void exec();
	QString getName();

private:
	Command **commandList = nullptr;
	int commandListSize = 0;
	QString name;
};
