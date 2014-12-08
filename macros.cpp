#include "macros.h"
#include <QDebug>
#include <string>

Macros::Macros(MacrosOutputHolder *holder)
{
	commandList = holder->getCommandList();
	commandListSize = holder->getCommandListSize();
	name = holder->getName();
	type = holder->getType();
}

void Macros::exec()
{
	for (int i = 0; i < commandListSize; i++)
		commandList[i]->exec();
}

QString Macros::getName()
{
	return name;
}

QPair<Command **, int> Macros::getCommandList()
{
	QPair<Command **, int> cPair(commandList, commandListSize);
	return cPair;
}

Macros* Macros::createMacros(MacrosOutputHolder *holder) {
	Macros *newMacros = nullptr;
	if (holder->getType() == "keystring")
		newMacros = new KeyStringMacros(holder);
	else if (holder->getType() == "shortcut")
		newMacros = new ShortcutMacros(holder);
	return newMacros;
}

KeyStringMacros::KeyStringMacros(MacrosOutputHolder *holder) : Macros(holder) {
	keyString = holder->getPath();
}

QString KeyStringMacros::getKeyString() {
	return keyString;
}

QString KeyStringMacros::getType() {
	return "keystring";
}

ShortcutMacros::ShortcutMacros(MacrosOutputHolder *holder) : Macros(holder) {
	keys = holder->getKeys();
}

QSet<QString> *ShortcutMacros::getKeys() {
	return keys;
}

QString ShortcutMacros::getType() {
	return "shortcut";
}
