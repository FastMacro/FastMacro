#include <QMainWindow>
#include "keypressfilter.h"
#include "commandline.h"
#include "commandexplorer.h"
#include "addingdialog.h"
#include "macrosfactory.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void yesWasClicked();
	void noWasClicked();
	void addWasClicked();
	void updateMakers();

private:
	Ui::MainWindow *ui;
	KeyPressFilter *keyFilter;
	CommandLine *commandLine;
	CommandExplorer *commandExplorer;
	MacrosFactory *macrosFactory;
	AddingDialog ad;
};
