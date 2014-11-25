#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
    minimizeAction = new QAction(tr("Show"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    createTrayIcon();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                 this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();

    mAd = new AddingDialog();
	mCm = new CurrentMacrosesDialog();
	mKeyFilter = KeyPressFilter::getInstance();
	installEventFilter(mKeyFilter);
	mController = new Controller();
	mController->setConnection(mKeyFilter);
	mSender = new Sender(mAd, mController, mCm);
	connect(mAd, SIGNAL(wasUpdated()), mSender, SLOT(newMacrosWasCreated()));
	connect(mAd, SIGNAL(deleteMacros(QString)), mSender, SLOT(deleteMacros(QString)));
	connect(mAd, SIGNAL(refreshCurrentMacroses()), mSender, SLOT(needMacroses()));
	connect(mCm, SIGNAL(wasUpdated(QString)), mSender, SLOT(deleteMacros(QString)));
	connect(mCm, SIGNAL(editMacros(QString)), mSender, SLOT(editMacros(QString)));
	connect(ui->checkBoxNo, SIGNAL(clicked()), this, SLOT(noWasClicked()));
	connect(ui->checkBoxYes, SIGNAL(clicked()), this, SLOT(yesWasClicked()));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addWasClicked()));
	connect(ui->currentButton, SIGNAL(clicked()), mSender, SLOT(needMacroses()));
//	connect(ui->currentButton, SIGNAL(clicked()), this,SLOT(currentWasClicked()));

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
	mAd->addMacros();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        QMessageBox::information(this, tr("Systray"),
                              tr("The program will keep running in the "
                                 "system tray. To terminate the program, "
                                 "choose <b>Quit</b> in the context menu "
                                 "of the system tray entry."));
        hide();
        event->ignore();
    }
}

void MainWindow::setIcon()
 {
     QIcon icon(":/images/trash.svg");
     trayIcon->setIcon(icon);
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
     trayIconMenu->addSeparator();
     trayIconMenu->addAction(quitAction);
     trayIcon = new QSystemTrayIcon(this);
     trayIcon->setContextMenu(trayIconMenu);
     setIcon();
 }

void MainWindow::currentWasClicked()
{
	mCm->show();
}
