/**
    Provide by Walon Li

    File: KRTC.h
**/
#ifndef KRTC_H
#define KRTC_H

#include "../ITrainBase.h"

namespace ttp
{
class KRTC : public ITrainBase
{
public:
    explicit KRTC() : ITrainBase("www.krtco.com.tw"), view_state(""), validate("")
    {
        this->add_page_into_pages("/") ;
    }

    ~KRTC(){}

    // virtual STATE connect_server( const int ) ;
    virtual STATE parse_data_from_web() ;
    virtual STATE get_list_with_user_input(QDate, string, string, QStringList&) ;
private:
    string view_state ;
    string validate ;
} ;
}
#endif
