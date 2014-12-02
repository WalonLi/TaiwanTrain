/**
    Provide by Walon Li

    File: THSR.h
**/

#include "../ITrainBase.h"

namespace ttp
{
class TRA : public ITrainBase
{
public:
    explicit TRA() : ITrainBase("twtraffic.tra.gov.tw")
    {
        this->add_page_into_pages("/twrail/") ;
        this->add_page_into_pages("/twrail/SearchResult.aspx?") ;
    }

    ~TRA(){}

    // virtual STATE connect_server( const int ) ;
    virtual STATE parse_data_from_web() ;
    virtual QStringList get_list_with_user_input(QDate, QString, QString) ;
private:

} ;
}
