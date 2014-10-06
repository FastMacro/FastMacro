#include "macrosfactory.h"

MacrosFactory::MacrosFactory()
{
	map = new QMap<QString, QString>();
}

MacrosFactory::~MacrosFactory()
{
	delete map;
}

void MacrosFactory::makeMacros(QString macros, QString path)
{
	map->insert(macros, path);
	emit wasUpdated();
}

void MacrosFactory::catchMacros(QString str)
{
	int index = 0;
	for (; index < str.size(); ++index)
		if (str.at(index) == QChar('#'))
			break;
	qDebug() << str << " " << str.mid(0, index) << " " << str.mid(index + 1, str.size() - index - 1);
	makeMacros(str.mid(0, index), str.mid(index + 1, str.size() - index - 1));
}

QMap<QString, QString> * MacrosFactory::getMap()
{
	return map;
}
