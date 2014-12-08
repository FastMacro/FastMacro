#pragma once
#include <QDialog>
#include <QPair>
#include <QMap>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QList>
#include <macros.h>
#include "command.h"
#include "macrosoutputholder.h"

namespace Ui {
class AddingDialog;
}

class AddingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddingDialog(QWidget *parent = 0);
	~AddingDialog();
	void addMacros();
	void editMacros(Macros *macros);
	MacrosOutputHolder *holder;

signals:
	void wasUpdated();
	void deleteMacros(const QString &name);
	void refreshCurrentMacroses();

private slots:
	void OkClicked();
	void CancelClicked();
	void keyStringChanged(const QString &key);
	void modeChanged(const QString &mode);
	void addCommandWidget();

private:
	void initialize();
	QMap<QString, Macros> *load();
	void initializeExecutionModes();
	void createCommandWidget(const QString &oldPath, const QString &oldType);
	QString editingMacrosName;
	Ui::AddingDialog *ui;
	QLineEdit *macrosName;
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
