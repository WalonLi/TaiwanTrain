/**
    Provide by Walon Li

    File: mainwindow.cpp
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SpinBar/spinbar.h"
#include <QMessageBox>
#include <QScrollBar>


using namespace ttp ;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    instance(0)
{

    spinbar = new SpinBar(this) ;

    parse_action = new ParseAction(0) ;
    QObject::connect(parse_action, SIGNAL(send_parse_action_result_signal(int)),
                     this, SLOT(get_parse_action_result_slot(int))) ;

    go_action = new GoAction(0) ;
    QObject::connect(go_action, SIGNAL(send_go_action_result_signal(int, QStringList)),
                     this, SLOT(get_go_action_result_slot(int, QStringList))) ;

    this->setFixedSize(800,600);
    ui->setupUi(this);

    // set default date is current date.
    ui->date_edit_box->setDate(QDate::currentDate());

    // set scroll bar
    ui->start_combo_box->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded) ;
    ui->arrival_combo_box->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded) ;

}

MainWindow::~MainWindow()
{
    delete ui ;
    delete instance ;
    delete parse_action ;
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
    case STATE_DATA_NOT_FOUND :
        msg = new QMessageBox(QMessageBox::Warning,
                              "No Data",
                              "沒有資料。") ;
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

void MainWindow::get_parse_action_result_slot(int state)
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

void MainWindow::get_go_action_result_slot(int state, QStringList list)
{
    ui->train_list_widget->clear();

    if (state != STATE_SUCCESS)
        show_popup_error_message(static_cast<STATE>(state)) ;
    else
        ui->train_list_widget->addItems(list);

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

    spinbar->pop_up_spin_loading_bar();
    go_action->set_user_input(instance, ui->date_edit_box->date(),
                              ui->start_combo_box->currentText().toStdString(),
                              ui->arrival_combo_box->currentText().toStdString());
    go_action->start();
}


void MainWindow::on_THSR_btn_clicked()
{
    if (instance) delete instance ;

    ui->train_list_widget->clear();
    spinbar->pop_up_spin_loading_bar();
    instance = new THSR() ;
    parse_action->set_train_type_instance(instance);
    parse_action->start();
}

void MainWindow::on_TRA_btn_clicked()
{
    if (instance) delete instance ;

    ui->train_list_widget->clear();
    spinbar->pop_up_spin_loading_bar();
    instance = new TRA() ;
    parse_action->set_train_type_instance(instance);
    parse_action->start();

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
    if (instance) delete instance ;

    ui->train_list_widget->clear();
    spinbar->pop_up_spin_loading_bar();
    instance = new TRTC() ;
    parse_action->set_train_type_instance(instance);
    parse_action->start();
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
    parse_action->set_train_type_instance(instance);
    parse_action->start();
}

void MainWindow::on_EXIT_btn_clicked()
{
    QApplication::quit() ;
}


