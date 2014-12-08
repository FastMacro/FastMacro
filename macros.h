#pragma once
#include "command.h"
#include <QPair>
#include <QSet>
#include "macrosoutputholder.h"

class Macros
{
public:
	Macros(MacrosOutputHolder *holder);
	QPair<Command **, int> getCommandList();
	void exec();
	QString getName();
	QString getType();
	QString getKeystring();
	static Macros* createMacros(MacrosOutputHolder *holder);
private:
	Command **commandList = nullptr;
	int commandListSize = 0;
	QString name;
	QString keystring;
	QString type;
};
