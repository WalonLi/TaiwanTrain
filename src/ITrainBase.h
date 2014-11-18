/**
    Provide by Walon Li

    File: ITrainBase.h
**/

#ifndef ITRAIN_BASE_H
#define ITRAIN_BASE_H

#include <string>
#include "boost/asio.hpp"
#include "state.h"
#include "Train.h"


namespace ttp
{
class ITrainBase
{
public:
    ITrainBase(){} ;
    ~ITrainBase(){} ;


    const std::vector<Train> & get_table() { return table; }

    STATE connect_server( const int time_out )
    {
        if (!time_out) return STATE_NETWORK_ERROR ;
        asio_stream.expires_from_now(boost::posix_time::seconds(time_out)) ;

        asio_stream.connect(web_server, "http") ;
        if (!asio_stream)
            return STATE_NETWORK_ERROR ;
        else
        {
            asio_stream << "GET" << web_page << " HTTP/1.0\r\n" ;
            asio_stream << "Host: " << web_server << "\r\n" ;
            asio_stream << "Accept: */*\r\n" << "Connection: close\r\n\r\n" ;
            return STATE_SUCCESS ;
        }
    }

//    virtual
//#endif
protected:
    STATE add_train_into_table(const Train t)
    {
        if (!t) return STATE_ERROR ;
        else
        {
            table.push_back(t) ;
            return STATE_SUCCESS ;

        }
    }


    boost::asio::ip::tcp::iostream & get_stream() { return asio_stream; }


private:
    std::vector<Train> table ;
    boost::asio::ip::tcp::iostream asio_stream ;
    std::string web_server ;
    std::string web_page ;
} ;
}


#endif // ITRAIN_H
