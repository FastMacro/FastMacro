#include "macros.h"
#include <QDebug>
#include <string>

Macros::Macros(Command *command)
	: mCommand(command)
{
}

void Macros::exec()
{
	mCommand->exec();
}
