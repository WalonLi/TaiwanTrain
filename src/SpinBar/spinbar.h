#ifndef SPINBAR_H
#define SPINBAR_H

#include "busyindicator.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimeLine>
#include <QtTest/QTest>
#include <QObject>

namespace ttp
{
class SpinBar : public QObject
{
    Q_OBJECT
public :
    explicit SpinBar(QWidget *m) :
        QObject(m),
        ParentWidget(m),
        Indicator(0),
        View(0),
        TimeLine(0),
        Scene(0) {}

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

        if (!Indicator)
            Indicator = new BusyIndicator();

        if (!Scene)
        {
            Scene = new QGraphicsScene();
            Scene->addItem(dynamic_cast<QGraphicsItem*>(Indicator));
        }

        if (!View)
        {
            View = new QGraphicsView(Scene, ParentWidget);
            View->setMinimumHeight(100);
            View->setMinimumWidth(100);

            // None bolder style
            View->setWindowFlags(Qt::FramelessWindowHint);
            View->setStyleSheet("background: transparent;border:Opx");

            View->move((ParentWidget->width()-View->width())/2,
                   (ParentWidget->height()-View->height())/2);
        }

        if (!TimeLine)
        {
            TimeLine = new QTimeLine(1000, ParentWidget);
            TimeLine->setLoopCount(0);
            TimeLine->setFrameRange(0, 36);
            connect(TimeLine, SIGNAL(frameChanged(int)), this, SLOT(rotate_spinner(int)));
        }

        View->show();
        TimeLine->start();

        QTest::qWait(500) ;
    }

    void close_spin_loading_bar()
    {
        if (TimeLine && View && ParentWidget)
        {
            TimeLine->stop();
            View->close() ;
            ParentWidget->setEnabled(true);
        }
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
