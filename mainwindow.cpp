#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtXml/QDomDocument>

void load();

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this); 
	setFixedSize(190, 100);
	mAd = new AddingDialog();
	mCm = new CurrentMacrosesDialog();
	mKeyFilter = KeyPressFilter::getInstance();
	mController = new Controller();
	mController->setConnection(mKeyFilter);
	mSender = new Sender(mAd, mController, mCm);
	startTrayIcon();
	setConnections();
}

MainWindow::~MainWindow()
{
	delete ui;
	delete mKeyFilter;
	delete mController;
	delete minimizeAction;
	delete quitAction;
	delete trayIcon;
	delete trayIconMenu;
	delete mSender;
	delete mAd;
	delete mCm;
}

void MainWindow::startTrayIcon()
{
	minimizeAction = new QAction(tr("Show"), this);
	connect(minimizeAction, SIGNAL(triggered()), this, SLOT(showNormal()));

	changeStateAction = new QAction(tr("Active"), this);
	connect(changeStateAction, SIGNAL(triggered()), this, SLOT(trayCheck()));
	changeStateAction->setCheckable(true);
	changeStateAction->setChecked(true);

	quitAction = new QAction(tr("&Quit"), this);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

	createTrayIcon();

	connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
				 this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
	trayIcon->show();
}

void MainWindow::setConnections()
{
	connect(mAd, SIGNAL(wasUpdated()), mSender, SLOT(newMacrosWasCreated()));
	connect(mAd, SIGNAL(deleteMacros(QString)), mSender, SLOT(deleteMacros(QString)));
	connect(mAd, SIGNAL(refreshCurrentMacroses()), mSender, SLOT(needMacroses()));
	connect(mCm, SIGNAL(deleteMacros(QString)), mSender, SLOT(deleteMacros(QString)));
	connect(mCm, SIGNAL(editMacros(QString)), mSender, SLOT(editMacros(QString)));
	connect(ui->checkBoxState, SIGNAL(clicked()), this, SLOT(activeWasClicked()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addWasClicked()));
	connect(ui->currentButton, SIGNAL(clicked()), mSender, SLOT(needMacroses()));
}

void MainWindow::activeWasClicked()
{
	mController->setEnabled(mKeyFilter, ui->checkBoxState->isChecked());
}

void MainWindow::trayCheck()
{
	ui->checkBoxState->setChecked(!ui->checkBoxState->isChecked());
	activeWasClicked();
}

void MainWindow::addWasClicked()
{
	mAd->addMacros();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (trayIcon->isVisible()) {
		hide();
		event->ignore();
	}
}

void MainWindow::setIcon()
{
	QIcon icon(":/images/fastMacroIcon2.svg");
	setWindowIcon(icon);
	trayIcon->setIcon(icon);
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason) {
	case QSystemTrayIcon::Trigger:
	case QSystemTrayIcon::DoubleClick:
		setVisible(true);
		break;
	default:
	 ;
	}
}

void MainWindow::createTrayIcon()
{
	trayIconMenu = new QMenu(this);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(changeStateAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	trayIcon = new QSystemTrayIcon(this);
	trayIcon->setContextMenu(trayIconMenu);
	setIcon();
}
