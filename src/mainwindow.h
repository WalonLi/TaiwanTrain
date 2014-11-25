#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimeLine>
#include "ITrainBase.h"
#include "THSR/THSR.h"
#include "SpinBar/busyindicator.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    Ui::MainWindow *get_ui_ptr(){return ui;} ;

private slots:
    // button click signal function
    void on_GO_btn_clicked();

    void on_THSR_btn_clicked();

    void on_TRA_btn_clicked();

    void on_TRTC_btn_clicked();

    void on_KRTC_btn_clicked();

    void on_Refresh_btn_clicked();

    void on_EXIT_btn_clicked();

    void rotate_spinner(int v);

private :
    // member function
    void show_popup_error_message(ttp::STATE st) ;

    void refresh_start_station_combobox() ;

    void refresh_arrival_station_combobox() ;

    void pop_up_spinner_bar() ;

private:
    // member
    Ui::MainWindow *ui;
    ttp::ITrainBase *instance;
    BusyIndicator* m_pBusyIndicator;
    QGraphicsView* pView ;
    QTimeLine* m_pTimeLine;
};

#endif // MAINWINDOW_H
