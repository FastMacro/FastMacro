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
	MacrosFactory(QMap<QString, Macros*> *macroses);
	~MacrosFactory();
	void makeMacros(const QString &key, Command **commands, int size);

private:
	QMap<QString, Macros*> * mMacros;
};
