#pragma once
#include "command.h"


/// Придумать как лучше реализовать список комманд макроса
class Macros
{
public:
	Macros(Command *command);
	Macros(){}
	void exec();
private:
	Command *mCommand;
};
