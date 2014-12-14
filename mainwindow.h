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
    /// What is happening when you click to exit button
    void closeEvent(QCloseEvent *);

public slots:
    /// start scanning user's text mode
    void activeWasClicked();
    /// check from systray was done
    void trayCheck();

    void addWasClicked();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setIcon();
    void createTrayIcon();
    void startTrayIcon();
    void setConnections();
    QAction *minimizeAction;
    QAction *changeStateAction;
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
