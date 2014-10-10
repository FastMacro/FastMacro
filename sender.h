#include <QObject>
#include "addingdialog.h"
#include "controller.h"

class Sender : public QObject
{
	Q_OBJECT
public:
	Sender(AddingDialog *mDial, Controller *mController);

public slots:
	void newMacrosWasCreated();

private:
	void getAndSend();
	AddingDialog *mDial;
	Controller *mController;
};
