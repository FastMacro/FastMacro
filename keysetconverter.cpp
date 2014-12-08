#include "keysetconverter.h"
#include <QString>
#include <QStringList>

KeySetConverter *KeySetConverter::instance = 0;

QSet<QString>* KeySetConverter::toSet(const QString &keystring) {
	if (keystring[0] != '#')
		return nullptr;
	return new QSet<QString>(keystring.mid(1, keystring.length() - 1).split("#").toSet());
}

QString KeySetConverter::toString(QSet<QString> *keySet) {
	QString output = "";
	foreach (const QString &value, *keySet) {
		output += "#" + value;
	}
	return output;
}

