#pragma once
#include "command.h"

class MacrosOutputHolder {
public:
	MacrosOutputHolder(const QString &newName, const QString &newType, Command **newList, int newListSize, const QString &newPath, QSet<QString> *newKeys)
		: name(newName), type(newType), commandList(newList), commandListSize(newListSize), path(newPath), keys(newKeys) {}
	QString getName();
	QString getType();
	Command** getCommandList();
	int getCommandListSize();
	QString getPath();
	QSet<QString>* getKeys();
private:
	QString name;
	QString type;
	Command **commandList;
	int commandListSize;
	QString path;
	QSet<QString> *keys;
};
