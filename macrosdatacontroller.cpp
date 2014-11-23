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

		out << "	<name>" << i.key() << "</name>" << endl;

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

Macros* MacrosDataController::parseMacros(QDomNode docElem)
{
	if(docElem.isNull())
		return nullptr;
	QDomNode node = docElem.firstChild();

	QString name = "";
	Command** commandList = new Command*[docElem.childNodes().size() - 1];
	int i = 0;

	while(!node.isNull())
	{
		QDomElement element = node.toElement();
		if(element.tagName() == "name")
			name = element.text();
		if(element.tagName() == "command")
		{
			commandList[i] = parseCommand(element);
			i++;
		}
		node = node.nextSibling();
	}

	return new Macros(name, commandList, i);
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
