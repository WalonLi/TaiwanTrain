/**
    Provide by Walon Li

    File: THSR.h
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"

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


void MainWindow::show_popup_error_message(STATE)
{
    qDebug() << "ERROR" << endl ;
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

STATE MainWindow::update_train_list_content()
{
    string start_station = ui->start_combo_box->currentText().toStdString() ;
    string arrival_station = ui->arrival_combo_box->currentText().toStdString() ;


    vector<Train> table = instance->get_table() ;
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

            // ui->train_list_widget->add
            // qDebug() << *start_time << " " << *arrival_time ;
        }

        delete start_time ;
        delete arrival_time ;
    }

    return STATE_SUCCESS ;
}

void MainWindow::on_GO_btn_clicked()
{
    STATE state ;

    // check date
    if (ui->date_edit_box->date() < QDate::currentDate())
        return show_popup_error_message(STATE_DATE_TIME_ERROR) ;

    // check focus train button
    if (!ui->btn_group_box->focusWidget())
        return show_popup_error_message(STATE_SELECT_TRAIN_ERROR) ;

    // check combo box
    if (ui->start_combo_box->currentText() == ui->arrival_combo_box->currentText())
        return show_popup_error_message(STATE_START_ARRIVE_IS_SAME_ERROR) ;

    // check instance
    if (!instance)
        return show_popup_error_message(STATE_DATA_ERROR) ;

    state = update_train_list_content() ;
    if (state != STATE_SUCCESS)
        return show_popup_error_message(state) ;

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
