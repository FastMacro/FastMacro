#include "addingdialog.h"
#include "ui_addingdialog.h"

AddingDialog::AddingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddingDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(OkWasClicked()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

AddingDialog::~AddingDialog()
{
	delete ui;
}

void AddingDialog::OkWasClicked()
{
	mName = ui->macrosLine->text().toUpper();
	mPath = ui->pathLine->text();
	emit wasUpdated();
	close();
}

QString &AddingDialog::getName()
{
	return mName;
}

QString &AddingDialog::getPath()
{
	return mPath;
}
