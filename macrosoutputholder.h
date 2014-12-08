#pragma once
#include "command.h"

class MacrosOutputHolder {
public:
	MacrosOutputHolder(const QString &newName, const QString &newType, Command **newList, int newListSize, const QString &newKeystring)
		: name(newName), type(newType), commandList(newList), commandListSize(newListSize), keystring(newKeystring.toUpper()) {}
	QString getName();
	QString getType();
	Command** getCommandList();
	int getCommandListSize();
	QString getKeystring();
private:
	QString name;
	QString type;
	Command **commandList;
	int commandListSize;
	QString keystring;
};
