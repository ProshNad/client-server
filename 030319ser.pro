#-------------------------------------------------
#
# Project created by QtCreator 2019-04-03T17:48:10
#
#-------------------------------------------------

QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 030319ser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    dialog2.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    dialog2.h

FORMS    += mainwindow.ui \
    dialog.ui \
    dialog2.ui
