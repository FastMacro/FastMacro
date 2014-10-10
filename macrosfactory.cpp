#include "macrosfactory.h"

MacrosFactory::MacrosFactory(QMap<QString, Macros> *macroses) :
	mMacros(macroses)
{
	macroses->insert("VK", Macros(new WebpageCommand("http://vk.com/feed")));
	macroses->insert("CMD", Macros(new SystemCommand("gnome-terminal")));
}

MacrosFactory::~MacrosFactory()
{
}

void MacrosFactory::makeMacros(QString &key, QString &path)
{
	/// Тут должны создаваться разные, но пока есть только 1
	mMacros->insert(key, Macros(new FileCommand(path)));
}
