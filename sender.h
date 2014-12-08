#include <QObject>
#include "addingdialog.h"
#include "controller.h"
#include "currentmacrosesdialog.h"

class Sender : public QObject
{
	Q_OBJECT
public:
	Sender(AddingDialog *mDial, Controller *mController, CurrentMacrosesDialog *mCurrent);

public slots:
	void newMacrosWasCreated();
	void deleteMacros(const QString &name);
	void needMacroses();
	void editMacros(const QString &macrosName);
private:
	void getAndSend();
	AddingDialog *mDial;
	CurrentMacrosesDialog *mCurrentMacroses;
	Controller *mController;
};
