#include "macrosfactory.h"

MacrosFactory::MacrosFactory(QMap<QString, Macros *> *macroses) :
	mMacros(macroses)
{
}

MacrosFactory::~MacrosFactory()
{
}

void MacrosFactory::makeMacros(MacrosOutputHolder *holder)
{
	mMacros->insert(holder->getName(), Macros::createMacros(holder));
}
