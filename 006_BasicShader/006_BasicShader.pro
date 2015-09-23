#-------------------------------------------------
#
# Project created by QtCreator 2015-09-23T15:04:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 006_BasicShader
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
