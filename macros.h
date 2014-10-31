#pragma once
#include "command.h"

class Macros
{
public:
	Macros(){}
	Macros(Command *command);
	Macros(Command **commands, int size);
	void exec();
private:
	Command **commandList = nullptr;
	int commandListSize = 0;
};
