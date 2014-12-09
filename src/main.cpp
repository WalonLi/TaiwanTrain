/**
    Provide by Walon Li

    File: main.cpp
**/

#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // move window to center.
    if (QApplication::desktop()->screenCount() <= 1)
    {
        w.move((QApplication::desktop()->width() - w.width())/2,
               (QApplication::desktop()->height() - w.height())/2);
    }
    else
    {
        QRect rect = QApplication::desktop()->screenGeometry(0);
        w.move((rect.width()-w.width())/2,
               (rect.height()-w.height())/2);
    }

    return a.exec();
}
