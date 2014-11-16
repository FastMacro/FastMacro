#include <QObject>
#include "addingdialog.h"
#include "controller.h"
#include "currentmacrosesdialog.h"

class Sender : public QObject
{
	Q_OBJECT
public:
	Sender(AddingDialog *mDial, Controller *mController, CurrentMacrosesDialog *mCurrnet);

public slots:
	void newMacrosWasCreated();
	void deleteMacros(QString name);
	void needMacroses();

private:
	void getAndSend();
	AddingDialog *mDial;
	CurrentMacrosesDialog *mCurrentMacroses;
	Controller *mController;
};
