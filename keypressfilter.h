#pragma once
#include <QEvent>
#include <QKeyEvent>
#include <QtGui>
#include <QChar>

#include <QDebug>

class KeyPressFilter : public QObject
{
	Q_OBJECT

public:
	KeyPressFilter() {}
	~KeyPressFilter() {}

signals:
	void throwChar(QChar key);

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		if (event->type() == QEvent::KeyPress) {
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

				qDebug() << keyEvent->nativeVirtualKey();
			emit throwChar(keyEvent->key());
			return true;
		} else {
			// standard event processing
			return QObject::eventFilter(obj, event);
		}
	}

};
