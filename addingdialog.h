#pragma once
#include <QDialog>
#include <QPair>
#include <QMap>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QList>
#include "command.h"

namespace Ui {
class AddingDialog;
}

class AddingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddingDialog(QWidget *parent = 0);
	~AddingDialog();
	Command **outputCommandList = nullptr;
	QString outputKey;
	int outputSize;

signals:
	void wasUpdated();

private slots:
	void OkClicked();
	void CancelClicked();
	void keyStringChanged(const QString *key);
	void modeChanged(const QString &mode);
	void createCommandWidget();

private:
	void initializeWindow();
	void initializeExecutionModes();
	Ui::AddingDialog *ui;
	QVBoxLayout *inputLayout;
	QVBoxLayout *macrosLayout;
	QLineEdit *keyString;
	QComboBox *selectExecutionMode;
	QDialogButtonBox *buttonBox;
	QMap<QString, QString> *executionModes;
	QList<PreCommand*> *commandList;
};

class CommandDestructor : public QObject {
	Q_OBJECT
	public:
		PreCommand *command;
		QLayout *commandLayout;
		QList<PreCommand*> *commandList;
	public slots:
		void deleteCommand();
};
