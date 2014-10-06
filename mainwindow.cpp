#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	keyFilter = new KeyPressFilter();
	installEventFilter(keyFilter);
	connect(ui->checkBoxNo, SIGNAL(clicked()), this, SLOT(noWasClicked()));
	connect(ui->checkBoxYes, SIGNAL(clicked()), this, SLOT(yesWasClicked()));
	QList<QString> commands;
	commands.push_back("VK");
	commands.push_back("CMD");
	commandLine = new CommandLine(10, commands);
	commandExplorer = new CommandExplorer();
	connect(commandLine, SIGNAL(throwCommand(QString)),
			commandExplorer, SLOT(catchCommand(QString)));
	connect(keyFilter, SIGNAL(throwChar(QChar)), commandLine, SLOT(catchChar(QChar)));
	connect(ui->addButton, SIGNAL(clicked()), this, SLOT(addWasClicked()));
	macrosFactory = new MacrosFactory();
	connect(&ad, SIGNAL(throwMacros(QString)), macrosFactory, SLOT(catchMacros(QString)));
	connect(macrosFactory, SIGNAL(wasUpdated()), this, SLOT(updateMakers()));
}

MainWindow::~MainWindow()
{
	delete ui;
	delete keyFilter;
	delete commandLine;
	delete commandExplorer;
	delete macrosFactory;
}

void MainWindow::updateMakers()
{
	commandExplorer->updateMap(macrosFactory);
	commandLine->updateMap(macrosFactory);
}

void MainWindow::yesWasClicked()
{
	installEventFilter(keyFilter);
	ui->checkBoxYes->setEnabled(false);
	ui->checkBoxNo->setEnabled(true);
	ui->checkBoxNo->setChecked(false);
}

void MainWindow::noWasClicked()
{
	removeEventFilter(keyFilter);
	ui->checkBoxNo->setEnabled(false);
	ui->checkBoxYes->setEnabled(true);
	ui->checkBoxYes->setChecked(false);
}

void MainWindow::addWasClicked()
{
	ad.show();
}
