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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_GO_btn_clicked()
{
}


void MainWindow::on_THSR_btn_clicked()
{
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