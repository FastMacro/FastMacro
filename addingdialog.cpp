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
	if (!outputCommandList)
		delete outputCommandList;
	outputCommandList = nullptr;
	commandList = new QList<PreCommand*>;
	initializeWindow();
}

AddingDialog::~AddingDialog()
{
	delete ui;
}

void AddingDialog::initializeWindow()
{
	ui->mainLayout->addWidget(new QLabel("Macros Constructor"), 1);

	ui->mainLayout->addWidget(new QLabel("Select macros execution type:"), 1);
	selectExecutionMode = new QComboBox();
	ui->mainLayout->addWidget(selectExecutionMode, 1);
	initializeExecutionModes();

	inputLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(inputLayout, 1);

	macrosLayout = new QVBoxLayout;
	ui->mainLayout->addLayout(macrosLayout, 1);

	QPushButton *addCommandButton = new QPushButton;
	addCommandButton->setText("Add command");
	ui->mainLayout->addWidget(addCommandButton, 1);
	connect(addCommandButton, SIGNAL(clicked()), this, SLOT(createCommandWidget()));

	buttonBox = new QDialogButtonBox;
	buttonBox->addButton(QDialogButtonBox::Ok);
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	buttonBox->addButton(QDialogButtonBox::Cancel);
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(OkClicked()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(CancelClicked()));
	ui->mainLayout->addWidget(buttonBox);
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
	emit wasUpdated();
	clearLayout(macrosLayout);
	close();
}

void AddingDialog::keyStringChanged(const QString *key)
{
	if (key->toStdString() == "") {
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

void AddingDialog::createCommandWidget()
{
	QVBoxLayout *layout = new QVBoxLayout;

	QLineEdit *path = new QLineEdit;
	layout->addWidget(path, 3);

	QComboBox *box = new QComboBox;
	for (int i = 0; i < Command::commandTypesNumber; i++)
		box->addItem(Command::commandTypes[i]);
	layout->addWidget(box, 2);

	PreCommand *newCommand = new PreCommand("", box->currentText());
	commandList->append(newCommand);

	QPushButton *deleteButton = new QPushButton;
	deleteButton->setText("X");
	layout->addWidget(deleteButton, 1);

	connect(path, SIGNAL(textChanged(QString)), newCommand, SLOT(updateCommandPath(QString)));
	connect(box, SIGNAL(currentTextChanged(QString)), newCommand, SLOT(updateCommandType(QString)));

	macrosLayout->addLayout(layout);
}
