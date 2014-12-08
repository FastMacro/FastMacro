#pragma once

#include <QObject>
#include <QMap>
#include "commandline.h"
#include "macros.h"
#include "macrosfactory.h"
#include "keypressfilter.h"
#include "macrosdatacontroller.h"
#include "macrosoutputholder.h"

class Controller : public QObject
{
	Q_OBJECT
public:
	Controller();
	~Controller();
	void makeMacros(MacrosOutputHolder *holder);
	void setConnection(KeyPressFilter *keyFilter);
	void deleteMacros(const QString &name);
	QMap<QString, Macros*> *getKeystringMacroses();
	QMap<QSet, Macros*> *getShortcutMacroses();

private:
	CommandLine *mCommandLine;
	QMap<QString, Macros*> *mKeystringMacros;
	QMap<QSet, Macros*> *mShortcutMacros;
	MacrosFactory *mFactory;
	MacrosDataController *mMacrosDataController;
	int mSize = 30;
};
