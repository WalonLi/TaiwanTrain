/**
    Provide by Walon Li

    File: TRTC.h
**/
#ifndef TRTC_H
#define TRTC_H

#include "../ITrainBase.h"

namespace ttp
{
class TRTC : public ITrainBase
{
public:
    explicit TRTC() : ITrainBase("web.trtc.com.tw")
    {
        this->add_page_into_pages("/c/2stainfo.asp") ;
    }

    ~TRTC(){}

    // virtual STATE connect_server( const int ) ;
    virtual STATE parse_data_from_web() ;
    virtual STATE get_list_with_user_input(QDate, string, string, QStringList&) ;
private:

} ;
}
#endif
