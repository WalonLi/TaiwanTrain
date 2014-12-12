/**
    Provide by Walon Li

    File: TRTC.cpp
**/

#include "TRTC.h"
#include <QDebug>
#include <QtTest>
#include <iostream>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ttp {

STATE TRTC::parse_data_from_web()
{
    vector<string> pages = get_web_pages() ;

    const string optgroup_pattern("<optgroup style=") ;
    const string option_pattern("option style=");

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

    qDebug() << "Get" << get_web_server().c_str() << pages[0].c_str() <<"..." ;
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
        if (data.empty()) continue ;

        // match pattern "opt group"
        if (PREFIX_COMPARE(optgroup_pattern, data))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("<>"), boost::token_compress_on) ;
            for ( auto it = vec.begin() ; it != vec.end() ; ++it )
            {
                // match option
                if (PREFIX_COMPARE(option_pattern, (*it)))
                {
                    vector<string> temp ;
                    boost::split(temp, *it++, boost::is_any_of("\'"), boost::token_compress_on) ;
                    int num = boost::lexical_cast<int>(temp[3]) ;
                    add_station_into_map(*it, num) ;
                }
            }
            break ; // only match once
        }

    }
    stream.close();
    QTest::qWait(100) ;


    return STATE_SUCCESS ;
}
STATE TRTC::get_list_with_user_input(QDate date, string start, string arrival, QStringList & list)
{

    const string match_pattern("<p align=\'right\'>") ;
    const string text_pattern("font size=\'-1\'") ;

    string form_item ;
    string s1elect_item("s1elect=") ;
    string action_item("action=query") ;
    string s2elect_item("s2elect=") ;
    string submit_item("submit= 確定 ") ;

    vector< pair<int, string> > st_map = get_station_map();
    for (auto it = st_map.begin() ; it != st_map.end() ; ++it)
    {
        if (!it->second.compare(start))
            s1elect_item += (boost::format("%03d") % it->first).str() ;
        else if (!it->second.compare(arrival))
            s2elect_item += (boost::format("%03d") % it->first).str() ;
    }

    vector<string> pages = get_web_pages() ;
    form_item = s1elect_item + "&" + action_item+ "&" + s2elect_item+ "&" + submit_item ;

    qDebug() << form_item.c_str() ;
    // Try to connect page.
    if (connect_server(10) != STATE_SUCCESS)
        return STATE_NETWORK_ERROR ;
    boost::asio::ip::tcp::iostream & stream = get_stream() ;

    // fill http header
    stream << "POST " << pages[0] << " HTTP/1.1\r\n" ;
    stream << "Host: " << get_web_server() << "\r\n" ;
    stream << "Accept: */*\r\n" ;
    stream << "Cache-Control: no-cache\r\n" ;
    stream << "Content-Type: application/x-www-form-urlencoded\r\n" ;
    stream << "Content-Length: " << form_item.length() << "\r\n" ;
    stream << "Connection: close\r\n\r\n" ;
    stream << form_item ;
    stream.flush() ;


    // Recieve header and check is invalid or not.
    string mHttpVer, data ;
    u_int mHttpCode ;

    qDebug() << "Post" << get_web_server().c_str() << pages[0].c_str() <<"..." ;
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

        if (PREFIX_COMPARE(match_pattern, data))
        {
            vector<string> temp ;
            boost::split(temp, data, boost::is_any_of("<>"), boost::token_compress_on) ;
            int count = 0 ;
            string str ;
            for (auto it = temp.begin() ; it != temp.end() ; ++it)
            {
                if (it->size() == text_pattern.size() && !it->compare(text_pattern))
                {
                    ++it ; // skip pattern
                    // magic number.... Q__Q, we can not use tag to distinguish it.
                    if (count < 3)
                    {
                        str += *it + "\t\t" ;
                        if (count == 2)
                        {
                            list.push_back(str.c_str());
                            str.clear();
                        }
                    }
                    else if (count < 6)
                    {
                        str += *it + "\t\t" ;
                        if (count == 5)
                        {
                            list.push_back(str.c_str());
                            str.clear();
                        }
                    }
                    else if (count < 9)
                    {
                        str += *it ;
                        if (count == 8)
                        {
                            list.push_back(str.c_str());
                            str.clear();
                        }
                    }

                    count++ ;
                }
            }
            break ;
        }
    }

    return list.empty() ? STATE_DATA_NOT_FOUND : STATE_SUCCESS ;
}

}
