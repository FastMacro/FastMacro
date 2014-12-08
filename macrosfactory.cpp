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
	Macros *newMacros = new Macros(holder);
	mMacros->insert(newMacros->getKeystring(), newMacros);
}
