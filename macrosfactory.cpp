#include "macrosfactory.h"

MacrosFactory::MacrosFactory(QMap<QString, Macros> *macroses) :
	mMacros(macroses)
{
	macroses->insert("VK", Macros("VK", new WebpageCommand("http://vk.com/feed")));
	macroses->insert("CMD", Macros("CMD", new SystemCommand("cmd")));
}

MacrosFactory::~MacrosFactory()
{
}

void MacrosFactory::makeMacros(const QString &key, Command **commands, int size)
{
	mMacros->insert(key.toUpper(), Macros(key, commands, size));
}
