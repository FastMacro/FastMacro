#pragma once
#include <QChar>
#include <QString>
#include <QList>
#include <QObject>
#include <QDebug>
#include "macrosfactory.h"

class CommandLine : public QObject
{
	Q_OBJECT

public:
	CommandLine(int size, QList<QString> &commandList);
	void add(QChar &currentChar);
	void updateMap(MacrosFactory * macrosFactory);

signals:
	void throwCommand(QString command);

public slots:
	void catchChar(QChar key);

private:
	void scan();
	QList<QChar> list;
	QMap<QString, QString> * map;
	QList<QString> commandList;
	int size;
};
