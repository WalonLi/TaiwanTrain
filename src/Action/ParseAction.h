/**
    Provide by Walon Li

    File: ParseAction.h
**/
#ifndef PARSEACTION_H
#define PARSEACTION_H

#include <QThread>
#include <QWidget>
#include "../ITrainBase.h"

namespace ttp
{
class ParseAction : public QThread
{
    Q_OBJECT
public:
    ParseAction(ITrainBase *t) : instance(t) {}
    ~ParseAction() {}

    void set_train_type_instance(ITrainBase *t){instance = t ;}

signals:
    void send_parse_action_result_signal(int) ;

protected:
    void run()
    {
        if (!instance) return ;

        STATE state  = instance->parse_data_from_web() ;
        emit send_parse_action_result_signal(state) ;
    }

private:
    ITrainBase *instance;
};
}
#endif // PARSEACTION_H
