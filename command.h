#pragma once
#include <QObject>
#include <QString>
#include <QDesktopServices>
#include <QUrl>

class Command : public QObject
{
	Q_OBJECT
public:
	Command() {}
	Command(const QString &execPath);
	~Command();
	virtual void exec() = 0;
	virtual const QString &getType() = 0;
	static const int commandTypesNumber = 3;
	static const QString commandTypes[];
	const QString &getPath();
	static Command *createCommand(const QString &path, const QString &type);
protected:
	QString path;
};

class PreCommand : public Command
{
	Q_OBJECT
public:
	PreCommand(const QString &execPath, const QString &execType);
	const QString &getType();
	void exec();
public slots:
	void updateCommandType(const QString &newType);
	void updateCommandPath(const QString &newPath);
private:
	QString type;
};

class WebpageCommand : public Command {
public:
	WebpageCommand(const QString &execPath) : Command(execPath){}
	const QString &getType();
	void exec();
	QString type = commandTypes[0];
};

class FileCommand: public Command {
public:
	FileCommand(const QString &execPath) : Command(execPath){}
	const QString &getType();
	void exec();
	QString type = commandTypes[1];
};

class SystemCommand: public Command {
public:
	SystemCommand(const QString &execPath) : Command(execPath){}
	const QString &getType();
	void exec();
	QString type = commandTypes[2];
};
