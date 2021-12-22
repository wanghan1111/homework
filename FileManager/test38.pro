#-------------------------------------------------
#
# Project created by QtCreator 2019-10-28T10:46:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test38
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ZoonView.cpp \
    ImageDlg.cpp \
    MyTextEdit.cpp \
    TextDlg.cpp \
    RenameDlg.cpp \
    FileThread.cpp

HEADERS  += MainWindow.h \
    ZoonView.h \
    ImageDlg.h \
    MyTextEdit.h \
    TextDlg.h \
    RenameDlg.h \
    FileThread.h

FORMS    += MainWindow.ui \
    ImageDlg.ui \
    TextDlg.ui \
    RenameDlg.ui

RESOURCES += \
    Resource.qrc
