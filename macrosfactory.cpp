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
	mMacros->insertMulti(newMacros->getKeystring(), newMacros);
}
