#include "commandline.h"
#include "keypressfilter.h"
#include "keysetconverter.h"

CommandLine::CommandLine(int size, QMap<QString, Macros*> *macroses)
	: mSize(size), mMacros(macroses)
{
}

void CommandLine::add(QChar &currentChar)
{
	qDebug() << "start";
	if (mList.size() == mSize)
		mList.pop_front();
	mList.push_back(currentChar);
	scan();
	qDebug() << "finish";
}

void CommandLine::scan()
	{
	QList<QChar>::iterator i;
	i = mList.end();
	--i;
	QString curStr(*i);
	while(i + 1 != mList.begin())
	{
		if (mMacros->contains(curStr))
		{
			/// Если нашли - запустили
			mMacros->value(curStr)->exec();
			mList.clear();
			break;
		}
		--i;
		curStr.push_front(*i);
	}
}

void CommandLine::scanShortcut()
{
	QSet<QString> *pressedKeys = KeyPressFilter::getInstance()->getPressedKeys();

	QMap<QString, Macros*>::iterator i;
	for (i = mMacros->begin(); i != mMacros->end(); ++i) {
		if (i.key()[0] != '#')
			continue;
		QSet<QString> *macrosSet = KeySetConverter::getInstance()->toSet(i.key());
		if (*macrosSet == *pressedKeys) {
			delete macrosSet;
			i.value()->exec();
			break;
		}
		delete macrosSet;
	}
}

void CommandLine::catchChar(QChar key)
{
	//qDebug() << "CommandLineAAA : char : " <<  key;
	add(key);
	scanShortcut();
	//qDebug() << "scan  done : " <<  key;
}

void CommandLine::catchMouseEvent(QString type)
{
	QMap<QString, Macros*>::iterator i;
	for (i = mMacros->begin(); i != mMacros->end(); ++i)
	{
		if (i.key() == type)
			i.value()->exec();
	}
}
