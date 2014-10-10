#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <QDebug>
#include "macros.h"

class MacrosFactory : public QObject
{
	Q_OBJECT

public:
	MacrosFactory(QMap<QString, Macros> *macroses);
	~MacrosFactory();
	void makeMacros(QString &key, QString &path);

private:
	QMap<QString, Macros> * mMacros;
};
