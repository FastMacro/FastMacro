#pragma once
#include <QChar>
#include <QString>
#include <QMap>
#include <QObject>
#include <QDebug>
#include "macros.h"

class CommandLine : public QObject
{
	Q_OBJECT

public:
	CommandLine(int size, QMap<QString, Macros*> *macroses);
	void add(QChar &currentChar);

public slots:
	void catchChar(QChar key);
	void catchMouseEvent(QString type);

private:
	void scan();
	void scanShortcut();
	QMap<QString, Macros*> *mMacros;
	QList<QChar> mList;
	QSet<QString> *drawKeys;
	int mSize;
};
