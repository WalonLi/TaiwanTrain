#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>

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
    void on_GO_btn_clicked();

    void on_THSR_btn_clicked();

    void on_TRA_btn_clicked();

    void on_TRTC_btn_clicked();

    void on_KRTC_btn_clicked();

    void on_Refresh_btn_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
