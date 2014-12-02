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
QStringList TRA::get_list_with_user_input(QDate date, QString start, QString arrival)
{
    QStringList list ;
    return list ;
}
}
