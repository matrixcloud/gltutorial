#-------------------------------------------------
#
# Project created by QtCreator 2015-09-27T08:19:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 007_Index
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cglwidget.cpp \
    objloader.cpp \
    tangentspace.cpp \
    vboindexer.cpp

HEADERS  += mainwindow.h \
    cglwidget.h \
    objloader.h \
    tangentspace.h \
    vboindexer.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
