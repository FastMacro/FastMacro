#include "commandline.h"

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
			/// Если нашли - запустили
			mMacros->value(curStr)->exec();
			mList.clear();
			break;
		}
		--i;
		curStr.push_front(*i);
	}
}

void CommandLine::catchChar(QChar key)
{
	qDebug() << "CommandLine : char : " <<  key;
	add(key);
}
