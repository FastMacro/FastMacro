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
	mMacros->insert(holder->getKeystring(), new Macros(holder));
}
