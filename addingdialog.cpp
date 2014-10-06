#include "addingdialog.h"
#include "ui_addingdialog.h"

AddingDialog::AddingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddingDialog)
{
	ui->setupUi(this);
	connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(wasGood()));
	connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(close()));
}

AddingDialog::~AddingDialog()
{
	delete ui;
}

void AddingDialog::wasGood()
{
	QString str = ui->macrosLine->text().toUpper();
	str += "#" + ui->pathLine->text();
	emit throwMacros(str);
	close();
}
