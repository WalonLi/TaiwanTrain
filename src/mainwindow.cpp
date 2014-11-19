/**
    Provide by Walon Li

    File: THSR.h
**/

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->setFixedSize(800,600);
    ui->setupUi(this);

    // set default date is current date.
    ui->date_edit_box->setDate(QDate::currentDate());

    // test
    on_THSR_btn_clicked() ;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::show_popup_error_message(ttp::STATE)
{
    qDebug() << "ERROR" << endl ;
}

void MainWindow::on_GO_btn_clicked()
{
}


void MainWindow::on_THSR_btn_clicked()
{
    ttp::STATE state ;
    ttp::THSR train ;

    state = train.parse_data_from_web();
    if (state != ttp::STATE_SUCCESS)
    {
        show_popup_error_message(state) ;
        return ;
    }

    vector<ttp::Train> t = train.get_table() ;
    for (auto it = t.begin() ; it != t.end() ; ++it)
    {
        qDebug() << (*it).get_id().c_str() ;
        vector< pair<string, QTime> > t2 = (*it).get_data() ;

        for(auto it2 = t2.begin() ; it2 != t2.end() ; ++it2 )
        {
            qDebug() << (*it2).first.c_str() << " " << (*it2).second ;
        }
    }
}

void MainWindow::on_TRA_btn_clicked()
{

}

void MainWindow::on_TRTC_btn_clicked()
{

}

void MainWindow::on_KRTC_btn_clicked()
{

}

void MainWindow::on_Refresh_btn_clicked()
{

}
