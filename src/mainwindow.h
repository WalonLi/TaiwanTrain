#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "ITrainBase.h"
#include "THSR/THSR.h"

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

private :
    // member function
    void show_popup_error_message(ttp::STATE) ;

    void refresh_start_station_combobox() ;

    void refresh_arrival_station_combobox() ;

    ttp::STATE update_train_list_content() ;

private:
    // member
    Ui::MainWindow *ui;
    ttp::ITrainBase *instance;
};

#endif // MAINWINDOW_H
