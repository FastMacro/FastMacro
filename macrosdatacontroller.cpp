#include "macrosdatacontroller.h"

MacrosDataController::MacrosDataController()
{
}

MacrosDataController::~MacrosDataController()
{
}

void MacrosDataController::save(QMap<QString, Macros*> *macroses)
{
	QFile file("db.xml");
	file.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&file);

	QMap<QString, Macros*>::iterator i;

	out << "<FastMacroDatabase>" << endl;

	for (i = macroses->begin(); i != macroses->end(); ++i)
	{
		out << "<macros>"<< endl;

		out << "	<name>" << i.value()->getName() << "</name>" << endl;

		QString type = i.value()->getType();
		out << "	<type>" << type << "</type>" << endl;

		if (type == "keystring")
			out << "	<keystring>" << dynamic_cast<KeyStringMacros*>(i.value())->getKeyString() << "</keystring>" << endl;
		if (type == "shortcut") {
			out << "	<keys>" << endl;
			foreach (const QString &value, *dynamic_cast<ShortcutMacros*>(i.value())->getKeys())
				out << "		<key>" << value << "</key>" << endl;
		}

		int size = i.value()->getCommandList().second;

		for (int j = 0; j < size; j++)
		{
			out << "	<command>" << endl;
			out << "		<type>" << i.value()->getCommandList().first[j]->getType() << "</type>" << endl;
			out << "		<path>" << i.value()->getCommandList().first[j]->getPath() << "</path>" << endl;
			out << "	</command>" << endl;
		}
		out << "</macros>" << endl;
	}
	out << "</FastMacroDatabase>" << endl;

	file.close();
}

Command* MacrosDataController::parseCommand(QDomNode docElem)
{
	if(docElem.isNull())
		return nullptr;

	QDomNode node = docElem.firstChild();
	QString type = "";
	QString path = "";

	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if(element.tagName() == "type")
			type = element.text();
		if(element.tagName() == "path")
			path = element.text();
		node = node.nextSibling();
	}

	return Command::createCommand(path, type);
}

QSet<QString>* MacrosDataController::parseKeys(QDomNode docElem) {
	if(docElem.isNull())
		return nullptr;

	QDomNode node = docElem.firstChild();
	QSet<QString> *set = new QSet<QString>;

	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if(element.tagName() == "key")
			set->insert(element.text());
		node = node.nextSibling();
	}

	return set;
}

Macros* MacrosDataController::parseMacros(QDomNode docElem)
{
	if(docElem.isNull())
		return nullptr;
	QDomNode node = docElem.firstChild();

	QString name = "";
	QString type = "";
	QString keyString = "";
	QSet<QString> *keys = nullptr;
	Command** commandList = new Command*[docElem.childNodes().size() - 1];
	int i = 0;

	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if (element.tagName() == "name")
			name = element.text();
		if (element.tagName() == "keystring")
			keyString = element.text();
		if (element.tagName() == "keys")
			keys = parseKeys(element);
		if (element.tagName() == "type")
			type = element.text();
		if (element.tagName() == "command")
		{
			commandList[i] = parseCommand(element);
			i++;
		}
		node = node.nextSibling();
	}

	return Macros::createMacros(new MacrosOutputHolder(name, type, commandList, i, keyString, keys));
}

QMap<QString, Macros*> *MacrosDataController::load()
{
	QMap<QString, Macros*> *loadedMacroses = new QMap<QString, Macros*>;

	QDomDocument doc("mydocument");
	QFile file("db.xml");
	file.open(QIODevice::ReadOnly);
	doc.setContent(&file);
	file.close();

	QDomElement docElem = doc.documentElement();

	QDomNode node = docElem.firstChild();

	while(!node.isNull())
	{
		Macros *newMacros = parseMacros(node);
		if (newMacros)
			loadedMacroses->insert(newMacros->getName(), newMacros);
		node = node.nextSibling();
	}
	return loadedMacroses;
}
