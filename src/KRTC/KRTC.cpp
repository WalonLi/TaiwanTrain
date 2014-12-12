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
        if (data.empty()) continue ;

        // match pattern "opt group"
        if (PREFIX_COMPARE(match_pattern, data))
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

        if (PREFIX_COMPARE(view_pattern, data))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("\""), boost::token_compress_on) ;
            view_state = vec[7];
            // convert to html style
            view_state =  boost::replace_all_copy(view_state, "/", "%2F")  ;
            view_state =  boost::replace_all_copy(view_state, "+", "%2B")  ;
        }


        if (PREFIX_COMPARE(validate_pattern, data))
        {
            vector<string> vec ;
            boost::split(vec, data, boost::is_any_of("\""), boost::token_compress_on) ;
            validate = vec[7] ;
            // convert to html style
            validate =  boost::replace_all_copy(validate, "/", "%2F")  ;
            validate =  boost::replace_all_copy(validate, "+", "%2B")  ;
        }

    }
    stream.close();
    QTest::qWait(100) ;


    return STATE_SUCCESS ;
}


STATE KRTC::get_list_with_user_input(QDate date, string start, string arrival, QStringList & list)
{

    const string card_pattern("單程票\t\t普卡\t\t學生卡\t\t社福卡") ;
    const string single_pattern("<td align=\"right\"><span id=\"lblTicket\">") ;
    const string regular_pattern("<td align=\"right\"><span id=\"lblTicket1\">") ;
    const string student_pattern("<td align=\"right\"><span id=\"lblTicket3\">") ;
    const string welfare_pattern("<td align=\"right\"><span id=\"lblTicket2\">") ;
    const string shipping_pattern("<td align=\"right\"><span id=\"lblShipping\">") ;

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
    string AsyncPost("__ASYNCPOST=false") ;

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

    string price, ship_time ;
    std::stringstream web_stream ;
    web_stream << stream.rdbuf() ;
    while (std::getline(web_stream, data))
    {
        data = ttp::trim(data) ;
        if (data.empty()) continue ;

        if (PREFIX_COMPARE(single_pattern, data)
            || PREFIX_COMPARE(regular_pattern, data)
            || PREFIX_COMPARE(student_pattern, data)
            || PREFIX_COMPARE(welfare_pattern, data))
        {
            vector<string> temp ;
            boost::split(temp, data, boost::is_any_of("<>"), boost::token_compress_on) ;
            price += temp[3].c_str() ;
            if (!PREFIX_COMPARE(welfare_pattern, data)) price += "\t\t" ;
        }
        else if (PREFIX_COMPARE(shipping_pattern, data))
        {
            vector<string> temp ;
            boost::split(temp, data, boost::is_any_of("<>"), boost::token_compress_on) ;
            ship_time = (boost::format("註：站間行駛時間約%s鐘")% temp[3].c_str()).str();
        }
    }

    list.push_back(card_pattern.c_str());
    list.push_back(price.c_str());
    list.push_back(ship_time.c_str());
    return list.empty() ? STATE_DATA_NOT_FOUND : STATE_SUCCESS ;
}

}
