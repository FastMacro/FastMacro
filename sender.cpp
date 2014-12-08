#include "sender.h"

Sender::Sender(AddingDialog *mDial, Controller *mController, CurrentMacrosesDialog *mCurrent)
	: mDial(mDial), mController(mController), mCurrentMacroses(mCurrent)
{
}

void Sender::newMacrosWasCreated()
{
	getAndSend();
}

void Sender::needMacroses()
{
	mCurrentMacroses->showMacroses(mController->getMacroses());
}

void Sender::editMacros(const QString &macrosName)
{
	QMap<QString, Macros*> *macroses = mController->getMacroses();
	foreach (Macros *value, *macroses)
		if (value->getName() == macrosName) {
			mDial->editMacros(value);
			return;
		}
}

void Sender::getAndSend()
{
	mController->makeMacros(mDial->holder);
}

void Sender::deleteMacros(const QString &name)
{
	mController->deleteMacros(name);
}
