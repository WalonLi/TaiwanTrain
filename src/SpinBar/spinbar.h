#ifndef SPINBAR_H
#define SPINBAR_H

#include "busyindicator.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimeLine>
#include <QtTest/QTest>
#include <QObject>

// class MainWindow ;

namespace ttp
{
class SpinBar : public QObject
{
    Q_OBJECT
public :
    explicit SpinBar(QWidget *m) : QObject(m)
    {
        ParentWidget = m ;
    }

    ~SpinBar()
    {
        delete Indicator ;
        delete View ;
        delete TimeLine ;
        delete Scene ;
    }

    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    void pop_up_spin_loading_bar()
    {
        ParentWidget->setEnabled(false);

        Scene = new QGraphicsScene();
        Indicator = new BusyIndicator();
        Scene->addItem(dynamic_cast<QGraphicsItem*>(Indicator));
        // QGraphicsScene* m_scene = NULL;

        View = new QGraphicsView(Scene, ParentWidget);
        View->setMinimumHeight(100);
        View->setMinimumWidth(100);

        // None bolder style
        View->setWindowFlags(Qt::FramelessWindowHint);
        View->setStyleSheet("background: transparent;border:Opx");

        View->move((ParentWidget->width()-View->width())/2,
                   (ParentWidget->height()-View->height())/2);
        View->show();

        TimeLine = new QTimeLine(1000, ParentWidget);
        TimeLine->setLoopCount(20);
        TimeLine->setFrameRange(0, 36);

        connect(TimeLine, SIGNAL(frameChanged(int)), this, SLOT(rotate_spinner(int)));
        TimeLine->start();

        // QTest::qWait(1000) ;
    }

    void close_spin_loading_bar()
    {
        // TimeLine->stop();
        // View->close() ;
        /*
        delete Indicator ;
        delete View ;
        delete TimeLine ;
        // delete Scene ;*/
        ParentWidget->setEnabled(true);
    }

private slots:
    void rotate_spinner(int v)
    {
        qreal nTransX = Indicator->actualOuterRadius();
        Indicator->setTransform(QTransform().translate(nTransX, nTransX).
                            rotate(v*10).translate(-1*nTransX, -1*nTransX));
    }

private:
    QWidget *ParentWidget ; // this widget don't delete !!
    BusyIndicator* Indicator;
    QGraphicsView* View ;
    QTimeLine* TimeLine;
    QGraphicsScene* Scene ;
};
}
#endif // SPINBAR_H
