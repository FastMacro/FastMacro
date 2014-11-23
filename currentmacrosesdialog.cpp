#include "currentmacrosesdialog.h"
#include "ui_currentmacrosesdialog.h"
#include "addingdialog.h"

CurrentMacrosesDialog::CurrentMacrosesDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CurrentMacrosesDialog)
{
	ui->setupUi(this);
	ui->mainLayout->setSpacing(50);

	mView = new QLabel("You have no macroses");
	ui->mainLayout->addWidget(mView);

	macrosLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(macrosLayout);
	macrosLayout->setSpacing(20);
}

CurrentMacrosesDialog::~CurrentMacrosesDialog()
{
	MacrosDestructor::clearLayout(ui->mainLayout);
	delete ui;
}

void CurrentMacrosesDialog::closeEvent(QCloseEvent *event)
{
	MacrosDestructor::clearLayout(macrosLayout);
	event->accept();
}

void CurrentMacrosesDialog::showMacroses(QMap<QString, Macros> *mMacroses)
{
	MacrosDestructor::clearLayout(macrosLayout);
	mNumberOfMacroses = mMacroses->count();
	mView->setText("You have " + QString::number(mNumberOfMacroses) + " macroses");

	for (QMap<QString, Macros>::iterator it = mMacroses->begin(); it != mMacroses->end(); ++it) {

		QGridLayout *layout = new QGridLayout;
		layout->setSpacing(5);

		QLineEdit *editLine = new QLineEdit;
		editLine->setText(it.key());
		editLine->setReadOnly(true);
		layout->addWidget(editLine, 0, 0, 1, 3);

		QPushButton *editButton = new QPushButton;
		editButton->setText("Edit");
		layout->addWidget(editButton, 0, 3, 1, 1);

		QPushButton *deleteButton = new QPushButton;
		deleteButton->setText("Delete");
		layout->addWidget(deleteButton, 0, 4, 1, 1);

		MacrosDestructor *mDestructor = new MacrosDestructor(editLine->text());
		mDestructor->mLayout = layout;
		MacrosUpdater *mUpdater = new MacrosUpdater(it.key(), this);
		connect(deleteButton, SIGNAL(clicked()), mDestructor, SLOT(deleteMacros()));
		connect(editButton, SIGNAL(clicked()), mUpdater, SLOT(updateMacros()));
		connect(mDestructor, SIGNAL(wasUpdated(QString)), this, SLOT(recountNumber()));
		macrosLayout->addLayout(layout);
	}
	show();
}

void CurrentMacrosesDialog::emitMacrosEdit(const QString &name)
{
	emit editMacros(name);
}

void CurrentMacrosesDialog::recountNumber()
{
	mNumberOfMacroses--;
	QString text;
	if (mNumberOfMacroses == 0)
		text = "You have no macroses";
	else if (mNumberOfMacroses == 1)
		text = "You have one macros";
	else
		text = "You have " + QString::number(mNumberOfMacroses) + " macroses";
	mView->setText(text);
}

void MacrosDestructor::clearLayout(QLayout *layout)
{
	int widgetsCount = layout->count();
	for (int i = 0; i < widgetsCount; i++) {
		QLayoutItem *item = layout->itemAt(0);
		QLayout *cast = dynamic_cast<QLayout*>(item);
		if (cast) {
			clearLayout(cast);
			delete item;
			continue;
		}
		delete item->widget();
	}
}

void MacrosDestructor::deleteMacros()
{
	clearLayout(mLayout);
	delete mLayout;
	emit wasUpdated(name);
}

void MacrosUpdater::updateMacros()
{
	dialog->emitMacrosEdit(name);
}
