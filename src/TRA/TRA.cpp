/**
    Provide by Walon Li

    File: TRA.h
**/

#include "TRA.h"
#include <QDebug>
#include <QtTest>
#include <iostream>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ttp {

STATE TRA::parse_data_from_web()
{
    vector<string> pages = get_web_pages() ;

    const string match_pattern("<script>") ;
    const string station_pattern("TRStation.push");

    // Try to connect page.
    if (connect_server(10) != STATE_SUCCESS)
        return STATE_NETWORK_ERROR ;
    boost::asio::ip::tcp::iostream & stream = get_stream() ;

    // fill http header
    stream << "GET " << pages[0] << " HTTP/1.1\r\n" ;
    stream << "Host: " << get_web_server() << "\r\n" ;
    stream << "Accept: */*\r\n" ;
    stream << "Cache-Control: no-cache\r\n" ;
    stream << "Connection: close\r\n\r\n" ;
    stream.flush() ;



    // Recieve header and check is invalid or not.
    string mHttpVer, data ;
    u_int mHttpCode ;

    qDebug() << "Get"<< get_web_server().c_str() << pages[0].c_str() <<"..." ;
    stream >> mHttpVer >> mHttpCode ;
    if (!stream || mHttpVer.substr(0,5) != "HTTP/" || mHttpCode != 200)
        return STATE_NETWORK_ERROR ;
    else
        qDebug() << mHttpVer.c_str() << " " << mHttpCode << "OK" << endl;


    std::stringstream web_stream ;
    web_stream << stream.rdbuf() ;
    while (std::getline(web_stream, data))
    {
        // clean white space
        data = ttp::trim(data) ;
        if (data.empty() || data.size() < match_pattern.size())
            continue ;

        // match pattern "<script>"
        if (!data.substr(0, match_pattern.size()).compare(match_pattern))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("<>;"), boost::token_compress_on) ;
            for ( auto it = vec.begin() ; it != vec.end() ; ++it )
            {
                if (!(*it).substr(0, station_pattern.size()).compare(station_pattern))
                {
                    // locate number        (skip)
                    // station number
                    // station name
                    // station name(english)(skip)
                    ++it ;
                    vector<string> temp ;
                    boost::split(temp, *it++, boost::is_any_of("\'"), boost::token_compress_on) ;
                    int num = boost::lexical_cast<int>(temp[1]) ;
                    temp.clear();
                    boost::split(temp, *it++, boost::is_any_of("\'"), boost::token_compress_on) ;
                    add_station_into_map(temp[1], num) ;
                }
            }
        }
    }
    stream.close();
    QTest::qWait(100) ;


    return STATE_SUCCESS ;
}
STATE TRA::get_list_with_user_input(QDate date, string start, string arrival, QStringList & list)
{

    const string info_pattern("</font><\/td><td align=\"center\" width=\"25\"\>") ;
    const string id_pattern("<a id=\"TrainCodeHyperLink\"");

    string cmd ;
    string search_type("searchtype=0") ;
    string from_city("fromcity=") ;
    string to_city("tocity=") ;

    string from_station("fromstation=1008") ;
    string to_station("tostation=1238") ;

    string train_class("trainclass=2") ;
    string from_time("fromtime=0000") ;
    string to_time("totime=2359") ;

    string search_date = (boost::format("searchdate=%d\/%d\/%02d") \
                                        % date.year() \
                                        % date.month() \
                                        % date.day()).str() ;
    /*
    vector< pair<int, string> > st_map = get_station_map();
    for (auto it = st_map.begin() ; it != st_map.end() ; ++it)
    {
        if (!it->second.compare(start))
            from_station += (boost::format("%d") % it->first).str() ;
        else if (!it->second.compare(arrival))
            to_station += (boost::format("%d") % it->first).str() ;
    }
    */
    vector<string> pages = get_web_pages() ;
    cmd = pages[1].c_str() \
          + search_type + "&" \
          + search_date + "&" \
          + from_city + "&" \
          + to_city + "&" \
          + from_station + "&" \
          + to_station + "&" \
          + train_class + "&" \
          + from_time + "&" \
          + to_time;



    qDebug() << cmd.c_str() ;


    // Try to connect page.
    if (connect_server(10) != STATE_SUCCESS)
        return STATE_NETWORK_ERROR ;
    boost::asio::ip::tcp::iostream & stream = get_stream() ;

    // fill http header
    stream << "GET " << cmd << " HTTP/1.1\r\n" ;
    stream << "Host: " << get_web_server() << "\r\n" ;
    stream << "Accept: */*\r\n" ;
    stream << "Cache-Control: no-cache\r\n" ;
    stream << "Connection: close\r\n\r\n" ;
    stream.flush() ;


    // Recieve header and check is invalid or not.
    string mHttpVer, data ;
    u_int mHttpCode ;

    qDebug() << "Get"<< get_web_server().c_str() << pages[1].c_str() <<"..." ;
    stream >> mHttpVer >> mHttpCode ;
    if (!stream || mHttpVer.substr(0,5) != "HTTP/" || mHttpCode != 200)
        return STATE_NETWORK_ERROR ;
    else
        qDebug() << mHttpVer.c_str() << " " << mHttpCode << "OK" << endl;


    std::stringstream web_stream ;
    web_stream << stream.rdbuf() ;
    while (std::getline(web_stream, data))
    {
        data = ttp::trim(data) ;
        if (data.empty()) continue ;
        qDebug() << data.c_str() ;
    }


    return STATE_SUCCESS ;
}
}
