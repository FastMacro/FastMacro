#include "addingdialog.h"
#include "ui_addingdialog.h"
#include "keypressfilter.h"
#include "keysetconverter.h"
#include <QLineEdit>
#include <QString>

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
	QIcon icon(":/images/fastMacroIcon2.svg");
	setWindowIcon(icon);
}

AddingDialog::~AddingDialog()
{
	delete ui;
}

void AddingDialog::initialize()
{
	commandList->clear();

	gestureController = nullptr;
	editingGesture = "";

	clearLayout(ui->mainLayout);
	setFixedWidth(mWidth);
	ui->mainLayout->setSpacing(20);
	QVBoxLayout *nameLayout =  new QVBoxLayout;
	ui->mainLayout->addLayout(nameLayout, 1);
	nameLayout->setSpacing(5);

	nameLayout->addWidget(new QLabel("Type macros name:"));
	macrosName = new QLineEdit;
	nameLayout->addWidget(macrosName);

	QVBoxLayout *modeLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(modeLayout, 1);
	modeLayout->setSpacing(5);

	modeLayout->addWidget(new QLabel("Select macros execution type:"));
	selectExecutionMode = new QComboBox();
	modeLayout->addWidget(selectExecutionMode);

	inputLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(inputLayout);
	inputLayout->setSpacing(5);

	macrosLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(macrosLayout);
	macrosLayout->addWidget(new QLabel("Commands:"));
	macrosLayout->setSpacing(5);

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
	initializeExecutionModes();
}

void AddingDialog::addMacros()
{
	initialize();
	editingMacrosName = "";
	show();
}

void AddingDialog::editMacros(Macros *macros) {
	initialize();
	if (macros->getType() == "mouse")
		editingGesture = macros->getKeystring();
	selectExecutionMode->setCurrentText(executionModes->value(macros->getType()));
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	if (macros->getType() == "keystring") {
		keyString->setText(macros->getKeystring());
	}
	if (macros->getType() == "shortcut") {
		QString text = "Currently selected keys:";
		shortcutKeys = KeySetConverter::getInstance()->toSet(macros->getKeystring());
		foreach (const QString &value, *shortcutKeys)
			text += " " + value + " ";
		shortcutLabel->setText(text);
	}
	macrosName->setText(macros->getName());
	editingMacrosName = macros->getName();
	Command **commands = macros->getCommandList().first;
	for (int i = 0; i < macros->getCommandList().second; i++)
		createCommandWidget(commands[i]->getType(), commands[i]->getPath());
	show();
}

void AddingDialog::initializeExecutionModes()
{
	executionModes = new QMap<QString, QString>();
	executionModes->insert("keystring", "Type key string");
	executionModes->insert("shortcut", "Use shortcut");
	executionModes->insert("mouse", "Use mouse gestures");
	foreach (const QString &value, *executionModes)
		selectExecutionMode->addItem(value);
	selectExecutionMode->setCurrentText(executionModes->value("keystring"));
	modeChanged(executionModes->value("keystring"));
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
	int outputSize = commandList->count();
	Command **outputCommandList = new Command*[outputSize];
	for (int i = 0; i < outputSize; i++) {
		PreCommand *command = commandList->at(i);
		outputCommandList[i] = Command::createCommand(command->getPath(), command->getType());
	}
	QString name = macrosName->text();

	if (selectExecutionMode->currentText() == executionModes->value("keystring"))
		holder = new MacrosOutputHolder(name, "keystring", outputCommandList, outputSize, keyString->text(), nullptr);
	else if (selectExecutionMode->currentText() == executionModes->value("shortcut")) {
		holder = new MacrosOutputHolder(name, "shortcut",  outputCommandList, outputSize, "", shortcutKeys);
	} else if (selectExecutionMode->currentText() == executionModes->value("mouse"))
		holder = new MacrosOutputHolder(name, "mouse",  outputCommandList, outputSize, "$" + gestureController->getGesture(), nullptr);
	shortcutKeys = nullptr;

	if (editingMacrosName != "")
		emit deleteMacros(editingMacrosName);
	emit wasUpdated();
	if (editingMacrosName != "")
		emit refreshCurrentMacroses();
	delete holder;
	clearLayout(macrosLayout);
	close();
}

