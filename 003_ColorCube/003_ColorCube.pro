#-------------------------------------------------
#
# Project created by QtCreator 2015-09-13T13:38:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 003_ColorCube
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cglwidget.cpp

HEADERS  += mainwindow.h \
    cglwidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
