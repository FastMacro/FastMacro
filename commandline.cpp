#include "commandline.h"

CommandLine::CommandLine(int size, QList<QString> &commandList)
	: size(size), commandList(commandList), map(new QMap<QString, QString>)
{
}

void CommandLine::add(QChar &currentChar)
{
	if (list.size() == size)
		list.pop_front();
	list.push_back(currentChar);
	scan();
}

void CommandLine::scan()
{
	qDebug() << "scan : " << map->size();
	QList<QChar>::iterator i;
	 i = list.end();
	 --i;
	 QString curStr(*i);
	 while(i + 1 != list.begin())
	 {
		 if (commandList.contains(curStr) || map->contains(curStr))
		 {
			 emit throwCommand(curStr);
			 list.clear();
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

void CommandLine::updateMap(MacrosFactory *macrosFactory)
{
	map = macrosFactory->getMap();
}
