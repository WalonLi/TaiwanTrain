#ifndef ACTIONTHREAD_H
#define ACTIONTHREAD_H

#include <QThread>
#include <QWidget>
#include "ITrainBase.h"

namespace ttp
{
class ActionThread : public QThread
{
    Q_OBJECT
public:
    ActionThread(ITrainBase *t) : instance(t) {}
    ~ActionThread() {}

    void set_train_type_instance(ITrainBase *t){instance = t ;}

signals:
    void send_result_signal(int) ;

protected:
    void run()
    {
        if (!instance) return ;

        STATE state  = instance->parse_data_from_web() ;
        emit send_result_signal(state) ;
    }

private:
    ITrainBase *instance;
};
}
#endif // ACTIONTHREAD_H
