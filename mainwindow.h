#include <QMainWindow>
#include "keypressfilter.h"
#include "addingdialog.h"
#include "controller.h"
#include "sender.h"
#include "currentmacrosesdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	AddingDialog *getAddingDialogWindow();
	~MainWindow();

public slots:
	void yesWasClicked();
	void noWasClicked();
	void addWasClicked();
	void currentWasClicked();

private:
	Ui::MainWindow *ui;
	KeyPressFilter *mKeyFilter;
	Controller *mController;
	Sender *mSender;
	AddingDialog *mAd;
	CurrentMacrosesDialog *mCm;
};
