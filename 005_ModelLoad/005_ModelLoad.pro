#-------------------------------------------------
#
# Project created by QtCreator 2015-09-15T16:42:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 005_ModelLoad
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cglwidget.cpp \
    objloader.cpp

HEADERS  += mainwindow.h \
    cglwidget.h \
    objloader.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
