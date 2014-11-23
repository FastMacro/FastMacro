#include "addingdialog.h"
#include "ui_addingdialog.h"
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

void clearLayout(QLayout *layout)
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

AddingDialog::AddingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddingDialog)
{
	ui->setupUi(this);
	commandList = new QList<PreCommand*>;
}

AddingDialog::~AddingDialog()
{
	delete ui;
}

void AddingDialog::initialize()
{
	commandList->clear();

	clearLayout(ui->mainLayout);

	ui->mainLayout->setSpacing(50);
	ui->mainLayout->addWidget(new QLabel("Macros Constructor"));

	QVBoxLayout *modeLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(modeLayout, 1);
	modeLayout->setSpacing(20);

	modeLayout->addWidget(new QLabel("Select macros execution type:"));
	selectExecutionMode = new QComboBox();
	modeLayout->addWidget(selectExecutionMode);
	initializeExecutionModes();

	inputLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(inputLayout);
	inputLayout->setSpacing(20);

	macrosLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(macrosLayout);
	macrosLayout->setSpacing(20);

	QPushButton *addCommandButton = new QPushButton;
	addCommandButton->setText("Add command");
	ui->mainLayout->addWidget(addCommandButton);
	connect(addCommandButton, SIGNAL(clicked()), this, SLOT(addCommandWidget()));

	buttonBox = new QDialogButtonBox;
	buttonBox->addButton(QDialogButtonBox::Ok);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	buttonBox->addButton(QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(OkClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(CancelClicked()));
	ui->mainLayout->addWidget(buttonBox);
}

void AddingDialog::addMacros()
{
	initialize();
	editingMacrosName = "";
	show();
}

void AddingDialog::editMacros(Macros macros) {
	initialize();
	selectExecutionMode->setCurrentText("Type key string");
	modeChanged("Type key string");
	keyString->setText(macros.getName());
	keyStringChanged(macros.getName());
	editingMacrosName = macros.getName();
	Command **commands = macros.getCommandList().first;
	for (int i = 0; i < macros.getCommandList().second; i++)
		createCommandWidget(commands[i]->getType(), commands[i]->getPath());
	show();
}

void AddingDialog::initializeExecutionModes()
{
	executionModes = new QMap<QString, QString>();
	executionModes->insert("none", "Not selected");
	executionModes->insert("type", "Type key string");
	for (int i = 0; i < executionModes->size(); i++)
		selectExecutionMode->addItem(executionModes->values().at(i));
	connect(selectExecutionMode, SIGNAL(currentTextChanged(QString)), this, SLOT(modeChanged(QString)));
}

void AddingDialog::addCommandWidget() {
	createCommandWidget("", "");
}

void AddingDialog::CancelClicked()
{
	clearLayout(macrosLayout);
	close();
}

void AddingDialog::OkClicked()
{
	outputSize = commandList->count();
	outputCommandList = new Command*[outputSize];
	for (int i = 0; i < outputSize; i++) {
		PreCommand *command = commandList->at(i);
		outputCommandList[i] = Command::createCommand(command->getPath(), command->getType());
	}
	outputKey = keyString->text();
	if (editingMacrosName != "")
		emit deleteMacros(editingMacrosName);
	emit wasUpdated();
	if (editingMacrosName != "")
		emit refreshCurrentMacroses();
	clearLayout(macrosLayout);
	close();
}

void AddingDialog::keyStringChanged(const QString &key)
{
	if (key.toStdString() == "") {
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	if (executionModes->value("none") == selectExecutionMode->currentText()) {
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddingDialog::modeChanged(const QString &mode)
{
	clearLayout(inputLayout);
	if (executionModes->value("none") == mode) {
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);

	QLabel *inputLabel = new QLabel("Enter key string:");
	inputLayout->addWidget(inputLabel);

	keyString = new QLineEdit;
	inputLayout->addWidget(keyString);
}

void AddingDialog::createCommandWidget(const QString &oldType, const QString &oldPath)
{
	QGridLayout *layout = new QGridLayout;
	layout->setSpacing(5);

	QComboBox *box = new QComboBox;
	for (int i = 0; i < Command::commandTypesNumber; i++)
		box->addItem(Command::commandTypes[i]);
	if (oldType != "")
		box->setCurrentText(oldType);
	layout->addWidget(box, 0, 0, 1, 4);

	PreCommand *newCommand = new PreCommand(oldPath, box->currentText());
	commandList->append(newCommand);

	QPushButton *deleteButton = new QPushButton;
	deleteButton->setText("X");
	layout->addWidget(deleteButton, 0, 4, 1, 1);
	CommandDestructor *destructor = new CommandDestructor;
	destructor->command = newCommand;
	destructor->commandLayout = layout;
	destructor->commandList = commandList;

	QLineEdit *path = new QLineEdit;
	path->setText(oldPath);
	layout->addWidget(path,	1, 0, 1, 5);

	connect(path, SIGNAL(textChanged(QString)), newCommand, SLOT(updateCommandPath(QString)));
	connect(box, SIGNAL(currentTextChanged(QString)), newCommand, SLOT(updateCommandType(QString)));
	connect(deleteButton, SIGNAL(clicked()), destructor, SLOT(deleteCommand()));
	connect(deleteButton, SIGNAL(clicked()), destructor, SLOT(deleteLater()));

	macrosLayout->addLayout(layout);
}

void CommandDestructor::deleteCommand()
{
	commandList->removeAll(command);
	delete command;
	clearLayout(commandLayout);
	delete commandLayout;
}
