#pragma once
#include <QString>
#include <QDesktopServices>
#include <QUrl>

class Command
{
	public:
		Command(const QString &execPath);
		Command() {}
		virtual void exec() = 0;
	protected:
		QString path;
};

class WebpageCommand : public Command {
public:
	WebpageCommand(const QString &execPath) : Command(execPath){}
	void exec();
};

class FileCommand: public Command {
public:
	FileCommand(const QString &execPath) : Command(execPath){}
	void exec();
};

class SystemCommand: public Command {
public:
	SystemCommand(const QString &execPath) : Command(execPath){}
	void exec();
};
