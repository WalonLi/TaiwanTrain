#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T12:16:35
#
#-------------------------------------------------

QT			+= core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET		= TaiwanTrain
TEMPLATE	= app

INCLUDEPATH += /usr/include/boost_1_57_0
LIBS		+= -L /usr/lib/boost_lib -lboost_system
CONFIG		+= boost
DEFINES		+= STATIC

SOURCES += src/main.cpp\
		   src/mainwindow.cpp

HEADERS  += src/mainwindow.h

FORMS    += UI/mainwindow.ui
