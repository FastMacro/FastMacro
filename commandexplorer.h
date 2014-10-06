#pragma once
#include <QString>
#include <QDesktopServices>
#include <QUrl>
#include <QObject>
#include <QDebug>
#include "macrosfactory.h"

class CommandExplorer : public QObject
{
	Q_OBJECT

public:
	CommandExplorer();
	void doCommand(QString command);
	void updateMap(MacrosFactory * macrosFactory);

public slots:
	void catchCommand(QString command);

private:
	QMap<QString, QString> * map;
};
