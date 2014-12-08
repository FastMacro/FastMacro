#include "controller.h"

Controller::Controller()
{
	mMacrosDataController = new MacrosDataController;
	mKeystringMacros = mMacrosDataController->load();
	mCommandLine = new CommandLine(mSize, mKeystringMacros, mShortcutMacros);
	mFactory = new MacrosFactory(mMacros);
}

Controller::~Controller()
{
	delete mKeystringMacros;
	delete mCommandLine;
	delete mFactory;
}

QMap<QString, Macros*> * Controller::getKeystringMacroses()
{
	return mMacros;
}

QMap<QSet, Macros*> * Controller::getShortcutMacroses() {

}

void Controller::makeMacros(MacrosOutputHolder *holder)
{
	mFactory->makeMacros(holder);
	mMacrosDataController->save(mMacros);
}

void Controller::setConnection(KeyPressFilter *keyFilter)
{
	connect(keyFilter, SIGNAL(throwChar(QChar)), mCommandLine, SLOT(catchChar(QChar)));
}

void Controller::deleteMacros(const QString &name)
{
	mMacros->remove(name);
	mMacrosDataController->save(mMacros);
}
