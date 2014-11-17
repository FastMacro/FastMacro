#pragma once
#include "command.h"
#include <QPair>

class Macros
{
public:
	Macros(){}
	Macros(Command *command);
	Macros(Command **commands, int size);
	QPair<Command **, int> getCommandList();
    void exec();

private:
	Command **commandList = nullptr;
	int commandListSize = 0;
};
