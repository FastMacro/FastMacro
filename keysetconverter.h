#pragma once
#include <QSet>

class KeySetConverter {
public:
	static KeySetConverter *getInstance() {
		if (!instance)
			instance = new KeySetConverter;
		return instance;
	}
	QSet<QString> *toSet(const QString &keystring);
	QString toString(QSet<QString> *keySet);
private:
	KeySetConverter() {}
	static KeySetConverter *instance;
};


