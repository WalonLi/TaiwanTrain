/**
    Provide by Walon Li

    File: mainwindow.h
**/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "ITrainBase.h"
#include "THSR/THSR.h"
#include "TRA/TRA.h"
#include "TRTC/TRTC.h"
#include "KRTC/KRTC.h"
#include "SpinBar/spinbar.h"
#include "Action/ParseAction.h"
#include "Action/GoAction.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

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

    void get_parse_action_result_slot(int) ;

    void get_go_action_result_slot(int, QStringList) ;

private:

    // member function
    void show_popup_error_message(ttp::STATE st) ;

    void refresh_start_station_combobox() ;

    void refresh_arrival_station_combobox() ;


private:
    // member
    Ui::MainWindow *ui;
    ttp::ITrainBase *instance;
    ttp::ParseAction *parse_action ;
    ttp::GoAction *go_action ;
    ttp::SpinBar *spinbar ;
};

#endif // MAINWINDOW_H
