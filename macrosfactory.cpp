#include "macrosfactory.h"

MacrosFactory::MacrosFactory(QMap<QString, Macros *> *macroses) :
	mMacros(macroses)
{
}

MacrosFactory::~MacrosFactory()
{
}

void MacrosFactory::makeMacros(const QString &key, Command **commands, int size)
{
	mMacros->insert(key.toUpper(), new Macros(key, commands, size));
}
