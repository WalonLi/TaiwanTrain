/**
    Provide by Walon Li

    File: GoAction.h
**/
#ifndef GOACTION_H
#define GOACTION_H

#include <QThread>
#include <QWidget>
#include <QStringList>
#include "../ITrainBase.h"

namespace ttp
{
class GoAction : public QThread
{
    Q_OBJECT
public:
    GoAction(ITrainBase *t) : instance(t) {}
    ~GoAction() {}

    void set_user_input(ITrainBase *t, QDate d, string s, string a)
    {
        instance = t ;
        date = d ;
        start_station = s ;
        arrival_station = a ;
    }

signals:
    void send_go_action_result_signal(int, QStringList) ;

protected:
    void run()
    {
        if (!instance) return ;
        QStringList list ;
        STATE state = instance->get_list_with_user_input(date, start_station, arrival_station, list) ;
        emit send_go_action_result_signal(state, list) ;
    }

private:
    ITrainBase *instance;
    QDate date ;
    string start_station ;
    string arrival_station ;
};
}
#endif // GOACTION_H
