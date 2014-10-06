#-------------------------------------------------
#
# Project created by QtCreator 2014-09-30T21:54:11
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FastMacro
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    commandline.cpp \
    commandexplorer.cpp \
    addingdialog.cpp \
    macrosfactory.cpp

HEADERS  += mainwindow.h \
    commandline.h \
    commandexplorer.h \
    keypressfilter.h \
    addingdialog.h \
    macrosfactory.h

FORMS    += mainwindow.ui \
    addingdialog.ui
