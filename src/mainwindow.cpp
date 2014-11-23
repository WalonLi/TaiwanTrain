/**
    Provide by Walon Li

    File: THSR.h
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

using namespace ttp ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    instance(0)
{
    this->setFixedSize(800,600);
    ui->setupUi(this);

    // set default date is current date.
    ui->date_edit_box->setDate(QDate::currentDate());
}

MainWindow::~MainWindow()
{
    delete ui ;
    delete instance ;
}


void MainWindow::show_popup_error_message(STATE st)
{
    QMessageBox *msg = 0 ;

    switch(st)
    {
    case STATE_DATA_ERROR :
        msg = new QMessageBox(QMessageBox::Warning,
                              "Data",
                              "資料錯誤。") ;
        break ;
    case STATE_DATE_TIME_ERROR :
        msg = new QMessageBox(QMessageBox::Warning,
                              "Date",
                              "請選擇正確的日期。") ;
        break ;
    case STATE_NETWORK_ERROR :
        msg = new QMessageBox(QMessageBox::Warning,
                              "Network",
                              "網路阻塞，請重新連線或者檢查您的網路。") ;
        break ;
    case STATE_SELECT_TRAIN_ERROR :
        msg = new QMessageBox(QMessageBox::Warning,
                              "Type",
                              "請於左方Menu選擇車種。") ;
        break ;
    case STATE_START_ARRIVE_IS_SAME_ERROR :
        msg = new QMessageBox(QMessageBox::Warning,
                              "Start/Arrival",
                              "起始站和抵達站無法相同。") ;
        break ;
    }

    msg->exec() ;
    delete msg ;
}

void MainWindow::refresh_start_station_combobox()
{
    ui->start_combo_box->clear() ;

    if (!instance) return ;
    auto map = instance->get_station_map() ;
    for (auto it = map.begin() ; it != map.end() ; ++it)
        ui->start_combo_box->addItem((*it).c_str());
}

void MainWindow::refresh_arrival_station_combobox()
{
    ui->arrival_combo_box->clear();

    if (!instance) return ;
    auto map = instance->get_station_map() ;
    for (auto it = map.begin() ; it != map.end() ; ++it)
        ui->arrival_combo_box->addItem((*it).c_str());
}

void MainWindow::on_GO_btn_clicked()
{
    // check date
    if (ui->date_edit_box->date() < QDate::currentDate())
        return show_popup_error_message(STATE_DATE_TIME_ERROR) ;

    // check focus train button
    if (!ui->btn_group_box->focusWidget())
        return show_popup_error_message(STATE_SELECT_TRAIN_ERROR) ;

    // check instance
    if (!instance)
        return show_popup_error_message(STATE_DATA_ERROR) ;

    // check combo box
    if (ui->start_combo_box->currentText() == ui->arrival_combo_box->currentText())
        return show_popup_error_message(STATE_START_ARRIVE_IS_SAME_ERROR) ;

}


void MainWindow::on_THSR_btn_clicked()
{
    STATE state ;
    if (instance) delete instance ;

    instance = new THSR() ;
    state = instance->parse_data_from_web() ;
    if ( state != STATE_SUCCESS)
        return show_popup_error_message(state) ;

    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;

    /*
    vector<string> t = instance->get_station_map() ;
    for (auto it = t.begin() ; it != t.end() ; ++it )
    {
        qDebug() << (*it).c_str() ;
    }
    */
    /*
    vector<ttp::Train> t = mTHSR.get_table() ;
    for (auto it = t.begin() ; it != t.end() ; ++it)
    {
        qDebug() << (*it).get_id().c_str() ;
        vector< pair<string, QTime> > t2 = (*it).get_data() ;

        for(auto it2 = t2.begin() ; it2 != t2.end() ; ++it2 )
        {
            qDebug() << (*it2).first.c_str() << " " << (*it2).second ;
        }
    }
    */
}

void MainWindow::on_TRA_btn_clicked()
{
    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;

}

void MainWindow::on_TRTC_btn_clicked()
{
    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;
}

void MainWindow::on_KRTC_btn_clicked()
{
    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;
}

void MainWindow::on_Refresh_btn_clicked()
{
    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;
}

void MainWindow::on_EXIT_btn_clicked()
{
    QApplication::quit() ;
}
