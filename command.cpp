#include "command.h"
#include <QDebug>

const QString Command::commandTypes[Command::commandTypesNumber] = {
	"Open webpage",
	"Open file",
	"System command"
};

Command::Command(const QString &execPath)
{
	path = execPath;
}

Command::~Command()
{
}

Command *Command::createCommand(const QString &path, const QString &type)
{
	Command *newCommand;
	if (type == "Open webpage")
		newCommand = new WebpageCommand(path);
	if (type == "Open file")
		newCommand = new FileCommand(path);
	if (type == "System command")
		newCommand = new SystemCommand(path);
	return newCommand;
}

const QString &Command::getPath()
{
	return path;
}

void PreCommand::exec() {}


const QString &PreCommand::getType()
{
	return type;
}

PreCommand::PreCommand(const QString &execPath, const QString &execType)
	:type(execType)
{
	path = execPath;
}

void PreCommand::updateCommandPath(const QString &newPath)
{
	path = newPath;
}

void PreCommand::updateCommandType(const QString &newType)
{
	type = newType;
}

void WebpageCommand::exec()
{
	QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

const QString & WebpageCommand::getType()
{
	return type;
}

void FileCommand::exec()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

const QString & FileCommand::getType()
{
	return type;
}

void SystemCommand::exec()
{
	QByteArray byteArray = path.toUtf8();
	system(byteArray.constData());
}

const QString & SystemCommand::getType()
{
	return type;
}
