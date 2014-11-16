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

void Sender::getAndSend()
{
	mController->makeMacros(mDial->outputKey, mDial->outputCommandList, mDial->outputSize);
}

void Sender::deleteMacros(QString name)
{
	mController->deleteMacros(name);
}
