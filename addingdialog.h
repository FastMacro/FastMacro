#pragma once
#include <QDialog>
#include <QPair>
#include <QMap>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QList>
#include <QLabel>
#include <QPushButton>
#include <macros.h>
#include "command.h"
#include "macrosoutputholder.h"

class GestureController;

namespace Ui {
class AddingDialog;
}

class AddingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddingDialog(QWidget *parent = 0);
	~AddingDialog();
	void addMacros();
	void editMacros(Macros *macros);
	MacrosOutputHolder *holder;
	void setGesture(GestureController *controller);
signals:
	void wasUpdated();
	void deleteMacros(const QString &name);
	void refreshCurrentMacroses();

private slots:
	void OkClicked();
	void CancelClicked();
	void keyStringChanged(const QString &key);
	void modeChanged(const QString &mode);
	void addCommandWidget();
	void recordShortcut();

private:
	void initialize();
	QMap<QString, Macros> *load();
	void initializeExecutionModes();
	void initializeMouseGestures();
	void createCommandWidget(const QString &oldPath, const QString &oldType);
	QString editingMacrosName;
	QString editingGesture;
	GestureController *gestureController = nullptr;
	Ui::AddingDialog *ui;
	QLineEdit *macrosName;
	QVBoxLayout *inputLayout;
	QVBoxLayout *macrosLayout;
	QLineEdit *keyString;
	QLabel *shortcutLabel;
	QComboBox *selectExecutionMode;
	QDialogButtonBox *buttonBox;
	QMap<QString, QString> *executionModes;
	QList<PreCommand*> *commandList;
	QSet<QString> *shortcutKeys = nullptr;
	static const int gesturesNumber = 6;
	const QString gestureNames[gesturesNumber] = {"horizontalline", "leftlowercorner", "leftuppercorner", "rightlowercorner", "rightuppercorner", "verticalline"};
	const int mWidth = 400;
	const int mHeight = 300;
};

class CommandDestructor : public QObject {
Q_OBJECT
public:
	PreCommand *command;
	QLayout *commandLayout;
	QList<PreCommand*> *commandList;
public slots:
	void deleteCommand();
};

class GestureController : public QObject
{
Q_OBJECT
public:
	GestureController(AddingDialog *newDial, QPushButton *newButton, const QString &newGesture) : dial(newDial), button(newButton), gesture(newGesture) {}
	QPushButton *getButton()
	{
		return button;
	}
	QString getGesture()
	{
		return gesture;
	}
public slots:
	void setGesture()
	{
		dial->setGesture(this);
	}
private:
	AddingDialog *dial;
	QPushButton *button;
	QString gesture;
};
