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
    explicit ITrainBase(const string server) : web_server(server) {}
    ~ITrainBase(){} ;


    // get private data method
    const vector<Train> & get_table() { return table; }
    const string & get_web_server() { return web_server; }
    const vector<string> & get_web_pages() { return web_pages; }

    // paring data method
    virtual STATE parse_data_from_web() = 0 ;

protected:
    // add train into table
    STATE add_train_into_table(const Train t)
    {
        if (!t) return STATE_ERROR ;
        else
        {
            table.push_back(t) ;
            return STATE_SUCCESS ;
        }
    }

    // add page into pages
    void add_page_into_pages(const string a){ web_pages.push_back(a); }


    // asio function
    boost::asio::ip::tcp::iostream & get_stream() { return asio_stream; }
    // try to connect server.
    virtual STATE connect_server( const int time_out = 30 )
    {
        if (!time_out) return STATE_NETWORK_ERROR ;
        asio_stream.expires_from_now(boost::posix_time::seconds(time_out)) ;

        asio_stream.connect(web_server, "http") ;
        if (!asio_stream)
            return STATE_NETWORK_ERROR ;
        else
            return STATE_SUCCESS ;
    }

private:
    boost::asio::ip::tcp::iostream asio_stream ;
    vector<Train> table ;
    string web_server ;
    vector<string> web_pages ;
} ;
}


#endif // ITRAIN_H


#if 0
        stream << "Accept: text/html,application/xhtml+xml,appliction/xml;q=0.9,image/webp,*/*;q=0.8\r\n" ;
        stream << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/38.0.2125.111 Safari/537.36\r\n" ;
        stream << "Referer: http://" << server << "/tw/TimeTable/SearchResult\r\n" ;
        stream << "Accept-Encoding: gzip,deflate,sdch\r\n" ;
        stream << "Accept-Language: zh-TW,zh;q=0.8,en-US;q=0.6,en;q=0.4,zh-CN;q=0.2\r\n" ;
        stream << "\r\n" ;
#endif
