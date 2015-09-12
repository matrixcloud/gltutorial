#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T14:14:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 001_Triangle
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cglwidget.cpp

HEADERS  += mainwindow.h \
    cglwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
