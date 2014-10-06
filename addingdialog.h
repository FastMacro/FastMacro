#include <QDialog>
#include <QPair>

namespace Ui {
class AddingDialog;
}

class AddingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddingDialog(QWidget *parent = 0);
	~AddingDialog();

signals:
	void throwMacros(QString str);

public slots:
	void wasGood();

private:
	Ui::AddingDialog *ui;
};
