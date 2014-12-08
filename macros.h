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
	virtual QString getType() = 0;
	static Macros* createMacros(MacrosOutputHolder *holder);
private:
	Command **commandList = nullptr;
	int commandListSize = 0;
	QString name;
	QString type;
};

class KeyStringMacros : public Macros
{
public:
	KeyStringMacros(MacrosOutputHolder *holder);
	QString getKeyString();
	QString getType();
private:
	QString keyString;
};

class ShortcutMacros : public Macros
{
public:
	ShortcutMacros(MacrosOutputHolder *holder);
	QSet<QString> *getKeys();
	QString getType();
private:
	QSet<QString> *keys;
};
