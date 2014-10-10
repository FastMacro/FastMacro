#include "command.h"
#include <QDebug>

Command::Command(const QString &execPath)
{
	path = execPath;
}

void WebpageCommand::exec()
{
	QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void FileCommand::exec()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(path));
}

void SystemCommand::exec()
{
	QByteArray byteArray = path.toUtf8();
	system(byteArray.constData());
}
