#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <QDebug>

class MacrosFactory : public QObject
{
	Q_OBJECT

public:
	MacrosFactory();
	~MacrosFactory();
	void makeMacros(QString macros, QString path);
	QMap<QString, QString> * getMap();

signals:
	void wasUpdated();

public slots:
	void catchMacros(QString str);

private:
	QMap<QString, QString> * map;
};
