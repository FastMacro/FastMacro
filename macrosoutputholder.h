#pragma once
#include "command.h"

class MacrosOutputHolder {
public:
	MacrosOutputHolder(const QString &newName, const QString &newType, Command **newList, int newListSize, const QString &newKeystring, QSet<QString> *newSet)
		: name(newName), type(newType), commandList(newList), commandListSize(newListSize), keystring(newKeystring.toUpper()), keyset(newSet) {}
	~MacrosOutputHolder();
	QString getName();
	QString getType();
	Command** getCommandList();
	int getCommandListSize();
	QString getKeystring();
	QSet<QString> *getKeyset();
private:
	QString name;
	QString type;
	Command **commandList;
	int commandListSize;
	QString keystring;
	QSet<QString> *keyset;
};
