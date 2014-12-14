#include "commandline.h"
#include "keypressfilter.h"
#include "keysetconverter.h"

CommandLine::CommandLine(int size, QMap<QString, Macros*> *macroses)
	: mSize(size), mMacros(macroses)
{
	drawKeys = new QSet<QString>;
	drawKeys->insert("CTRL");
	drawKeys->insert("F1");
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
	add(key);
	scanShortcut();
}

void CommandLine::catchMouseEvent(QString type)
{
	QSet<QString> *pressedKeys = KeyPressFilter::getInstance()->getPressedKeys();
	if (*pressedKeys != *drawKeys)
		return;
	QMap<QString, Macros*>::iterator i;
	for (i = mMacros->begin(); i != mMacros->end(); ++i)
	{
		if (i.key() == type)
			i.value()->exec();
	}
}
