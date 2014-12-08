#include "sender.h"

Sender::Sender(AddingDialog *mDial, Controller *mController, CurrentMacrosesDialog *mCurrnet)
	: mDial(mDial), mController(mController), mCurrentMacroses(mCurrnet)
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
	mDial->editMacros(mController->getMacroses()->value(macrosName));
}

void Sender::getAndSend()
{
	mController->makeMacros(mDial->holder);
}

void Sender::deleteMacros(QString name)
{
	mController->deleteMacros(name);
}