void AddingDialog::keyStringChanged(const QString &key)
{
	if (key.toStdString() == "") {
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}
	QString modifiedString = key;
	std::string normalString = key.toStdString();
	for (int i = 0; i < key.length(); i++) {
		char q = normalString[i];
		if (!(q >= 'a' && q <= 'z') && !(q >= 'A' && q <= 'Z') && !(q >= '0' && q <= '9')) {
			modifiedString = key.left(i) + key.right(key.length() - i - 1);
			break;
		}
	}
	keyString->setText(modifiedString);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void AddingDialog::modeChanged(const QString &mode)
{
	clearLayout(inputLayout);

	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

	if (executionModes->value("keystring") == mode) {
		QLabel *inputLabel = new QLabel("Enter key string: (only letters and numbers are allowed)");
		inputLayout->addWidget(inputLabel);
		keyString = new QLineEdit;
		inputLayout->addWidget(keyString);
		connect(keyString, SIGNAL(textChanged(QString)), this, SLOT(keyStringChanged(QString)));
		return;
	}

	if (executionModes->value("shortcut") == mode) {
		QHBoxLayout *upperLayout = new QHBoxLayout;
		QLabel *inputLabel = new QLabel("Press keys you want to use in shortcut:");
		QPushButton *recordButton = new QPushButton("OK");
		upperLayout->addWidget(inputLabel);
		upperLayout->addWidget(recordButton);
		shortcutLabel = new QLabel("Currently selected keys:");
		inputLayout->addLayout(upperLayout);
		inputLayout->addWidget(shortcutLabel);
		connect(recordButton, SIGNAL(clicked()), this, SLOT(recordShortcut()));
		return;
	}

	if (executionModes->value("mouse") == mode) {
		//inputLayout->addWidget(new QLabel("WARNING: now we are recognizing only a horizontal line,\na vertical line and a left lower corner as a gesture"));
		inputLayout->addWidget(new QLabel("For drawing gestures, hold Ctrl+F1 \nand draw it holding left mouse button"));
		initializeMouseGestures();
	}
}

void AddingDialog::initializeMouseGestures()
{
	QHBoxLayout *gesturesLayout = new QHBoxLayout;

	for (int i = 0; i < gesturesNumber; i++) {
		QPushButton *button = new QPushButton;
		QPixmap pixmap(":/images/" + gestureNames[i] + ".svg");
		QIcon ButtonIcon(pixmap);
		button->setIcon(ButtonIcon);
		gesturesLayout->addWidget(button);
		GestureController *controller = new GestureController(this, button, gestureNames[i]);
		connect(button, SIGNAL(clicked()), controller, SLOT(setGesture()));
		if (editingGesture == "$" + gestureNames[i].toUpper())
			setGesture(controller);
	}
	inputLayout->addLayout(gesturesLayout);
}

void AddingDialog::recordShortcut()
{
	QSet<QString> *pressedKeys = KeyPressFilter::getInstance()->getPressedKeys();
	if (pressedKeys->count() == 0)
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	else
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	QString text = "Currently selected keys:";
	foreach (const QString &value, *pressedKeys)
		text += " " + value + " ";
	shortcutLabel->setText(text);
	if (shortcutKeys)
		delete shortcutKeys;
	shortcutKeys = new QSet<QString>(*pressedKeys);
}

void AddingDialog::createCommandWidget(const QString &oldType, const QString &oldPath)
{
	QGridLayout *layout = new QGridLayout;

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

void AddingDialog::setGesture(GestureController *controller) {
	if (gestureController)
		gestureController->getButton()->setStyleSheet("background:");
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	gestureController = controller;
	controller->getButton()->setStyleSheet("background: #C7CDFF");
}

void CommandDestructor::deleteCommand()
{
	commandList->removeAll(command);
	delete command;
	clearLayout(commandLayout);
	delete commandLayout;
}
