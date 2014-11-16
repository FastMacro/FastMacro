#include "controller.h"

Controller::Controller()
{
	mMacros = new QMap<QString, Macros>;
	mCommandLine = new CommandLine(mSize, mMacros);
	mFactory = new MacrosFactory(mMacros);
}

Controller::~Controller()
{
	delete mMacros;
	delete mCommandLine;
	delete mFactory;
}

QMap<QString, Macros> * Controller::getMacroses()
{
	return mMacros;
}

void Controller::makeMacros(QString key, Command **commands, int size)
{
	mFactory->makeMacros(key, commands, size);
}

void Controller::setConnection(KeyPressFilter *keyFilter)
{
	connect(keyFilter, SIGNAL(throwChar(QChar)), mCommandLine, SLOT(catchChar(QChar)));
}

void Controller::deleteMacros(const QString &name)
{
	mMacros->remove(name);
}
