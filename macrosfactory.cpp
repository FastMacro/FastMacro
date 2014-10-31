#include "macrosfactory.h"

MacrosFactory::MacrosFactory(QMap<QString, Macros> *macroses) :
	mMacros(macroses)
{
	macroses->insert("VK", Macros(new WebpageCommand("http://vk.com/feed")));
	macroses->insert("CMD", Macros(new SystemCommand("cmd")));
}

MacrosFactory::~MacrosFactory()
{
}

void MacrosFactory::makeMacros(const QString &key, Command **commands, int size)
{
	/// Тут должны создаваться разные, но пока есть только 1
	mMacros->insert(key.toUpper(), Macros(commands, size));
}
