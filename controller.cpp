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

void Controller::makeMacros(QString &key, QString &path)
{
	/// Нужно придумать как лучше передавать какой именно вид макроса определен (web, file, system)
	mFactory->makeMacros(key, path);
}

void Controller::setConnection(KeyPressFilter *keyFilter)
{
	connect(keyFilter, SIGNAL(throwChar(QChar)), mCommandLine, SLOT(catchChar(QChar)));
}
