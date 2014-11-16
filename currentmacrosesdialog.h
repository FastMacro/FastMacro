#pragma once
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPair>
#include <QCloseEvent>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMap>
#include "macros.h"
#include "command.h"

namespace Ui {
class CurrentMacrosesDialog;
}

class CurrentMacrosesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CurrentMacrosesDialog(QWidget *parent = 0);
	~CurrentMacrosesDialog();
	void showMacroses(QMap<QString, Macros> *macroses);

signals:
	void wasUpdated(QString);

public slots:
	void recountNumber();

private:
	void closeEvent(QCloseEvent* event);
	QLabel *mView;
	QVBoxLayout *macrosLayout;
	int mNumberOfMacroses = 0;
	Ui::CurrentMacrosesDialog *ui;
};

class MacrosDestructor : public QObject {
	Q_OBJECT
	public:
		MacrosDestructor(const QString& name) : name(name) {}
		static void clearLayout(QLayout *layout);
		QString name;
		QLayout *mLayout;

	signals:
		void wasUpdated(QString);

	public slots:
		void deleteMacros();
};
