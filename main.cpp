#include "mainwindow.h"
#include "controller.h"
#include "keypressfilter.h"
#include <QApplication>
#include <functional>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
