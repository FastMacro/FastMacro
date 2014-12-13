#include "controller.h"

Controller::Controller()
{
	mMacrosDataController = new MacrosDataController;
	mMacros = mMacrosDataController->load();
	mCommandLine = new CommandLine(mSize, mMacros);
	mFactory = new MacrosFactory(mMacros);
}

Controller::~Controller()
{
	delete mMacros;
	delete mCommandLine;
	delete mFactory;
}

QMap<QString, Macros*> * Controller::getMacroses()
{
	return mMacros;
}

void Controller::makeMacros(MacrosOutputHolder *holder)
{
	mFactory->makeMacros(holder);
	mMacrosDataController->save(mMacros);
}

void Controller::setConnection(KeyPressFilter *keyFilter)
{
	connect(keyFilter, SIGNAL(throwChar(QChar)), mCommandLine, SLOT(catchChar(QChar)));
	connect(keyFilter, SIGNAL(throwMouseEvent()), mCommandLine, SLOT(catchMouseEvent()));
}

void Controller::deleteMacros(const QString &name)
{
	for (QMap<QString, Macros*>::iterator it = mMacros->begin(); it != mMacros->end(); ++it) {
		if (it.value()->getName() == name) {
			mMacros->erase(it);
			break;
		}
	}
	mMacrosDataController->save(mMacros);
}

void Controller::setEnabled(KeyPressFilter *keyFilter, bool enable)
{
	keyFilter->blockSignals(!enable);
}
