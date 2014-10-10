#pragma once
#include <QDialog>
#include <QPair>

namespace Ui {
class AddingDialog;
}

class AddingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddingDialog(QWidget *parent = 0);
	~AddingDialog();
	QString &getName();
	QString &getPath();

signals:
	void wasUpdated();

public slots:
	void OkWasClicked();

private:
	Ui::AddingDialog *ui;
	QString mName;
	QString mPath;
};
