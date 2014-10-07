#pragma once
#include <QString>
#include <QDesktopServices>
#include <QUrl>

class Command
{
    public:
        Command(const QString &execPath);
        virtual void exec() = 0;
    protected:
        QString path;
};

class WebpageCommand : public Command {
    void exec();
};

class FileCommand: public Command {
    void exec();
};

class SystemCommand: public Command {
    void exec();
};
