#include "commandexplorer.h"

CommandExplorer::CommandExplorer()
{
}

void CommandExplorer::catchCommand(QString command)
{
	doCommand(command);
}

void CommandExplorer::doCommand(QString command)
{
	qDebug() << command;
	if (command == QString("VK"))
	{
		QDesktopServices::openUrl(QUrl("http://vk.com/feed", QUrl::TolerantMode));
	}
	else if (command == QString("CMD"))
	{
        system("cmd");
	}
	else if (map->contains(command))
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(map->value(command)));
	}
}

void CommandExplorer::updateMap(MacrosFactory *macrosFactory)
{
	map = macrosFactory->getMap();
}
