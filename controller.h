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
	void setEnabled(KeyPressFilter *keyFilter, bool enable);
	QMap<QString, Macros*> *getMacroses();

private:
	CommandLine *mCommandLine;
	QMap<QString, Macros*> *mMacros;
	MacrosFactory *mFactory;
	MacrosDataController *mMacrosDataController;
	int mSize = 30;
	bool enabled = true;
};
