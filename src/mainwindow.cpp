/**
    Provide by Walon Li

    File: THSR.h
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "boost/format.hpp"
#include "SpinBar/spinbar.h"
#include <QMessageBox>



using namespace ttp ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    instance(0)
{

    spinbar = new SpinBar(this) ;

    action = new ActionThread(0) ;
    QObject::connect(action, SIGNAL(send_result_signal(int)),
                     this, SLOT(get_result_slot(int))) ;

    this->setFixedSize(800,600);
    ui->setupUi(this);

    // set default date is current date.
    ui->date_edit_box->setDate(QDate::currentDate());
    // while(true) qDebug() <<"123";
}

MainWindow::~MainWindow()
{
    delete ui ;
    delete instance ;
    delete action ;
    delete spinbar ;
}


void MainWindow::show_popup_error_message(STATE st)
{
    // close spin bar more carefully
    spinbar->close_spin_loading_bar();

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
        ui->start_combo_box->addItem((*it).second.c_str());
}

void MainWindow::refresh_arrival_station_combobox()
{
    ui->arrival_combo_box->clear();

    if (!instance) return ;
    auto map = instance->get_station_map() ;
    for (auto it = map.begin() ; it != map.end() ; ++it)
        ui->arrival_combo_box->addItem((*it).second.c_str());
}

STATE MainWindow::update_train_list_content()
{
    string start_station = ui->start_combo_box->currentText().toStdString() ;
    string arrival_station = ui->arrival_combo_box->currentText().toStdString() ;
    vector<Train> table = instance->get_table() ;

    ui->train_list_widget->clear();
    for (auto it = table.begin() ; it != table.end() ; ++it)
    {
        QTime *start_time = 0, *arrival_time = 0 ;
        vector< pair<string, QTime> > schedule = (*it).get_schedule() ;
        for (auto it2 = schedule.begin() ; it2 != schedule.end() ; ++it2)
        {
            // check time is legal.
            if ((*it2).second.isNull()) continue ;

            if (!(*it2).first.compare(start_station))
                start_time = new QTime((*it2).second) ;
            else if (!(*it2).first.compare(arrival_station) && start_time)
                arrival_time = new QTime((*it2).second) ;

            if (start_time && arrival_time) break ;
        }

        if (start_time && arrival_time)
        {
            string str = (boost::format("車號:%-10s出發:%-10s抵達:%-10s%s") \
                                        % it->get_id().c_str() \
                                        % start_time->toString("hh:mm").toStdString().c_str() \
                                        % arrival_time->toString("hh:mm").toStdString().c_str()\
                                        % it->get_help().c_str()).str() ;
            ui->train_list_widget->addItem(str.c_str()) ;
        }

        delete start_time ;
        delete arrival_time ;
    }

    return STATE_SUCCESS ;
}

void MainWindow::get_result_slot(int state)
{
    if (state != STATE_SUCCESS)
    {
        show_popup_error_message(static_cast<STATE>(state)) ;
    }
    else
    {
        refresh_start_station_combobox() ;
        refresh_arrival_station_combobox() ;
    }

    spinbar->close_spin_loading_bar();
}

void MainWindow::on_GO_btn_clicked()
{
    STATE state ;

    // check date
    if (ui->date_edit_box->date() < QDate::currentDate())
        return show_popup_error_message(STATE_DATE_TIME_ERROR) ;

    /*
    // check focus train button
    if (!ui->btn_group_box->focusWidget())
        return show_popup_error_message(STATE_SELECT_TRAIN_ERROR) ;
    */

    // check instance
    if (!instance)
        return show_popup_error_message(STATE_SELECT_TRAIN_ERROR) ;

    // check combo box
    if (ui->start_combo_box->currentText() == ui->arrival_combo_box->currentText())
        return show_popup_error_message(STATE_START_ARRIVE_IS_SAME_ERROR) ;

    state = update_train_list_content() ;
    if (state != STATE_SUCCESS)
        return show_popup_error_message(state) ;

}


void MainWindow::on_THSR_btn_clicked()
{
    if (instance) delete instance ;

    spinbar->pop_up_spin_loading_bar();
    instance = new THSR() ;
    action->set_train_type_instance(instance);
    action->start();
}

void MainWindow::on_TRA_btn_clicked()
{
    if (instance) delete instance ;

    spinbar->pop_up_spin_loading_bar();
    instance = new TRA() ;
    action->set_train_type_instance(instance);
    action->start();

    /*
    spinbar->pop_up_spin_loading_bar();

    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;

    spinbar->close_spin_loading_bar();
    */
}

void MainWindow::on_TRTC_btn_clicked()
{
    /*
    spinbar->pop_up_spin_loading_bar() ;

    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;

    spinbar->close_spin_loading_bar();
    */
}

void MainWindow::on_KRTC_btn_clicked()
{
    /*
    spinbar->pop_up_spin_loading_bar() ;

    STATE state ;
    if (instance) delete instance ;

    instance = 0 ;
    refresh_start_station_combobox() ;
    refresh_arrival_station_combobox() ;

    spinbar->close_spin_loading_bar();
    */
}

void MainWindow::on_Refresh_btn_clicked()
{
    if (!instance) return  ;

    // clear data.
    ui->start_combo_box->clear();
    ui->arrival_combo_box->clear();
    ui->train_list_widget->clear();
    ui->date_edit_box->setDate(QDate::currentDate());

    // start spin bar and parse web page again
    spinbar->pop_up_spin_loading_bar();
    if (ITrainBase *t = dynamic_cast<THSR*>(instance))
    {
        delete instance ;
        instance = new THSR() ;
    }
    action->set_train_type_instance(instance);
    action->start();
}

void MainWindow::on_EXIT_btn_clicked()
{
    QApplication::quit() ;
}


