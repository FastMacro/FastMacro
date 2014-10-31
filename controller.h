#pragma once

#include <QObject>
#include <QMap>
#include "commandline.h"
#include "macros.h"
#include "macrosfactory.h"
#include "keypressfilter.h"

class Controller : public QObject
{
	Q_OBJECT
public:
	Controller();
	~Controller();
	void makeMacros(QString key, Command **commands, int size);
	void setConnection(KeyPressFilter *keyFilter);

private:
	CommandLine *mCommandLine;
	QMap<QString, Macros> *mMacros;
	MacrosFactory *mFactory;
	int mSize = 10;
};
