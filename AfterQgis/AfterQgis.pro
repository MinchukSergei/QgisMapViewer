#-------------------------------------------------
#
# Project created by QtCreator 2016-07-05T17:01:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += xml
QT += network
QT += qgis



TARGET = AfterQgis
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui 

LIBS += -L/usr/local/lib -lqgis_core

INCLUDEPATH += /usr/local/include/qgis

QMAKE_CXXFLAGS += -std=c++0x

DEFINES += GUI_EXPORT= CORE_EXPORT=
