#include "commandline.h"
#include "keypressfilter.h"
#include "keysetconverter.h"

CommandLine::CommandLine(int size, QMap<QString, Macros*> *macroses)
	: mSize(size), mMacros(macroses)
{
}

void CommandLine::add(QChar &currentChar)
{
	if (mList.size() == mSize)
		mList.pop_front();
	mList.push_back(currentChar);
	scan();
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
			QList<Macros*> list = mMacros->values(curStr);
			foreach(Macros *value, list)
				value->exec();
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
		if (*macrosSet == *pressedKeys)
			i.value()->exec();
		delete macrosSet;
	}
}

void CommandLine::catchChar(QChar key)
{
	qDebug() << "CommandLine : char : " <<  key;
	add(key);
	scanShortcut();
}

void CommandLine::catchMouseEvent()
{
	QMap<QString, Macros*>::iterator i;
	for (i = mMacros->begin(); i != mMacros->end(); ++i)
	{
		if (i.key() == "$HORIZONTALLINE")
			i.value()->exec();
	}
}
