#include "sender.h"

Sender::Sender(AddingDialog *mDial, Controller *mController)
	: mDial(mDial), mController(mController)
{
}

void Sender::newMacrosWasCreated()
{
	getAndSend();
}

void Sender::getAndSend()
{
	mController->makeMacros(mDial->getName(), mDial->getPath());
}
