#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QAction>
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

protected:
    void closeEvent(QCloseEvent *);

public slots:
	void yesWasClicked();
	void noWasClicked();
	void addWasClicked();
	void currentWasClicked();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setIcon();
    void createTrayIcon();
    QAction *minimizeAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    Ui::MainWindow *ui;
	KeyPressFilter *mKeyFilter;
	Controller *mController;
	Sender *mSender;
	AddingDialog *mAd;
	CurrentMacrosesDialog *mCm;
};
