/**
    Provide by Walon Li

    File: KRTC.cpp
**/

#include "KRTC.h"
#include <QDebug>
#include <QtTest>
#include <iostream>
#include "boost/algorithm/string.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/format.hpp"

namespace ttp {

STATE KRTC::parse_data_from_web()
{

    vector<string> pages = get_web_pages() ;

    const string match_pattern("<option value=\"") ;
    const string view_pattern("<input type=\"hidden\" name=\"__VIEWSTATE\"") ;
    const string validate_pattern("<input type=\"hidden\" name=\"__EVENTVALIDATION\"") ;

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

    bool flag = false ;
    std::stringstream web_stream ;
    web_stream << stream.rdbuf() ;
    while (std::getline(web_stream, data))
    {
        // clean white space
        data = ttp::trim(data) ;
        if (data.empty() || data.size() < match_pattern.size())
            continue ;

        // match pattern "opt group"
        if (!data.substr(0, match_pattern.size()).compare(match_pattern))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("\""), boost::token_compress_on) ;

            if (vec[1] == "0")
            {
                if (!flag) flag = true ;
                else break ; // parse end.
            }
            else
            {
                add_station_into_map(vec[1]) ;
            }
        }

        if ( data.size() >= view_pattern.size() && !data.substr(0, view_pattern.size()).compare(view_pattern))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("\""), boost::token_compress_on) ;
            view_state = vec[7];
        }


        if ( data.size() >= validate_pattern.size() && !data.substr(0, validate_pattern.size()).compare(validate_pattern))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("\""), boost::token_compress_on) ;
            validate = vec[7] ;
        }

    }
    stream.close();
    QTest::qWait(100) ;


    return STATE_SUCCESS ;
}


STATE KRTC::get_list_with_user_input(QDate date, string start, string arrival, QStringList & list)
{

    const string match_pattern("<p align=\'right\'>") ;
    const string text_pattern("font size=\'-1\'") ;

    string form_item ;
    string ScriptMgr("ScriptManager1=UpdatePanel1|button3") ;
    string EventTarget("__EVENTTARGET=button3") ;
    string EventArg("__EVENTARGUMENT=") ;
    string ViewState("__VIEWSTATE=");
    string PosX("__SCROLLPOSITIONX=0") ;
    string PosY("__SCROLLPOSITIONY=0") ;
    string EventValidate("__EVENTVALIDATION=") ;
    string ddlstation1("ddlStation1=") ;
    string ddlstation2("ddlStation2=") ;
    string AsyncPost("__ASYNCPOST=true") ;

    vector< pair<int, string> > st_map = get_station_map();
    for (auto it = st_map.begin() ; it != st_map.end() ; ++it)
    {
        if (!it->second.compare(start))
            ddlstation1 += it->second ;
        else if (!it->second.compare(arrival))
            ddlstation2 += it->second ;
    }

    ViewState += view_state ;
    EventValidate += validate ;

    qDebug() << ViewState.c_str() ;
    vector<string> pages = get_web_pages() ;
    form_item = ScriptMgr + "&"
                + ddlstation1+ "&"
                + ddlstation2+ "&"
                + EventTarget+ "&"
                + EventArg+ "&"
                + ViewState+ "&"
                + PosX+ "&"
                + PosY+ "&"
                + EventValidate+ "&"
                + AsyncPost;

    // qDebug() << form_item.c_str() ;
    // Try to connect page.
    if (connect_server(10) != STATE_SUCCESS)
        return STATE_NETWORK_ERROR ;
    boost::asio::ip::tcp::iostream & stream = get_stream() ;

    // fill http header
    stream << "POST " << pages[0] << " HTTP/1.1\r\n" ;
    stream << "Host: " << get_web_server() << "\r\n" ;
    stream << "Accept: */*\r\n" ;
    stream << "Cache-Control: no-cache\r\n" ;
    stream << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64)\r\n" ;
    stream << "Content-Type: application/x-www-form-urlencoded; charset=UTF-8\r\n" ;
//    stream << "Content-Length: " << form_item.length() << "\r\n" ;
    stream << "Content-Length: " << form_item.size() << "\r\n" ;
    stream << "X-Requested-with: XMLHttpRequest\r\n" ;
    stream << "Connection: close\r\n\r\n" ;
    stream << form_item ;
    stream.flush() ;


    qDebug() << form_item.length() << form_item.size() ;


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
    string id, type ;
    while (std::getline(web_stream, data))
    {

        data = ttp::trim(data) ;
        if (data.empty()) continue ;

        qDebug() << data.c_str() ;
#if 0
        if (data.size() >= match_pattern.size() &&
                 !data.substr(0, match_pattern.size()).compare(match_pattern))
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
#endif
    }

    return list.empty() ? STATE_DATA_NOT_FOUND : STATE_SUCCESS ;
}

}
