#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <QDebug>
#include "macros.h"
#include "macrosoutputholder.h"

class MacrosFactory : public QObject
{
	Q_OBJECT

public:
	MacrosFactory(QMap<QString, Macros*> *macroses);
	~MacrosFactory();
	void makeMacros(MacrosOutputHolder *holder);

private:
	QMap<QString, Macros*> * mMacros;
};
