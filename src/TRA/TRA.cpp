/**
    Provide by Walon Li

    File: TRA.h
**/

#include "TRA.h"
#include <QDebug>
#include <QtTest>
#include <iostream>

namespace ttp {

STATE TRA::parse_data_from_web()
{
    vector<string> pages = get_web_pages() ;

    const string match_pattern("<script>") ;
    const string data_pre_pattern("<td title=\"");
    const string data_mid_pattern("\">") ;
    const string data_pos_pattern("</td>") ;
    const string data_end_pattern("<td title=\"\"><\/td>>") ;


    for(auto page_it = pages.begin() ; page_it != pages.end() ; ++page_it)
    {
        // Try to connect page.
        if (connect_server(10) != STATE_SUCCESS)
            return STATE_NETWORK_ERROR ;
        boost::asio::ip::tcp::iostream & stream = get_stream() ;

        // fill http header
        stream << "GET " << *page_it << " HTTP/1.1\r\n" ;
        stream << "Host: " << get_web_server() << "\r\n" ;
        stream << "Accept: */*\r\n" ;
        stream << "Cache-Control: no-cache\r\n" ;
        stream << "Connection: close\r\n\r\n" ;
        stream.flush() ;



        // Recieve header and check is invalid or not.
        string mHttpVer, data ;
        u_int mHttpCode ;

        qDebug() << "Get"<< get_web_server().c_str() << (*page_it).c_str() <<"..." ;
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
                qDebug() << data.c_str() <<"\n\n\n" ;
            }
        }
        stream.close();
        QTest::qWait(100) ;
    }

    return STATE_SUCCESS ;
}

}
