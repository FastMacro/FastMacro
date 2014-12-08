#pragma once
#include <QObject>
#include <QString>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include "commandline.h"
#include "macros.h"
#include "command.h"
#include "macrosfactory.h"

class MacrosDataController : public QObject
{
	Q_OBJECT

public:
	MacrosDataController();
	~MacrosDataController();

	void save(QMap<QString, Macros*> *macroses);
	QMap<QString, Macros *> *load();

private:
	Command *parseCommand(QDomNode docElem);
	Macros *parseMacros(QDomNode docElem);
	QSet<QString> *parseKeys(QDomNode docElem);
};
