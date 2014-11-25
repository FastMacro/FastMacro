#pragma once

#include <QObject>
#include <QMap>
#include "commandline.h"
#include "macros.h"
#include "macrosfactory.h"
#include "keypressfilter.h"
#include "macrosdatacontroller.h"

class Controller : public QObject
{
	Q_OBJECT
public:
	Controller();
	~Controller();
	void makeMacros(QString key, Command **commands, int size);
	void setConnection(KeyPressFilter *keyFilter);
	void deleteMacros(const QString &name);
	QMap<QString, Macros*> *getMacroses();

private:
	CommandLine *mCommandLine;
	QMap<QString, Macros*> *mMacros;
	MacrosFactory *mFactory;
	MacrosDataController *mMacrosDataController;
	int mSize = 30;
};
