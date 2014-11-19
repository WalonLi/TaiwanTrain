/**
    Provide by Walon Li

    File: THSR.h
**/

#include "../ITrainBase.h"

namespace ttp
{
class THSR : public ITrainBase
{
public:
    explicit THSR() : ITrainBase("www.thsrc.com.tw")
    {
        this->add_page_into_pages("/tw/TimeTable/WeeklyTimeTable/1") ;
        this->add_page_into_pages("/tw/TimeTable/WeeklyTimeTable/0") ;
    }

    ~THSR(){}

    // virtual STATE connect_server( const int ) ;
    virtual STATE parse_data_from_web() ;
private:

} ;
}
