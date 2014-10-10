#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	mAd = new AddingDialog();
	mKeyFilter = new KeyPressFilter();
	installEventFilter(mKeyFilter);
	mController = new Controller();
	mController->setConnection(mKeyFilter);
	mSender = new Sender(mAd, mController);
	connect(mAd, SIGNAL(wasUpdated()), mSender, SLOT(newMacrosWasCreated()));
	connect(ui->checkBoxNo, SIGNAL(clicked()), this, SLOT(noWasClicked()));
	connect(ui->checkBoxYes, SIGNAL(clicked()), this, SLOT(yesWasClicked()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addWasClicked()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete mKeyFilter;
	delete mController;
}

void MainWindow::yesWasClicked()
{
	installEventFilter(mKeyFilter);
	ui->checkBoxYes->setEnabled(false);
	ui->checkBoxNo->setEnabled(true);
	ui->checkBoxNo->setChecked(false);
}

void MainWindow::noWasClicked()
{
	removeEventFilter(mKeyFilter);
	ui->checkBoxNo->setEnabled(false);
	ui->checkBoxYes->setEnabled(true);
	ui->checkBoxYes->setChecked(false);
}

void MainWindow::addWasClicked()
{
	mAd->show();
}
