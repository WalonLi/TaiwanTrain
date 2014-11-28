#-------------------------------------------------
#
# Project created by QtCreator 2014-11-16T12:16:35
#
#-------------------------------------------------

QT += core gui declarative testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = TaiwanTrain
TEMPLATE = app

linux-g++* {
    INCLUDEPATH += /usr/include/boost_1_57_0
    LIBS        += -L /usr/lib/boost_lib -lboost_system
}

win32-g++* {
    Boost_VERSION = 1_56
    COMPILER      = mgw48
    Boost_DIR     = G:/boost_1_56_0
    INCLUDEPATH += G:/boost_1_56_0
    LIBS        += G:/boost_1_56_0/stage/lib/libboost_filesystem-$${COMPILER}-mt-$${Boost_VERSION}.a \
                   G:/boost_1_56_0/stage/lib/libboost_system-$${COMPILER}-mt-$${Boost_VERSION}.a \
                   -lws2_32
    PRE_TARGETDEPS += G:/boost_1_56_0/stage/lib/libboost_filesystem-$${COMPILER}-mt-$${Boost_VERSION}.a \
                      G:/boost_1_56_0/stage/lib/libboost_system-$${COMPILER}-mt-$${Boost_VERSION}.a
}

CONFIG  += c++11
DEFINES += STATIC

SOURCES += src/main.cpp\
           src/mainwindow.cpp\
           src/THSR/THSR.cpp\
           src/SpinBar/busyindicator.cpp

HEADERS += src/mainwindow.h\
           src/ITrainBase.h\
           src/Train.h\
           src/THSR/THSR.h\
           src/basic.h\
           src/SpinBar/busyindicator.h \
           src/SpinBar/spinbar.h

FORMS   += UI/mainwindow.ui

OTHER_FILES +=
